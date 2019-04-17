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

#ifndef LBLMC_FUNCTION_DEFINITION_HPP
#define LBLMC_FUNCTION_DEFINITION_HPP

#include <string>
#include <vector>

#include "VariableDefinition.hpp"

namespace lblmc
{

class FunctionDefinition
{
private:
	VariableDefinition return_type;
	std::vector<VariableDefinition> parameters;
	std::string function_body;

public:

	FunctionDefinition() :
		return_type(),
		parameters(),
		function_body("")
	{}

	inline void setReturnType(VariableDefinition return_type) { this->return_type = return_type; }
	inline void setFunctionBody(std::string function_body) { this->function_body = function_body; }

	inline VariableDefinition getReturnType() const { return return_type; }
	inline const std::string& getFunctionBody() const { return function_body; }

	inline void addParameter(VariableDefinition parameter) { parameters.push_back(parameter); }
	inline const std::vector<VariableDefinition>& getParameters() const { return parameters; }
	inline void getParameters(std::vector<VariableDefinition>& copy) const { copy = parameters; }

	void asFunctionDeclaration(std::string& decl) const;
	void asFunctionDefinition(std::string& def) const;
};

} //namespace lblmc

#endif // LBLMC_FUNCTION_DEFINITION_HPP
