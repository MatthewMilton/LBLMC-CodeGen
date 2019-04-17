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

#include "Component.hpp"
#include "../SystemConductanceGenerator.hpp"
#include "../SystemSolverGenerator.hpp"
#include "../SimulationEngineGenerator.hpp"

namespace lblmc
{

void Component::stampSystem(SimulationEngineGenerator& gen, std::vector<std::string> outputs)
{
	std::string buf;
	SystemConductanceGenerator& scg = gen.getConductanceGenerator();
	SystemSourceVectorGenerator& ssvg = gen.getSourceVectorGenerator();

	stampConductance(scg);
	stampSources(ssvg);
	buf = generateParameters();
	gen.insertComponentParametersCode(buf);

	buf = generateFields();
	gen.insertComponentFieldsCode(buf);

	buf = generateInputs();
	gen.insertComponentInputsCode(buf);

	for(auto output : outputs)
	{
		buf = generateOutputs(output);
		gen.insertComponentOutputsCode(buf);

		buf = generateOutputsUpdateBody(output);
		gen.insertComponentOutputsUpdateBody(buf);
	}

	buf = generateUpdateBody();
	gen.insertComponentUpdateBody(buf);
}

} //namespace lblmc
