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

#ifndef LBLMC_HALFBRIDGECONVERTER3PHASE2_HPP
#define LBLMC_HALFBRIDGECONVERTER3PHASE2_HPP

#include <string>
#include <vector>

#include "Component.hpp"

namespace lblmc
{

/**
	\brief Component code generator for Half-Bridge 3-Phase Switching Power Electronic Converter

	The component expects a bipolar DC side and a 3-phase DC or AC side.  The DC side contains
	2 capacitors for bipolar configuration.  The 3-phase side contains series inductors per phase.
	The half-bridge switching elements are ideal switches.

	This component differs from HalfBridgeConverter3Phase by having a sixth terminal that is
	connected to center point between the DC capacitors while the other component assumes this
	point is connected to common (0V).

**/
class HalfBridgeConverter3Phase2 : public Component
{

private:

	double DT;
	double CAP;
	double IND;
	double RES;
	unsigned int P, G, N, A, B, C;
	unsigned int source_id_P, source_id_N, source_id_A, source_id_B, source_id_C;

	constexpr static double CAP_CONDUCTANCE = 10000.0;
	constexpr static double IND_CONDUCTANCE = 0.0;

public:

	HalfBridgeConverter3Phase2(std::string comp_name);
	HalfBridgeConverter3Phase2
	(
		std::string comp_name,
		double dt,
		double cap,
		double ind,
		double res
	);
	HalfBridgeConverter3Phase2(const HalfBridgeConverter3Phase2& base);

	inline unsigned int getNumberOfTerminals() const { return 6; }
	inline unsigned int getNumberOfSources() const { return 5; }
	void getSourceIds(std::vector<unsigned int>& ids) const;
	inline void setTerminalConnections
	(
		unsigned int p,
		unsigned int g,
		unsigned int n,
		unsigned int a,
		unsigned int b,
		unsigned int c
	) { P = p; G = g; N = n; A = a; B = b; C = c; }

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

