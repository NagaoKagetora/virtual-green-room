#pragma once
#ifndef _VGR_COMMON_3RDPARTY_EIGEN_HPP
#define _VGR_COMMON_3RDPARTY_EIGEN_HPP

#include <common/global.hpp>
#include <common/3rdparty/3rdparty.hpp>
#include <eigen/Eigen/Dense>

#define _VGR_EIGEN						vgr::eigen
#define _VGR_EIGEN_BEGIN				namespace eigen {
#define _VGR_EIGEN_END					}



_VGR_BEGIN
_VGR_EIGEN_BEGIN



namespace src = Eigen;



using src::Dynamic;
using src::Infinity;
using src::DecompositionOptions;
using src::JacobiSVD;
using src::HouseholderQR;
using src::EigenSolver;

typedef src::Matrix<real_t, 3, 1>				Vector3;
typedef src::Matrix<real_t, 3, 3>				Matrix3;
typedef src::Matrix<real_t, Dynamic, 1>			Vector;
typedef src::Matrix<real_t, Dynamic, Dynamic>	Matrix;
typedef src::Matrix<real_t, 3, Dynamic>			Matrix3xN;
typedef src::Matrix<real_t, Dynamic, 3>			MatrixNx3;
typedef src::DiagonalMatrix<real_t, Dynamic>	Diagonal;



_VGR_EIGEN_END
_VGR_END



#endif /* _VGR_COMMON_3RDPARTY_EIGEN_HPP */
