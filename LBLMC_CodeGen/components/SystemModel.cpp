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

#include "SystemModel.hpp"
#include "Component.hpp"
#include "../SimulationEngineGenerator.hpp"

#include <stdexcept>

namespace lblmc
{

SystemModel::SystemModel(std::string model_name, unsigned int num_solutions) :
	components(),
	sim_eng_gen(model_name, num_solutions)
{}

std::string SystemModel::getModelName() const
{
	return sim_eng_gen.getModelName();
}

unsigned int SystemModel::getNumberOfSolutions() const
{
	return sim_eng_gen.getNumberOfSolutions();
}

unsigned int SystemModel::getNumberOfComponents() const
{
	return components.size();
}

Component& SystemModel::addComponent(Component* component)
{
	if(component == nullptr)
		throw std::invalid_argument("SystemModel::addComponent(Component*): component cannot be null or nonexistent");

	components.push_back(std::unique_ptr<Component>{component});
	return *(components.back());
}


Component& SystemModel::addComponent(std::unique_ptr<Component>& component)
{
	if(component == nullptr)
		throw std::invalid_argument("SystemModel::addComponent(std::unique_ptr<Component>&): component cannot be null or nonexistent");

	components.push_back(std::move(component));
	return *(components.back());
}

void SystemModel::removeComponent(std::string name)
{
	auto iter = components.begin();

	while(iter != components.end())
	{
		if( (*iter)->getName() == name )
			iter = components.erase(iter);
		else
			iter++;
	}
}

Component* SystemModel::getComponent(std::string name)
{
	auto iter = components.begin();

	while(iter != components.end())
	{
		if( (*iter)->getName() == name )
			return (*iter).get();
		else
			iter++;
	}

	return nullptr;
}

const Component* SystemModel::getComponent(std::string name) const
{
	auto iter = components.begin();

	while(iter != components.end())
	{
		if( (*iter)->getName() == name )
			return (*iter).get();
		else
			iter++;
	}

	return nullptr;
}

bool SystemModel::componentNamesUnique() const
{
	for(auto i = components.begin(); i != components.end(); i++)
	{
		for(auto j = i+1; j != components.end(); j++)
		{
            if( (*i)->getName() == (*j)->getName() )
				return false;
		}
	}

	return true;
}

void SystemModel::setupSolverCodeGenerator()
{
	if(!componentNamesUnique())
		throw std::runtime_error("SystemModel::setupSolverGenerator(...): components must all have unique names");

	if(components.size() == 0)
		throw std::runtime_error("SystemModel::setupSolverGenerator(...): model must have components");

	auto model_name = sim_eng_gen.getModelName();
	auto num_solutions = sim_eng_gen.getNumberOfSolutions();
	sim_eng_gen.reset(model_name, num_solutions);

	for(auto& component : components)
	{
		component->stampSystem(sim_eng_gen);
	}
}

std::string SystemModel::generateSolverCode(double zero_bound) const
{
	return sim_eng_gen.generateCFunction(zero_bound);
}

void SystemModel::generateSolverCodeAndExport(std::string filename, double zero_bound) const
{
	sim_eng_gen.generateCFunctionAndExport(filename, zero_bound);
}

} //namespace lblmc
