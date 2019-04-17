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

#ifndef LBLMC_COMPONENT_HPP
#define LBLMC_COMPONENT_HPP

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace lblmc
{

class SystemConductanceGenerator;
class SystemSourceVectorGenerator;
class SimulationEngineGenerator;

/**
	\brief base class for LB-LMC component models for simulation engine code generation

	Unlike ComponentDefinition which provides for more general generation of component model code,
	descendants of Component provide specific, hard-coded generation of code for component models.
	The descendants of Component act as a placeholder until the general component code generation
	is completed.

	Component and its descendants cannot be used for simulation directly, nor for HDL synthesis.

	\author Matthew Milton
	\date 2019
**/
class Component
{

protected:

	std::string comp_name;

public:

	Component(std::string comp_name = "") : comp_name(comp_name) {}
	Component(const Component& base) : comp_name(base.comp_name) {}

	inline void setName(std::string name)
	{
		if(name == "")
		{
			throw std::invalid_argument("Component::setName(): comp_name must be a valid, non-empty C++ label");
		}

		comp_name = name;
	}

	inline const std::string& getName() const { return comp_name; }

	inline virtual unsigned int getNumberOfTerminals() const { return 0; }
	inline virtual unsigned int getNumberOfSources() const { return 0; }
	inline virtual void getSourceIds(std::vector<unsigned int>& ids) const {}

	inline virtual void setTerminalConnections(std::vector<unsigned int> term_ids) {}

	inline virtual void setIntegrationMethod(std::string method) {}
	inline virtual std::string getIntegrationMethod() const { return std::string(""); }

	inline virtual std::vector<std::string> getSupportedInputs() const { return std::vector<std::string>(); }
	inline virtual std::vector<std::string> getSupportedOutputs() const { return std::vector<std::string>(); }

	virtual void stampConductance(SystemConductanceGenerator& gen) {}
	virtual void stampSources(SystemSourceVectorGenerator& gen) {}
	virtual void stampSystem(SimulationEngineGenerator& gen, std::vector<std::string> outputs = {"ALL"});
	virtual std::string generateParameters() { return std::string(""); }
	virtual std::string generateFields() { return std::string(""); }
	virtual std::string generateInputs() { return std::string(""); }
	virtual std::string generateOutputs(std::string output = "ALL") { return std::string(""); }
	virtual std::string generateOutputsUpdateBody(std::string output = "ALL") { return std::string(""); }
	virtual std::string generateUpdateBody() { return std::string(""); }

protected:

	inline std::string& appendName(std::string& var)
	{
		var += "_";
		var += comp_name;
		return var;
	}

	inline std::string& appendName(std::string&& var)
	{
		var += "_";
		var += comp_name;
		return var;
	}

	inline void generateParameter(std::stringstream& sstrm, std::string var, double value)
	{

		sstrm << "const static "<<"real "<<appendName(var)<<" = "<<value<<";\n";
	}

	inline void generateField(std::stringstream& sstrm, std::string var, double value)
	{

		sstrm << "static "<<"real "<<appendName(var)<<" = "<<value<<";\n";
	}

	inline void generateBoolField(std::stringstream& sstrm, std::string var, bool value)
	{
		sstrm << "static "<<"bool "<<appendName(var)<<" = "<< (value?"true":"false") <<";\n";
	}

};

} //namespace lblmc

#endif // LBLMC_COMPONENT_HPP
