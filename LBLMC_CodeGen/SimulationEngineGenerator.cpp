/*

Copyright (C) 2019 Matthew Milton

This file is part of the LB-LMC Solver C++ Code Generation Library.

LB-LMC Solver C++ Code Generation Library is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LB-LMC Solver C++ Code Generation Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LB-LMC Solver C++ Code Generation Library.  If not, see <https://www.gnu.org/licenses/>.

*/

#include "SimulationEngineGenerator.hpp"

#include <stdexcept>
#include <sstream>
#include <fstream>

#include "codegen/ArrayObject.hpp"

namespace lblmc
{

SimulationEngineGenerator::SimulationEngineGenerator
(
	std::string model_name,
	unsigned int num_solutions
) :
	model_name(model_name),
	num_solutions(num_solutions),
	comp_parameters(),
	comp_fields(),
	comp_inputs(),
	comp_outputs(),
	comp_outputs_update_bodies(),
	comp_update_bodies(),
	conductance_matrix_gen(num_solutions),
	source_vector_gen(num_solutions),
	parameters()
{
	if(model_name == "")
		throw std::runtime_error("SimulationEngineGenerator::constructor(): model_name cannot be null or empty");

	if(num_solutions == 0)
		throw std::runtime_error("SimulationEngineGenerator::constructor(): num_solutions must be positive nonzero value");
}

SimulationEngineGenerator::SimulationEngineGenerator(const SimulationEngineGenerator& base) :
	model_name(base.model_name),
	num_solutions(base.num_solutions),
	comp_parameters(base.comp_parameters),
	comp_fields(base.comp_fields),
	comp_inputs(base.comp_inputs),
	comp_outputs(base.comp_outputs),
	comp_outputs_update_bodies(base.comp_outputs_update_bodies),
	comp_update_bodies(base.comp_update_bodies),
	conductance_matrix_gen(base.conductance_matrix_gen),
	source_vector_gen(base.source_vector_gen),
	parameters(base.parameters)
{}

void SimulationEngineGenerator::reset(std::string model_name, unsigned int num_solutions)
{
	if(model_name == "")
		throw std::runtime_error("SimulationEngineGenerator::reset(): model_name cannot be null or empty");

	if(num_solutions == 0)
		throw std::runtime_error("SimulationEngineGenerator::reset(): num_solutions must be positive nonzero value");

	this->model_name = model_name;
	this->num_solutions = num_solutions;
	this->comp_parameters.clear();
	this->comp_fields.clear();
	this->comp_inputs.clear();
	this->comp_outputs.clear();
	this->comp_outputs_update_bodies.clear();
	this->comp_update_bodies.clear();
	this->conductance_matrix_gen = SystemConductanceGenerator(num_solutions);
	this->source_vector_gen = SystemSourceVectorGenerator(num_solutions);
}

void SimulationEngineGenerator::setModelName(std::string model_name)
{
	if(model_name == "")
		throw std::runtime_error("SimulationEngineGenerator::setModelName(): model_name cannot be null or empty");

	this->model_name = model_name;
}

std::string SimulationEngineGenerator::getModelName() const
{
	return model_name;
}

SystemConductanceGenerator&  SimulationEngineGenerator::getConductanceGenerator()
{
	return conductance_matrix_gen;
}

SystemSourceVectorGenerator& SimulationEngineGenerator::getSourceVectorGenerator()
{
	return source_vector_gen;
}

void SimulationEngineGenerator::insertComponentParametersCode(std::string& code)
{
	if(code.empty()) return;
	comp_parameters.push_back(code);
}

void SimulationEngineGenerator::insertComponentFieldsCode(std::string& code)
{
	if(code.empty()) return;
	comp_fields.push_back(code);
}

void SimulationEngineGenerator::insertComponentInputsCode(std::string& code)
{
	if(code.empty()) return;
	comp_inputs.push_back(code);
}

void SimulationEngineGenerator::insertComponentOutputsCode(std::string& code)
{
	if(code.empty()) return;
	comp_outputs.push_back(code);
}

void SimulationEngineGenerator::insertComponentOutputsUpdateBody(std::string& code)
{
	if(code.empty()) return;
	comp_outputs_update_bodies.push_back(code);
}

void SimulationEngineGenerator::insertComponentUpdateBody(std::string& code)
{
	if(code.empty()) return;
	comp_update_bodies.push_back(code);
}

std::string SimulationEngineGenerator::generateCFunctionParameterList() const
{
	std::stringstream sstrm;

	codegen::ArrayObject x_out("real", "x_out", "", {num_solutions});
	sstrm << x_out.generateArgument();

	if( parameters.io_signal_output_enable && !comp_outputs.empty())
	{
		sstrm << ",\n";

		auto iter = comp_outputs.begin();

		sstrm << *iter;

		for(iter = iter+1; iter != comp_outputs.end(); iter++)
		{
            sstrm << ",\n"
                  << *iter;
		}
	}

	if(!comp_inputs.empty())
	{
		sstrm << ",\n";

		auto iter = comp_inputs.begin();

		sstrm << *iter;

		for(iter = iter+1; iter != comp_inputs.end(); iter++)
		{
            sstrm << ",\n"
                  << *iter;
		}
	}

	return sstrm.str();
}

std::string SimulationEngineGenerator::generateCInlineCode(double zero_bound) const
{
	std::stringstream sstrm;

	SystemConductanceGenerator invg_gen(conductance_matrix_gen);
	invg_gen.invertSelf();
	const double * invg = invg_gen.asArray();

	unsigned int num_components = source_vector_gen.getNumSources();

	SystemSolverGenerator solver_gen(invg, num_solutions, num_components, zero_bound);

	std::string buf;

	//codegen xilinx HLS features
	if(parameters.xilinx_hls_enable)
	{
		sstrm << "//clock period=" << parameters.xilinx_hls_clock_period << "\n";

		if(parameters.xilinx_hls_inline)
		{
			sstrm << "#pragma HLS inline\n";
		}

		if(parameters.xilinx_hls_latency_enable)
		{
			sstrm << "#pragma HLS latency min="<<parameters.xilinx_hls_latency_min<<
			         " max="<<parameters.xilinx_hls_latency_max<<"\n";
		}

		sstrm << "\n";
	}

	sstrm << "//MODEL PARAMETERS\n\n";

	for(auto i : comp_parameters)
	{
		sstrm << i << "\n";
	}
	sstrm << "\n";

	sstrm << "//COMPONENT FIELDS AND STATES\n\n";

	for(auto i : comp_fields)
	{
		sstrm << i << "\n";
	}
	sstrm << "\n";

	sstrm << "//MODEL SOLUTIONS\n\n";

	sstrm
	<< "static real b["<<num_solutions<<"];\n"
	<< "static real x["<<num_solutions+1<<"];\n"
	<< "real b_components["<<num_components<<"];\n\n";

	sstrm << "//INVERTED CONDUCTANCE MATRIX\n\n";

	buf = invg_gen.asCLiteral("inv_g");
	sstrm << buf << "\n\n";

	sstrm << "//COMPONENT SOURCE CONTRIBUTION UPDATES\n\n";

	for(auto i : comp_update_bodies)
	{
		sstrm << i << "\n";
	}
	sstrm << "\n";

	if(parameters.io_signal_output_enable)
	{
		sstrm << "//MODEL OUTPUT SIGNAL UPDATES\n\n";

		for(auto i : comp_outputs_update_bodies)
		{
			sstrm << i << "\n";
		}
		sstrm << "\n";
	}

	sstrm << "//AGGREGRATE COMPONENT SOURCE CONTRIBUTIONS\n\n";

	source_vector_gen.asCInlineCode(buf);
	sstrm << buf << "\n\n";

	sstrm << "//MODEL UPDATE SOLUTIONS\n\n";

	solver_gen.generateCInlineCode(buf, "inv_g");
	sstrm << buf << "\n\n";

	return sstrm.str();
}

std::string SimulationEngineGenerator::generateCFunction(double zero_bound) const
{
	std::stringstream sstrm;

	sstrm
	<< "void "<<model_name<<"_simulationEngine\n"
	<< "(\n";

	sstrm
	<< generateCFunctionParameterList()
	<< "\n)\n"
	<< "{\n";

	std::string buf;
	buf = generateCInlineCode(zero_bound);
	sstrm << buf;

	for(unsigned int i = 0; i < num_solutions; i++)
	{
		sstrm << "x_out["<<i<<"] = x["<<i+1<<"];\n";
	}

	sstrm
	<< "\n}";

	return sstrm.str();
}

void SimulationEngineGenerator::generateCFunctionAndExport(std::string filename, double zero_bound) const
{
	if(filename == "")
		throw std::invalid_argument("SimulationEngineGenerator::generateCFunctionAndExport(): filename cannot be null or empty");

	std::fstream file;

	std::string fname = filename;

	try
	{
		file.open(fname.c_str(), std::fstream::out | std::fstream::trunc);
	}
	catch(...)
	{
		throw std::runtime_error("SimulationEngineGenerator::generateCFunctionAndExport(): failed to open or create source files");
	}

	file <<
			"/**\n"
			" *\n"
			" * LBLMC Vivado HLS Simulation Engine for FPGA Designs\n"
			" *\n"
			" * Auto-generated by SimulationEngineGenerator Object\n"
			" *\n"
			" */\n\n";

	file << "#ifndef " << model_name << "_SIMULATIONENGINE_HPP" << "\n";
	file << "#define " << model_name << "_SIMULATIONENGINE_HPP" << "\n";

	file << "\n\n";

	if(parameters.fixed_point_enable)
	{
		if(parameters.xilinx_hls_enable)
		{
			file <<
			"#include <ap_fixed.h>\n" <<
			"typedef ap_fixed<"<<parameters.fixed_point_word_width<<", "<<
			parameters.fixed_point_int_width<<", AP_RND> real;\n\n";

		}
		else
		{
			file << "//platform-agnostic fixed point not supported yet. Using double real values\n"<<
			"typedef double real;\n\n";
		}
	}
	else
	{
		file << "typedef double real;\n\n";
	}

	file << "inline\n";

    std::string buf;
    buf = generateCFunction(zero_bound);
    file << buf << "\n\n";

	file << "\n#endif";

	file.close();

}

} //namespace lblmc
