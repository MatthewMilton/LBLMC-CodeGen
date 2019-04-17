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

#ifndef LBLMC_COMPONENT_DEFINITION_HPP
#define LBLMC_COMPONENT_DEFINITION_HPP

#include <string>
#include <vector>

#include "VariableDefinition.hpp"
#include "FunctionDefinition.hpp"

namespace lblmc
{

class ComponentDefinition;

/**
	\brief defines state update function of a component
**/
class ComponentUpdateFunctionDefinition
{
	friend class ComponentDefinition;

private:

	std::string function_body;
	std::string integration_method;

public:

	ComponentUpdateFunctionDefinition(std::string integration_method = "") :
		function_body(),
		integration_method(integration_method)
	{}

	ComponentUpdateFunctionDefinition(const ComponentUpdateFunctionDefinition& base) :
		function_body(base.function_body),
		integration_method(base.integration_body)
	{}

	inline void setFunctionBody(std::string function_body) { this->function_body = function_body; }
	inline void setIntegrationMethod(std::string integration_method) { this->integration_method = integration_method; }
	inline const std::string& getFunctionBody() const { return function_body; }
	inline const std::string& getIntegrationMethod() const { return integration_method; }

	inline void addStateVariable(VariableDefinition state) { states.push_back(state); }
	inline const std::vector<VariableDefinition>& getStateVariables() const { return states; }
	inline void getStateVariables(std::vector<VariableDefinition>& copy) const { copy = states; }

};

/**
	\brief definition of a LB-LMC solver component C++ model
**/
class ComponentDefinition
{
private:

	std::string component_name;	///< type name of component
	unsigned int num_terminals;	///< number of terminals (and companion sources)  of component
	std::vector<VariableDefinition> parameters;	///< parameters of component
	std::vector<VariableDefinition> states;	///< persistent state variables or registers of component
	std::vector<VariableDefinition> signal_inputs;	///< input signals to component
	std::vector<VariableDefinition> signal_outputs;	///< output signals from component
	std::vector<ComponentUpdateFunctionDefinition> update_functions;	///< function definitions for update methods of component
	std::vector<ComponentUpdateFunctionDefinition> conductance_stamp_functions;	///< conductance matrix stamp function definitions

public:

	ComponentDefinition() :
		component_name("UNNAMED_COMPONENT"),
		num_terminals(0),
		parameters(),
		states(),
		signal_inputs(),
		signal_outputs(),
		update_functions(),
		conductance_stamp_functions()
	{}

	ComponentDefinition(const ComponentDefinition& base) :
		component_name(base.component_name),
		num_terminals(base.num_terminals),
		parameters(base.parameters),
		states(base.states),
		signal_inputs(base.signal_inputs),
		signal_outputs(base.signal_outputs),
		update_functions(base.update_functions),
		conductance_stamp_functions(base.conductance_stamp_functions)
	{}

};

} //namespace lblmc

#endif // LBLMC_COMPONENT_DEFINITION_HPP
