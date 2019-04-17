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

#ifndef LBLMC_SYSTEMMODEL_HPP
#define LBLMC_SYSTEMMODEL_HPP

#include <vector>
#include <memory>
#include <string>
#include "Component.hpp"
#include "../SimulationEngineGenerator.hpp"

namespace lblmc
{

class Component;
class SimulationEngineGenerator;

/**
	\brief Defines a LB-LMC Method System Model, composed of Component Objects, which from a C++
	solver can be code generated to simulate

	Objects of this class store Component objects that define a system network model using LB-LMC
	method.  From these components and their network connections, a SystemModel object can generate
	C++ solver code, using SimulationEngineGenerator, to compute solutions and simulate for the
	model.

	SystemModel objects require components to all have unique names in same model for code
	generation, and these names must be valid C++ labels (no operators, no spaces, and
	no reserved words; only alphanumerical and underscore characters allowed).

	\author Matthew Milton
	\date 2019
**/
class SystemModel
{
private:
	std::vector<std::unique_ptr<Component>> components;
	SimulationEngineGenerator sim_eng_gen;

public:

	SystemModel() = delete;

	/**
		\brief parameter constructor
		\param model_name name/label of the model; must be valid C++ label
		\param num_solutions number of solutions (across and through) to solve for model
	**/
	SystemModel(std::string model_name, unsigned int num_solutions);

	SystemModel(const SystemModel& base) = delete;
	SystemModel(SystemModel&& base) = delete;

	/**
		\return name/label of model
	**/
	std::string getModelName() const;

	/**
		\return number of solutions (across and through) to solve for model
	**/
	unsigned int getNumberOfSolutions() const;

	/**
		\return number of components in model
	**/
	unsigned int getNumberOfComponents() const;

	/**
		\brief inserts a component into the system model

		The system model will take full ownership of the component object inserted.  As such, the
		component object should NOT be deleted or destroyed by anything other than the system model
		owning the component.  Also, the component added to the system model should not be a
		temporary object that can go out of scope (in other words, should not reside in stack
		memory).

		\param component pointer to component to insert
		\return reference to component now owned by system model

	**/
	Component& addComponent(Component* component);

	/**
		\brief inserts a component into the system model

		The system model will take full ownership of the component object inserted.  As such, the
		component object should NOT be deleted or destroyed by anything other than the system model
		owning the component.  Also, the component added to the system model should not be a
		temporary object that can go out of scope (in other words, should not reside in stack
		memory).

		\param component unique pointer to component to insert
		\return reference to component now owned by system model

	**/
	Component& addComponent(std::unique_ptr<Component>& component);

	/**
		\brief removes a component in the system model by name

		This method removes all instances of a component by given name

		/param name name of the component(s) to remove from system model
	**/
	void removeComponent(std::string name);

	/**
		\brief gets a component in system model by name

		This method returns first found component with given name, regardless of how many components
		have same name.

		The returned pointer is an observing raw pointer that should NOT be used to delete the
		component.

		\param name name of the component to get
		\return raw observing pointer to component; nullptr if no component of name is found
	**/
	Component* getComponent(std::string name);

	/**
		\brief gets a component in system model by name

		This method returns first found component with given name, regardless of how many components
		have same name.

		The returned pointer is an observing raw pointer that should NOT be used to delete the
		component.  The component object is treated as constant.

		\param name name of the component to get
		\return constant raw observing pointer to component; nullptr if no component of name is
		found
	**/
	const Component* getComponent(std::string name) const;

	/**
		\brief checks if components of system model all have unique names

		SystemModel objects require components to all have unique names in same model for code
		generation, and these names must be valid C++ labels (no operators, no spaces, and
		no reserved words; only alphanumerical and underscore characters allowed).

		\return true if component names are all unique; false otherwise
	**/
	bool componentNamesUnique() const;

	/**
		\brief setups code generation using components stored in system model

		This method should be called before calling generateSolverCode() or
		generateSolverCodeAndExport() methods and after inserting components into model.  This
		method should also be called again at least once if components are added or removed since
		last call to this method.

		\throw std::runtime_error if component names are not unique or no components exist in model

	**/
	void setupSolverCodeGenerator();

	/**
		\return the solver code generator used by system model to generate code
	**/
	inline SimulationEngineGenerator& getSolverCodeGenerator()
	{
		return sim_eng_gen;
	}

	/**
		\brief generates C++ definition of a function that solves the system model

		\note the generated function contains static variables and therefore not usable for
		multiple instances of the solver, nor is thread safe.

		\param zero_bound range from zero where elements in system inverted conductance matrix and
		operations on said elements are discarded during code generation

		\return string containing the C++ definition of the solver function

	**/
	std::string generateSolverCode(double zero_bound = 1.0e-12) const;

	/**
		\brief generates C++ definition of a function that solves the system model and exports the
		code to file

		\note the generated function contains static variables and therefore not usable for
		multiple instances of the solver, nor is thread safe.

		\param filename name of the file (header) that will store the solver function definition
		\param zero_bound range from zero where elements in system inverted conductance matrix and
		operations on said elements are discarded during code generation

		\return string containing the C++ definition of the solver function

	**/
	void generateSolverCodeAndExport(std::string filename, double zero_bound = 1.0e-12) const;
};

} //namespace lblmc

#endif // LBLMC_SYSTEMMODEL_HPP
