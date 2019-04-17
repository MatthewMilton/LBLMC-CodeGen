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

#ifndef TBDATATYPES_HPP
#define TBDATATYPES_HPP

//#include "LBLMC/Params.hpp"

#ifdef LBLMC_XILINX_VIVADO_HLS
#include <ap_int.h>
#include <ap_fixed.h>
#include <hls_half.h>
#endif

#include <Eigen/Dense>

namespace lblmc
{

///////////////////////////////////////////////////////////////////////////////////////////////////
// Eigen3 Linear Algebra Library Typedefs (http://eigen.tuxfamily.org)

typedef Eigen::Matrix<double,
		Eigen::Dynamic,Eigen::Dynamic,
		Eigen::RowMajor>
MatrixRMXd; ///< Dynamically-allocated row-major double Eigen3 matrix type



///////////////////////////////////////////////////////////////////////////////////////////////////

} //namespace lblmc

#endif // TBDATATYPES_HPP
