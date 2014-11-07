#pragma once
#ifndef _VGR_COMMON_ALGO_UTIL_HPP
#define _VGR_COMMON_ALGO_UTIL_HPP

#include <common/global.hpp>
#include <common/3rdparty/eigen.hpp>
#include <common/3rdparty/vcglib.hpp>
#include <common/algo/algo.hpp>



_VGR_BEGIN
_VGR_ALGO_BEGIN



class Util;



class Util {

private:
	static real_t						Epsilon;

public:
	inline static real_t epsilon() { return (Util::Epsilon); }
	inline static void setEpsilon(real_t value) { Util::Epsilon = value; }

public:
	inline static bool zero(real_t v, real_t eps = Util::Epsilon) { return (std::abs(v) < eps); }
	inline static bool equal(real_t a, real_t b, real_t eps = Util::Epsilon) { return (Util::zero(a - b, eps)); }
	inline static bool greater(real_t a, real_t b, real_t eps = Util::Epsilon) { return (!Util::zero(a - b, eps) && (a > b)); }
	inline static bool less(real_t a, real_t b, real_t eps = Util::Epsilon) { return (!Util::zero(a - b, eps) && (a < b)); }

public:
	template <typename T>
	inline static real_t dot(const T &a, const T &b) {
		return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
	}
	template <typename T>
	inline static T cross(const T &a, const T &b) {
		return (T(
				a[1] * b[2] - a[2] * b[1],
				a[2] * b[0] - a[0] * b[2],
				a[0] * b[1] - a[1] * b[0]));
	}
	template <typename T>
	inline static real_t norm(const T &v) {
		return (std::sqrt(Util::dot(v, v)));
	}
	template <typename T>
	inline static real_t length(const T &v) {
		return (std::sqrt(Util::dot(v, v)));
	}
	template <typename T>
	inline static T normalized(const T &v) {
		real_t d = 1.0 / Util::length(v);
		return (T(v[0] * d, v[1] * d, v[2] * d));
	}
	template <typename T>
	inline static real_t cot(const T &a, const T &b) {
		real_t s2 = Util::dot(a, a) * Util::dot(b, b);
		real_t t = Util::dot(a, b);
		real_t cos2 = t * t / s2;
		return ((((t > 0) << 1) - 1) * std::sqrt(cos2 / (1 - cos2)));
	}
	template <typename T>
	inline static real_t tan(const T &a, const T &b) {
		real_t s2 = Util::dot(a, a) * Util::dot(b, b);
		real_t t = Util::dot(a, b);
		real_t cos2 = t * t / s2;
		return ((((t > 0) << 1) - 1) * std::sqrt((1 - cos2) / cos2));
	}
	template <typename T>
	inline static real_t cos(const T &a, const T &b) {
		real_t s2 = Util::dot(a, a) * Util::dot(b, b);
		real_t t = Util::dot(a, b);
		return (t / std::sqrt(s2));
	}
	template <typename T>
	inline static real_t sin(const T &a, const T &b) {
		real_t s2 = Util::dot(a, a) * Util::dot(b, b);
		real_t t = Util::dot(a, b);
		return (std::sqrt((1 - t * t) / s2));
	}

public:
	template <typename T>
	inline static eigen::Vector3 toVector(const T &src) {
		return (eigen::Vector3(
				eigen::Vector3::Scalar(src[0]),
				eigen::Vector3::Scalar(src[1]),
				eigen::Vector3::Scalar(src[2])));
	}
	template <typename T>
	inline static pcvcg::Point3 toPoint(const T &src) {
		return (pcvcg::Point3(
				pcvcg::Point3::ScalarType(src[0]),
				pcvcg::Point3::ScalarType(src[1]),
				pcvcg::Point3::ScalarType(src[2])));
	}
	template <typename T, typename S>
	inline static T cast(const S &src) {
		return (T(src[0], src[1], src[2]));
	}

public:
	template <typename T>
	inline static eigen::Matrix3 coordinate(const T &p0, const T &p1, const T &p2) {
		eigen::Vector3 v1 = Util::toVector(p1 - p0);
		eigen::Vector3 v2 = Util::toVector(p2 - p0);
		if (Util::zero(v1.cross(v2).norm())) return (eigen::Matrix3::Identity());
		eigen::Matrix3 matrix;
		matrix.col(0) = v1.normalized();
		matrix.col(1) = (v2 - v2.dot(matrix.col(0)) * matrix.col(0)).normalized();
		matrix.col(2) = matrix.col(0).cross(matrix.col(1));
		return (matrix);
	}
	template <typename T>
	inline static eigen::Matrix3 rotation(const T &n0, const T &n1) {
		eigen::Vector3 v0 = Util::toVector(n0);
		eigen::Vector3 v1 = Util::toVector(n1);
		real_t a = v0.dot(v1) / (v0.norm() * v1.norm());
		real_t b = std::sqrt(1 - a * a);
		real_t c = 1 - a;
		if (Util::zero(c)) return (eigen::Matrix3::Identity());
		eigen::Vector3 n = v0.cross(v1).normalized();
		eigen::Matrix3 matrix;
		matrix(0, 0) = n[0] * n[0] * c + a;
		matrix(0, 1) = n[0] * n[1] * c - n[2] * b;
		matrix(0, 2) = n[0] * n[2] * c + n[1] * b;
		matrix(1, 0) = n[1] * n[0] * c + n[2] * b;
		matrix(1, 1) = n[1] * n[1] * c + a;
		matrix(1, 2) = n[1] * n[2] * c - n[0] * b;
		matrix(2, 0) = n[2] * n[0] * c - n[1] * b;
		matrix(2, 1) = n[2] * n[1] * c + n[0] * b;
		matrix(2, 2) = n[2] * n[2] * c + a;
		return (matrix);
	}

public:
	static void toMatrix(const pcvcg::TriMesh &mesh, eigen::Matrix3xN *matrix);
	static void toMatrix(const pcvcg::TriMesh &mesh, const std::vector<size_t> &indices, eigen::Matrix3xN *matrix);
	static void toMesh(const eigen::Matrix3xN &matrix, pcvcg::TriMesh *mesh);
	static void oneRing(const pcvcg::TriMesh &mesh, size_t index, std::vector<size_t> *neighbors);
};



_VGR_ALGO_END
_VGR_END



#endif /* _VGR_COMMON_ALGO_UTIL_HPP */
