#pragma once
#ifndef _VGR_COMMON_ALGO_REGISTER_HPP
#define _VGR_COMMON_ALGO_REGISTER_HPP

#include <common/global.hpp>
#include <common/3rdparty/boost.hpp>
#include <common/3rdparty/eigen.hpp>
#include <common/3rdparty/vcglib.hpp>
#include <common/algo/algo.hpp>
#include <common/algo/util.hpp>



_VGR_BEGIN
_VGR_ALGO_BEGIN



class Register;



class Register {

public:
	static bool rotationMatrix(const eigen::Matrix3xN &src, const eigen::Matrix3xN &dst, eigen::Matrix3 *rotation);
	static void rotationMatrix2(const eigen::Matrix3xN &src, const eigen::Matrix3xN &dst, eigen::Matrix3 *rotation);
	static bool transformation(const eigen::Matrix3xN &src, const eigen::Matrix3xN &dst, eigen::Matrix3 *rotation, eigen::Vector3 *translation);
	static void transformation2(const eigen::Matrix3xN &src, const eigen::Matrix3xN &dst, eigen::Matrix3 *rotation, eigen::Vector3 *translation);
};



_VGR_ALGO_END
_VGR_END



#endif /* _VGR_COMMON_ALGO_REGISTER_HPP */
