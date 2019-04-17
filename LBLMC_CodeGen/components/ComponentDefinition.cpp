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

#include "ComponentDefinition.hpp"

#include <string>
#include <sstream>
#include <stdexcept>

namespace lblmc
{

ComponentDefinition::ComponentDefinition() :
	name(""),
	parameters(),
	fields(),
	inputs(),
	outputs(),
	update_body(),
	num_sources(0),
	num_terminals(0)
{}


ComponentDefinition::ComponentDefinition(const ComponentDefinition& base) :
	name(base.name),
	parameters(base.parameters),
	fields(base.fields),
	inputs(base.inputs),
	outputs(base.outputs),
	update_body(base.update_body),
	num_sources(base.num_sources),
	num_terminals(base.num_terminals)
{}

void ComponentDefinition::reset()
{
	name = "";
	parameters.clear();
	fields.clear();
	inputs.clear();
	outputs.clear();
	update_body = "";
	num_sources = 0;
	num_terminals = 0;
}

void ComponentDefinition::setName(std::string name)
{
	this->name = name;
}

void ComponentDefinition::addParameter(VariableDefinition parameter)
{
	parameters.push_back(parameter);
}

void ComponentDefinition::addField(VariableDefinition field)
{
	fields.push_back(field);
}

void ComponentDefinition::addInput(VariableDefinition input)
{
	inputs.push_back(input);
}

void ComponentDefinition::addOutput(VariableDefinition output)
{
	outputs.push_back(output);
}

void ComponentDefinition::setUpdateBody(FunctionBody update_body)
{
	this->update_body = update_body;
}

void ComponentDefinition::setNumberOfSources(unsigned int ns)
{
	num_sources = ns;
}

void ComponentDefinition::setNumberOfTerminals(unsigned int nt)
{
	num_terminals = nt;
}

void ComponentDefinition::generateParametersCInlineCode(std::string& buf, std::string instance_name, std::vector<std::string> parameter_values)
{
	if(name == "")
		throw std::runtime_error("ComponentDefinition::generateParametersCInlineCode(): cannot generate code without component name being left undefined");

	if(parameter_values.size() != parameters.size())
		throw std::invalid_argument("ComponentDefinition::generateParametersCInlineCode(): number of given parameter values must equal number of parameters");

	for(auto pv : parameter_values)
	{
		if(pv == "")
			throw std::invalid_argument("ComponentDefinition::generateParametersCInlineCode(): none of the given parameter values can be null or empty");
	}

	std::stringstream sstrm;

	for(int i = 0; i < parameters.size(); i++)
	{
		auto& p = parameters[i];
		sstrm << "const static " <<p.type <<" "<< p.name <<"_"<<name<<"_"<<instance_name<<" = "<<parameter_values[i]<<";\n";
	}

	buf = sstrm.str();
}

void ComponentDefinition::generateFieldsCInlineCode(std::string& buf, std::string instance_name)
{
	if(name == "")
		throw std::runtime_error("ComponentDefinition::generateFieldsCInlineCode(): cannot generate code without component name being left undefined");

	std::stringstream sstrm;

	for(auto f : fields)
	{
		if(f.default_value != "")
			sstrm << f.type <<" "<< f.name <<"_"<<name<<"_"<<instance_name<<" = "<<f.default_value<<";\n";
		else
			sstrm << f.type <<" "<< f.name <<"_"<<name<<"_"<<instance_name<<";\n";
	}

	buf = sstrm.str();
}

void ComponentDefinition::generateInputsCInlineCode(std::string& buf, std::string instance_name, bool append_trailing_comma)
{
	if(name == "")
		throw std::runtime_error("ComponentDefinition::generateInputsCInlineCode(): cannot generate code without component name being left undefined");

	std::stringstream sstrm;

	if(inputs.empty())
	{
		buf = "";
		return;
	}

	auto b = inputs.begin();
	sstrm << b->type <<" "<< b->name <<"_in_"<<name<<"_"<<instance_name;

	for(auto i = ++b; i != inputs.end(); i++ )
	{
		sstrm
		<< ",\n"
		<< i->type <<" "<< i->name <<"_in_"<<name<<"_"<<instance_name;
	}

	if(append_trailing_comma)
		sstrm << ",\n";

	buf = sstrm.str();
}

void ComponentDefinition::generateOutputsCInlineCode(std::string& buf, std::string instance_name, bool append_trailing_comma)
{
	if(name == "")
		throw std::runtime_error("ComponentDefinition::generateOutputsCInlineCode(): cannot generate code without component name being left undefined");

	std::stringstream sstrm;

	if(outputs.empty())
	{
		buf = "";
		return;
	}

	auto b = outputs.begin();
	sstrm << b->type <<" "<< b->name <<"_out_"<<name<<"_"<<instance_name;

	for(auto i = ++b; i != outputs.end(); i++ )
	{
		sstrm
		<< ",\n"
		<< i->type <<" "<< i->name <<"_out_"<<name<<"_"<<instance_name;
	}

	if(append_trailing_comma)
		sstrm << ",\n";

	buf = sstrm.str();
}

void ComponentDefinition::generateUpdateBodyCInlineCode
(
	std::string& code,
	std::string instance_name,
	std::vector<unsigned int> terminal_indices,
	std::vector<unsigned int> source_indices
)

{
	if(terminal_indices.size() != num_terminals)
	throw std::invalid_argument("ComponentDefinition::generateUpdateBodyCInlineCode():  given number of terminal indices must equal number of terminals");

	if(source_indices.size() != num_sources)
	throw std::invalid_argument("ComponentDefinition::generateUpdateBodyCInlineCode():  given number of sources indices must equal number of sources");

	std::stringstream sstrm;

	sstrm << std::string(update_body) << "\n\n";

	code = sstrm.str();
}


} //namespace lblmc
