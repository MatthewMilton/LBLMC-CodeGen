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

#ifndef LBLMC_COMPONENTPRODUCER_HPP
#define LBLMC_COMPONENTPRODUCER_HPP

#include <string>
#include <vector>
#include <memory>

#include "Component.hpp"
#include "../netlist/NetlistComponent.hpp"

namespace lblmc
{

/**
	\brief
**/
class ComponentProducer
{
public:

	ComponentProducer() {}
	ComponentProducer(const ComponentProducer& base) {}

	virtual std::unique_ptr<Component> operator()(NetlistComponent& component_def) = 0;
	virtual std::unique_ptr<Component> operator()
	(
		std::string label,
		std::vector<double> parameters,
		std::vector<unsigned int> terminal_connections
	) = 0;

};

} //namespace lblmc

#endif // LBLMC_COMPONENTPRODUCER_HPP
