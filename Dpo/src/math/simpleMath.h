#pragma once

#include "pch.h"

template <typename T>
static T PI() { return (T)3.14159265358979323846264338327950288; }

template <typename T>
static T Epsilon() { return (T)2.71828182845904523536028747135266249775724709369995; }

template <typename T>
static T Sqrt(const T& a) { return (T)sqrt(a); }
template<>
static float Sqrt<float>(const float& a) { return sqrtf(a); }
template<>
static lfloat Sqrt<lfloat>(const lfloat& a) { return sqrtl(a); }

template <typename T>
static T Rsqrt(const T& a) { return T(1) / Sqrt<T>(a); }

template <typename T>
static T Abs(const T& a) { return abs(a); }
template<>
static float Abs(const float& a) { return fabsf(a); }
template<>
static double Abs(const double& a) { return fabs(a); }
template<>
static lfloat Abs(const lfloat& a) { return fabsl(a); }
template<>
static int32 Abs(const int32& a) { return labs(a); }
template<>
static int64 Abs(const int64& a) { return llabs(a); }

template <typename T>
static T Sin(const T& a) { return sin(a); }
template <>
static float Sin(const float& a) { return sinf(a); }
template <>
static lfloat Sin(const lfloat& a) { return sinl(a); }

template <typename T>
static T Cos(const T& a) { return cos(a); }
template <>
static float Cos(const float& a) { return cosf(a); }
template <>
static lfloat Cos(const lfloat& a) { return cosl(a); }

template <typename T>
static T Asin(const T& a) { return asin(a); }
template <>
static float Asin(const float& a) { return asinf(a); }
template <>
static lfloat Asin(const lfloat& a) { return asinl(a); }

template <typename T>
static T Acos(const T& a) { return acos(a); }
template <>
static float Acos(const float& a) { return acosf(a); }
template <>
static lfloat Acos(const lfloat& a) { return acosl(a); }

template <typename T>
static T Tan(const T& a) { return tan(a); }
template <>
static float Tan(const float& a) { return tanf(a); }
template <>
static lfloat Tan(const lfloat& a) { return tanl(a); }

template <typename T>
static T Atan(const T& a) { return atan(a); }
template <>
static float Atan(const float& a) { return atanf(a); }
template <>
static lfloat Atan(const lfloat& a) { return atanl(a); }

template <typename T>
static T Atan2(const T& a, const T& b) { return atan2(a, b); }
template <>
static float Atan2(const float& a, const float& b) { return atan2f(a, b); }
template <>
static lfloat Atan2(const lfloat& a, const lfloat& b) { return atan2l(a, b); }

template <typename T>
static T Ceil(const T& a) { return ceil(a); }
template <>
static float Ceil(const float& a) { return ceilf(a); }
template <>
static lfloat Ceil(const lfloat& a) { return ceill(a); }

template <typename T>
static T Round(const T& a) { return round(a); }
template <>
static float Round(const float& a) { return roundf(a); }
template <>
static lfloat Round(const lfloat& a) { return roundl(a); }

template <typename T>
static T Floor(const T& a) { return floor(a); }
template <>
static float Floor(const float& a) { return floorf(a); }
template <>
static lfloat Floor(const lfloat& a) { return floorl(a); }

template <typename T>
static T Clamp(const T& a, const T& b, const T& c) { return a < b ? b : ((a > c) ? c : a); }

template <typename T>
static T Clamp01(const T& a) { return Clamp<T>(a, 0, 1); }

template <typename T>
static T Exp(const T& a) { return exp(a); }
template <>
static float Exp(const float& a) { return expf(a); }
template <>
static lfloat Exp(const lfloat& a) { return expl(a); }

template <typename T>
static T Exp2(const T& a) { return exp2(a); }
template <>
static float Exp2(const float& a) { return exp2f(a); }
template <>
static lfloat Exp2(const lfloat& a) { return exp2l(a); }

template <typename T, typename P>
static T Pow(const T& a, const P& b) { return pow(a, b); }
template <>
static float Pow(const float& a, const float& b) { return powf(a, b); }
template <>
static lfloat Pow(const lfloat& a, const lfloat& b) { return powl(a, b); }
template <typename T>
static T Pow(const T& a, const int32& b)
{
	T x = a, y = 1;
	int32 n = b;
	if (n < 0)
	{
		x = T(1.0) / x;
		n = -n;
	}
	if (n == 0) return 1;
	while (n > 1)
	{
		if (n % 2 == 0)
		{
			x *= x;
			n /= 2;
		}
		else
		{
			y *= x;
			x *= x;
			n = (n - 1) / 2;
		}
	}
	return x * y;
}
template <typename T>
static T Pow(const T& a, const int16& b) { return Pow<T, int32>(a, b); }
template <typename T>
static T Pow(const T& a, const int8& b) { return Pow<T, int32>(a, b); }

template <typename T>
static T Lerp(const T& a, const T& b, const T& t) { return b * t + a * (T(1) - t); }

template <typename T>
static T Min(const T& a, const T& b) { return a < b ? a : b; }
template <typename T>
static T Min(const T& a, const T& b, const T& c) { return Min<T>(a, Min<T>(b, c)); }
template <typename T>
static T Min(const T& a, const T& b, const T& c, const T& d) { return Min<T>(Min<T>(a, b), Min<T>(c, d)); }

template <typename T>
static T Max(const T& a, const T& b) { return a > b ? a : b; }
template <typename T>
static T Max(const T& a, const T& b, const T& c) { return Max<T>(a, Max<T>(b, c)); }
template <typename T>
static T Max(const T& a, const T& b, const T& c, const T& d) { return Max<T>(Max<T>(a, b), Max<T>(c, d)); }

template <typename T>
static T Sign(const T& a) { return a < 0 ? T(-1) : T(1); }

template <typename T>
static T Log(const T& a) { return log(a); }
template <>
static float Log(const float& a) { return logf(a); }
template <>
static lfloat Log(const lfloat& a) { return logl(a); }

template <typename T>
static T Log10(const T& a) { return log10(a); }
template <>
static float Log10(const float& a) { return log10f(a); }
template <>
static lfloat Log10(const lfloat& a) { return log10l(a); }

template <typename T>
static T Log2(const T& a) { return log2(a); }
template <>
static float Log2(const float& a) { return log2f(a); }
template <>
static lfloat Log2(const lfloat& a) { return log2l(a); }

template <typename T>
static T LogBase(const T& a, const T& base) { return Log<T>(a) / Log<T>(base); }

template <typename T>
static T Mix(const T& x, const T& y, const T& a) { return (x + a * (y - x)); }

