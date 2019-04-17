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

#ifndef LBLMC_COMPONENTDEFINITION_HPP
#define LBLMC_COMPONENTDEFINITION_HPP

#include <string>
#include <vector>

namespace lblmc
{

struct VariableDefinition
{
	std::string type;
	std::string name;
	std::string default_value;

	VariableDefinition() : type(""), name(""), default_value("") {}
	VariableDefinition(std::string type, std::string name, std::string default_value) :
		type(type), name(name), default_value(default_value) {}
	VariableDefinition(const VariableDefinition& base) :
		type(base.type), name(base.name), default_value(base.default_value) {}
};

typedef std::string FunctionBody;

class ComponentDefinition
{
private:

	std::string name;
	std::vector<VariableDefinition> parameters;
	std::vector<VariableDefinition> fields;
	std::vector<VariableDefinition> inputs;
	std::vector<VariableDefinition> outputs;
	FunctionBody update_body;
	unsigned int num_sources;
	unsigned int num_terminals;

public:
	ComponentDefinition();
	ComponentDefinition(const ComponentDefinition& base);

	void reset();
	void setName(std::string name);
	void addParameter(VariableDefinition parameter);
	void addField(VariableDefinition field);
	void addInput(VariableDefinition input);
	void addOutput(VariableDefinition output);
	void setUpdateBody(FunctionBody update_body);
	void setNumberOfSources(unsigned int ns);
	void setNumberOfTerminals(unsigned int nt);

	void generateParametersCInlineCode(std::string& buf, std::string instance_name, std::vector<std::string> parameter_values);
	void generateFieldsCInlineCode(std::string& buf, std::string instance_name);
	void generateInputsCInlineCode(std::string& buf, std::string instance_name, bool append_trailing_comma=false);
	void generateOutputsCInlineCode(std::string& buf, std::string instance_name, bool append_trailing_comma=false);
	void generateUpdateBodyCInlineCode(std::string& code, std::string instance_name, std::vector<unsigned int> terminal_indices, std::vector<unsigned int> source_indices);

};

} //namespace lblmc

#endif // LBLMC_COMPONENTDEFINITION_HPP
