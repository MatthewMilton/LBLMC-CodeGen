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

#ifndef LBLMC_HALFBRIDGECONVERTER3PHASE_HPP
#define LBLMC_HALFBRIDGECONVERTER3PHASE_HPP

#include <string>
#include <vector>

#include "Component.hpp"

namespace lblmc
{

class HalfBridgeConverter3Phase : public Component
{

private:

	double DT;
	double CAP;
	double IND;
	double RES;
	unsigned int P, N, A, B, C;
	unsigned int source_id_P, source_id_N, source_id_A, source_id_B, source_id_C;

	constexpr static double CAP_CONDUCTANCE = 10000.0;
	constexpr static double IND_CONDUCTANCE = 0.0;

public:

	HalfBridgeConverter3Phase(std::string comp_name);
	HalfBridgeConverter3Phase
	(
		std::string comp_name,
		double dt,
		double cap,
		double ind,
		double res
	);
	HalfBridgeConverter3Phase(const HalfBridgeConverter3Phase& base);

	inline unsigned int getNumberOfTerminals() const { return 5; }
	inline unsigned int getNumberOfSources() const { return 5; }
	void getSourceIds(std::vector<unsigned int>& ids) const;
	inline void setTerminalConnections
	(
		unsigned int p,
		unsigned int n,
		unsigned int a,
		unsigned int b,
		unsigned int c
	) { P = p; N = n; A = a; B = b; C = c; }

	inline void setParameters
	(
		double dt,
		double cap,
		double ind,
		double res
	) { DT = dt; CAP = cap; IND = ind; RES = res; }
	inline const double& getDT() const { return DT; }
	inline const double& getCapacitance() const { return CAP; }
	inline const double& getInductance() const { return IND; }
	inline const double& getResistance() const { return RES; }

	inline void setIntegrationMethod(std::string method) {}
	inline std::string getIntegrationMethod() const { return std::string("euler_forward"); }

	inline std::vector<std::string> getSupportedOutputs() const
	{
		std::vector<std::string> ret
		{
			"cp_voltage",
			"cn_voltage",
			"la_current",
			"lb_current",
			"lc_current"
		};

		return ret;
	}

	void stampConductance(SystemConductanceGenerator& gen);
	void stampSources(SystemSourceVectorGenerator& gen);
	std::string generateParameters();
	std::string generateFields();
	std::string generateInputs();
	std::string generateOutputs(std::string output = "ALL");
	std::string generateOutputsUpdateBody(std::string output="ALL");
	std::string generateUpdateBody();

private:

	std::string generateCPVoltageOutputUpdateBody();
	std::string generateCNVoltageOutputUpdateBody();
	std::string generateLACurrentOutputUpdateBody();
	std::string generateLBCurrentOutputUpdateBody();
	std::string generateLCCurrentOutputUpdateBody();

};

} //namespace lblmc

#endif // LBLMC_HALFBRIDGECONVERTER3PHASE_HPP
