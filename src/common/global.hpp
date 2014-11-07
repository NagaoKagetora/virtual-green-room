#pragma once
#ifndef _VGR_COMMON_GLOBAL_HPP
#define _VGR_COMMON_GLOBAL_HPP

#pragma warning (push)
#pragma warning (disable:4819)	/* contains characters that cannot be represented in the current code page */
#pragma warning (disable:4996)	/* function was declared deprecated */

#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <locale>

#pragma warning (pop)

#ifndef _MATH_DEFINES_DEFINED
#define _MATH_DEFINES_DEFINED
#  define M_E							2.71828182845904523536
#  define M_LOG2E						1.44269504088896340736
#  define M_LOG10E						0.434294481903251827651
#  define M_LN2							0.693147180559945309417
#  define M_LN10						2.30258509299404568402
#  define M_PI							3.14159265358979323846
#  define M_PI_2						1.57079632679489661923
#  define M_PI_4						0.785398163397448309616
#  define M_1_PI						0.318309886183790671538
#  define M_2_PI						0.636619772367581343076
#  define M_2_SQRTPI					1.12837916709551257390
#  define M_SQRT2						1.41421356237309504880
#  define M_SQRT1_2						0.707106781186547524401
#endif /* _MATH_DEFINES_DEFINED */

#define _VGR_BEGIN						namespace vgr {
#define _VGR_END						}

#define _VGR_LOCAL_DECL					static
#define _VGR_GLOBAL_DECL				extern
#define _VGR_INLINE_DECL				inline
#define _VGR_LOCAL						static
#define _VGR_GLOBAL
#define _VGR_INLINE						inline
#define _VGR_STATIC						static
#define _VGR_EXPLICIT					explicit



typedef float							float32_t;
typedef double							float64_t;
typedef signed char						int8_t;
typedef unsigned char					uint8_t;
typedef short							int16_t;
typedef unsigned short					uint16_t;
typedef int								int32_t;
typedef	unsigned int					uint32_t;
typedef long long						int64_t;
typedef	unsigned long long				uint64_t;
typedef double							real_t;
typedef int								int_t;



const float32_t							VGR_FLOAT32_MAX = FLT_MAX;
const float32_t							VGR_FLOAT32_MIN = FLT_MIN;
const float32_t							VGR_FLOAT32_EPS = FLT_EPSILON;
const float64_t							VGR_FLOAT64_MAX = DBL_MAX;
const float64_t							VGR_FLOAT64_MIN = DBL_MIN;
const float64_t							VGR_FLOAT64_EPS = DBL_EPSILON;
const int8_t							VGR_INT8_MAX = SCHAR_MAX;
const int8_t							VGR_INT8_MIN = SCHAR_MIN;
const int8_t							VGR_INT8_EPS = 0;
const uint8_t							VGR_UINT8_MAX = UCHAR_MAX;
const uint8_t							VGR_UINT8_MIN = 0;
const uint8_t							VGR_UINT8_EPS = 0;
const int16_t							VGR_INT16_MAX = SHRT_MAX;
const int16_t							VGR_INT16_MIN = SHRT_MIN;
const int16_t							VGR_INT16_EPS = 0;
const uint16_t							VGR_UINT16_MAX = USHRT_MAX;
const uint16_t							VGR_UINT16_MIN = 0;
const uint16_t							VGR_UINT16_EPS = 0;
const int32_t							VGR_INT32_MAX = INT_MAX;
const int32_t							VGR_INT32_MIN = INT_MIN;
const int32_t							VGR_INT32_EPS = 0;
const uint32_t							VGR_UINT32_MAX = UINT_MAX;
const uint32_t							VGR_UINT32_MIN = 0;
const uint32_t							VGR_UINT32_EPS = 0;
const int64_t							VGR_INT64_MAX = LLONG_MAX;
const int64_t							VGR_INT64_MIN = LLONG_MIN;
const int64_t							VGR_INT64_EPS = 0;
const uint64_t							VGR_UINT64_MAX = ULLONG_MAX;
const uint64_t							VGR_UINT64_MIN = 0;
const uint64_t							VGR_UINT64_EPS = 0;
const real_t							VGR_REAL_MAX = VGR_FLOAT64_MAX;
const real_t							VGR_REAL_MIN = VGR_FLOAT64_MIN;
const real_t							VGR_REAL_EPS = VGR_FLOAT64_EPS;
const int_t								VGR_INT_MAX = VGR_INT32_MAX;
const int_t								VGR_INT_MIN = VGR_INT32_MIN;
const int_t								VGR_INT_EPS = VGR_INT32_EPS;

const real_t							VGR_E = M_E;
const real_t							VGR_LBE = M_LOG2E;
const real_t							VGR_LGE = M_LOG10E;
const real_t							VGR_LN2 = M_LN2;
const real_t							VGR_LN10 = M_LN10;
const real_t							VGR_PI = M_PI;
const real_t							VGR_2PI = 2.0 * M_PI;
const real_t							VGR_PI2 = 0.5 * M_PI;
const real_t							VGR_DGR_PER_RAD = 180 / M_PI;
const real_t							VGR_RAD_PER_DGR = M_PI / 180;



_VGR_BEGIN



class Exception;



class Exception : public std::exception {

public:
	typedef std::exception				ExceptionBase;

public:
	inline Exception() : ExceptionBase() { }
	inline Exception(const Exception &exception) : ExceptionBase(exception) { }
	inline Exception(const ExceptionBase &exception) : ExceptionBase(exception) { }
	inline explicit Exception(const char *const &message) : ExceptionBase(message) { }
	inline explicit Exception(const std::string &message) : ExceptionBase(message.c_str()) { }

public:
	inline const char *Message() const { return (std::exception::what()); }
};



#ifndef _VGR_EXCEPTION_DEFINED
#define _VGR_EXCEPTION_DEFINED
#  define _VGR_EXCEPTION_SPEC_BASE(_E, _B)										\
		class _E : public _B {													\
																				\
		public:																	\
			inline _E() : _B() { }												\
			inline _E(const _E &exception) :									\
					_B(exception) { }											\
			inline _E(const _B &exception) :									\
					_B(exception) { }											\
			inline _E(const ExceptionBase &exception) :							\
					_B(exception) { }											\
			inline explicit _E(const char *const &message) :					\
					_B(message) { }												\
			inline explicit _E(const std::string &message) :					\
					_B(message) { }												\
		};
#  define _VGR_EXCEPTION(_E)			_VGR_EXCEPTION_SPEC_BASE(_E, vgr::Exception)
#endif /*_VGR_EXCEPTION_DEFINED */



_VGR_END



#endif /* _VGR_COMMON_GLOBAL_HPP */
