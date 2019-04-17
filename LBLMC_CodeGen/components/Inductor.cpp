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

#include "Inductor.hpp"
#include "../SystemConductanceGenerator.hpp"
#include "../SystemSourceVectorGenerator.hpp"
#include "../codegen/Object.hpp"

#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace lblmc
{

Inductor::Inductor(std::string comp_name) :
	Component(comp_name),
	DT(1.0),
    IND(1.0),
    P(0),
    N(0),
    source_id(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("Inductor::constructor(): comp_name must be a valid, non-empty C++ label");
	}
}

Inductor::Inductor(std::string comp_name, double dt, double ind) :
	Component(comp_name),
	DT(dt),
	IND(ind),
	P(0),
	N(0),
	source_id(0)
{
	if(comp_name == "")
	{
		throw std::invalid_argument("Inductor::constructor(): comp_name must be a valid, non-empty C++ label");
	}

	if(DT <= 0.0 || IND <= 0.0)
	{
		throw std::invalid_argument("Inductor::constructor(): parameters dt and ind must be positive nonzero values");
	}
}

Inductor::Inductor(const Inductor& base) :
	Component(base),
	DT(base.DT),
	IND(base.IND),
	P(base.P),
	N(base.N),
	source_id(base.source_id)
{}

void Inductor::getSourceIds(std::vector<unsigned int>& ids) const
{
	ids.clear();
	ids.push_back(source_id);
}

void Inductor::stampConductance(SystemConductanceGenerator& gen)
{
	const double HOL2 = DT/2.0/IND;

	gen.stampConductance(HOL2, P, N);
}

void Inductor::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id = gen.insertSource(P,N);
}

std::string Inductor::generateParameters()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	const double HOL2 = DT/2.0/IND;

	sstrm <<
	"const static "<<"real "<<appendName("DT")<<" = "<<DT<<";\n" <<
	"const static "<<"real "<<appendName("IND")<<" = "<<IND<<";\n"<<
	"const static "<<"real "<<appendName("HOL2")<<" = "<<HOL2<<";\n";

	return sstrm.str();
}

std::string Inductor::generateFields()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	sstrm <<
	"static "<<"real "<<appendName("epos_past")       <<" = "<<0.0<<";\n" <<
	"static "<<"real "<<appendName("eneg_past")       <<" = "<<0.0<<";\n" <<
	"static "<<"real "<<appendName("delta_v")         <<" = "<<0.0<<";\n" <<
	"static "<<"real "<<appendName("current")         <<" = "<<0.0<<";\n" <<
	"static "<<"real "<<appendName("current_eq")      <<" = "<<0.0<<";\n" <<
	"static "<<"real "<<appendName("current_eq_past") <<" = "<<0.0<<";\n" ;

	return sstrm.str();
}

std::string Inductor::generateOutputs(std::string output)
{
	if(output == "ALL" || output == "l_current")
	{
		codegen::Object l_current("real*", appendName("l_current"), "");
		return l_current.generateArgument();
	}
	else
	{
		return std::string("");
	}
}

std::string Inductor::generateOutputsUpdateBody(std::string output)
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	if(output == "ALL" || output == "l_current")
	{
		sstrm << appendName("*l_current") << " = " << appendName("current") << ";\n\n";
		return sstrm.str();
	}
	else
	{
		return std::string("");
	}
}

std::string Inductor::generateUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	//epos_past = epos;
	//eneg_past = eneg;
	//current_eq_past = current_eq;

    sstrm <<
    appendName("epos_past")<<" = "<<"x["<<P<<"]"<<";\n" <<
    appendName("eneg_past")<<" = "<<"x["<<N<<"]"<<";\n" <<
	appendName("current_eq_past")<<" = "<<appendName("current_eq")<<";\n" ;

	//delta_v = AddSubType(epos_past) - AddSubType(eneg_past);
	//current = hol2*delta_v - current_eq_past;
	//current_eq = -current - hol2*delta_v;
	//*bout = current_eq;

	sstrm <<
	appendName("delta_v")<<" = "<<appendName("epos_past")<<" - "<<appendName("eneg_past")<<";\n" <<
	appendName("current")<<" = "<<appendName("HOL2")<<"*"<<appendName("delta_v")<<" - "<<appendName("current_eq_past")<<";\n" <<
    appendName("current_eq")<<" = "<<"-"<<appendName("current")<<" - "<<appendName("HOL2")<<"*"<<appendName("delta_v")<<";\n" <<
    "b_components["<<source_id-1<<"]"<<" = "<<appendName("current_eq")<<";\n";

	return sstrm.str();
}

} //namespace lblmc
