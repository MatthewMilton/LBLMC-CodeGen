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

#include "Resistor.hpp"
#include "../SystemConductanceGenerator.hpp"
#include "../SystemSourceVectorGenerator.hpp"

namespace lblmc
{

Resistor::Resistor(std::string comp_name) :
	Component(comp_name),
	RES(1.0),
	P(0),
	N(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("Resistor::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}


Resistor::Resistor(std::string comp_name, double res) :
	Component(comp_name),
	RES(res),
	P(0),
	N(0)
{
	if(res <= 0)
	{
		throw std::invalid_argument("Resistor::constructor(): res must be positive nonzero value");
	}

	if(comp_name == "")
	{
		throw std::invalid_argument("Resistor::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}


Resistor::Resistor(const Resistor& base) :
	Component(base),
	RES(base.RES),
	P(base.P),
	N(base.N)
{}

void Resistor::stampConductance(SystemConductanceGenerator& gen)
{
	gen.stampConductance(1.0/RES, P, N);
}

} //namespace lblmc
