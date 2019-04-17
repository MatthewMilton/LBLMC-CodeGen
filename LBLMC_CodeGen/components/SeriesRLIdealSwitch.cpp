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

#include "SeriesRLIdealSwitch.hpp"
#include "../SystemConductanceGenerator.hpp"
#include "../SystemSourceVectorGenerator.hpp"
#include "../SimulationEngineGenerator.hpp"
#include "../codegen/Object.hpp"
#include "../codegen/ArrayObject.hpp"
#include "../codegen/StringProcessor.hpp"
#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace lblmc
{

SeriesRLIdealSwitch::SeriesRLIdealSwitch(std::string comp_name) :
	Component(comp_name),
	DT(1.0),
	L(1.0),
	R(1.0),
	P(0), N(0),
	source_id(0)
{}

SeriesRLIdealSwitch::SeriesRLIdealSwitch(std::string comp_name, double dt, double l, double r) :
	Component(comp_name),
	DT(dt),
	L(l),
	R(r),
	P(0), N(0),
	source_id(0)
{}

SeriesRLIdealSwitch::SeriesRLIdealSwitch(const SeriesRLIdealSwitch& base) :
	Component(base),
	DT(base.DT),
	L(base.L),
	R(base.R),
	P(base.P), N(base.N),
	source_id(base.source_id)
{}

void SeriesRLIdealSwitch::stampConductance(SystemConductanceGenerator& gen)
{
	//do nothing since using explicit integration method
}

void SeriesRLIdealSwitch::stampSources(SystemSourceVectorGenerator& gen)
{
	source_id = gen.insertSource(P,N);
}

std::string SeriesRLIdealSwitch::generateParameters()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	const static double HOL = DT/L;

	generateParameter(sstrm, "DT", DT);
	generateParameter(sstrm, "L", L);
	generateParameter(sstrm, "R", R);
	generateParameter(sstrm, "HOL", HOL);

	return sstrm.str();
}

std::string SeriesRLIdealSwitch::generateFields()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	generateField(sstrm, "current_past", 0.0);
	generateBoolField(sstrm, "sw_past", false);

	return sstrm.str();
}

std::string SeriesRLIdealSwitch::generateInputs()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	//append name to input ports
	codegen::Object sw("bool", appendName("sw"), "");

	sstrm <<
	sw.generateArgument();

	//sstrm << "\n";

	return sstrm.str();
}

const static std::string SERIESRLIDEALSWITCH_GENERATEUPDATEBODY_BASE_STRING =
R"(
NumType current;

if(sw_past)
{
	current = current_past + HOL*(epos - R*current_past - eneg);
}
else
{
	current = 0; //force de-energizing of inductor to zero when switch open
}

current_past = current;
sw_past = sw;

*bout = -current;
)";


std::string SeriesRLIdealSwitch::generateUpdateBody()
{
	std::stringstream sstrm;
	sstrm <<
	std::setprecision(16) <<
	std::fixed <<
	std::scientific;

	std::string body = SERIESRLIDEALSWITCH_GENERATEUPDATEBODY_BASE_STRING;
	codegen::StringProcessor str_proc(body);

	str_proc.replaceWordAll("NumType", "real");

	str_proc.replaceWordAll("HOL", appendName("HOL"));
	str_proc.replaceWordAll("R", appendName("R"));
	str_proc.replaceWordAll("L", appendName("L"));
	str_proc.replaceWordAll("DT", appendName("DT"));

	str_proc.replaceWordAll("sw_past", appendName("sw_past"));
	str_proc.replaceWordAll("current", appendName("current"));
	str_proc.replaceWordAll("current_past", appendName("current_past"));
	str_proc.replaceWordAll("sw", appendName("sw"));

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<P<<"]";
	str_proc.replaceWordAll("epos", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "x["<<N<<"]";
	str_proc.replaceWordAll("eneg", sstrm.str());

	sstrm.str("");
	sstrm.clear();
	sstrm << "b_components["<<source_id-1<<"]";
	str_proc.replaceWordAll("*bout", sstrm.str());

	return body;
}

} //namespace lblmc
