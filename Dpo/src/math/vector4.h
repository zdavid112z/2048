#pragma once

#include "pch.h"
#include "Vector3.h"

	template <typename T, typename FP>
	struct Vector4
	{
		union
		{
			struct { Vector2<T, FP> xy, zw; };
			struct { Vector3<T, FP> xyz; };
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			struct { T data[4]; };
		};

		static Vector4<T, FP> Zero() { return Vector4<T, FP>((T)0, (T)0, (T)0, (T)0); }
		static Vector4<T, FP> One() { return Vector4<T, FP>((T)1, (T)1, (T)1, (T)1); }

		Vector4() : x(0), y(0), z(0), w(0) {}
		Vector4(const Vector2<T, FP>& a, const Vector2<T, FP>& b) : x(a.x), y(a.y), z(b.x), w(b.y) {}
		Vector4(const Vector3<T, FP>& a, const T& b = 0) : x(a.x), y(a.y), z(a.z), w(b) {}
		Vector4(const T& a) : x(a), y(a), z(a), w(a) {}
		Vector4(const T& xx, const T& yy, const T& zz, const T& ww) : x(xx), y(yy), z(zz), w(ww) {}
		//Vector4(const Vector4<T, FP>& a) : x(a.x), y(a.y), z(a.z), w(a.w) {}
		Vector4(const Vector4<T, FP>& a) = default;

		FP Dot(const Vector4<T, FP>& a) const { return (FP)x * (FP)a.x + (FP)y * (FP)a.y + (FP)z * (FP)a.z + (FP)w * (FP)a.w; }
		FP Length() const { return Sqrt<T>((FP)x * (FP)x + (FP)y * (FP)y + (FP)z * (FP)z + (FP)w * (FP)w); }
		FP LengthSquared() const { return (FP)x * (FP)x + (FP)y * (FP)y + (FP)z * (FP)z + (FP)w * (FP)w; }
		FP Distance(const Vector4<T, FP>& a) const { return Sqrt<T>( ((FP)x - (FP)a.x) * ((FP)x - (FP)a.x) + ((FP)y - (FP)a.y) * ((FP)y - (FP)a.y) + ((FP)z - (FP)a.z) * ((FP)z - (FP)a.z) + ((FP)w - (FP)a.w) * ((FP)w - (FP)a.w)); }
		FP DistanceSquared(const Vector4<T, FP>& a) const { return ((FP)x - (FP)a.x) * ((FP)x - (FP)a.x) + ((FP)y - (FP)a.y) * ((FP)y - (FP)a.y) + ((FP)z - (FP)a.z) * ((FP)z - (FP)a.z) + ((FP)w - (FP)a.w) * ((FP)w - (FP)a.w); }

		Vector4<T, FP> Normalized() const { FP l = Length(); return Vector4<T, FP>((T)((FP)x / l), (T)((FP)y / l), (T)((FP)z / l), (T)((FP)w / l)); }
		Vector4<T, FP> Added(const T& a) const { return Vector4<T, FP>(x + a, y + a, z + a, w + a); }
		Vector4<T, FP> Subtracted(const T& a) const { return Vector4<T, FP>(x - a, y - a, z - a, w - a); }
		Vector4<T, FP> Multiplied(const T& a) const { return Vector4<T, FP>(x * a, y * a, z * a, w * a); }
		Vector4<T, FP> Divided(const T& a) const { return Vector4<T, FP>(x / a, y / a, z / a, w / a); }
		Vector4<T, FP> Added(const Vector4<T, FP>& a) const { return Vector4<T, FP>(x + a.x, y + a.y, z + a.z, w + a.w); }
		Vector4<T, FP> Subtracted(const Vector4<T, FP>& a) const { return Vector4<T, FP>(x - a.x, y - a.y, z - a.z, w - a.w); }
		Vector4<T, FP> Multiplied(const Vector4<T, FP>& a) const { return Vector4<T, FP>(x * a.x, y * a.y, z * a.z, w * a.w); }
		Vector4<T, FP> Divided(const Vector4<T, FP>& a) const { return Vector4<T, FP>(x / a.x, y / a.y, z / a.z, w / a.w); }
		bool Equals(const Vector4<T, FP>& a) const { return x == a.x && y == a.y && z == a.z && w == a.w; }

		Vector4<T, FP> Lerp(const Vector4<T, FP>& a, FP v) const { return Multiplied((FP(1) - v)) + a * v; }
		Vector4<T, FP> Reflect(const Vector4<T, FP>& normal) const { return Subtracted((FP)2 * Dot(normal) * normal); }
		Vector4<T, FP> Refract(const Vector4<T, FP>& normal, const FP& eta) const 
		{
			T ndoti = Dot(normal);
			T k = (FP)1 - eta * eta * ((FP)1 - ndoti * ndoti);
			if (k < (FP)0)
				return Zero();
			return Multiplied(eta) - (eta * ndoti + Sqrt<T>(k)) * normal;
		}

		Vector4<T, FP>& Normalize() { FP l = Length(); x /= l; y /= l; z /= l; w /= l; return *this; }
		Vector4<T, FP>& Add(const T& a) { x += a; y += a; z += a; w += a; return *this; }
		Vector4<T, FP>& Subtract(const T& a) { x -= a; y -= a; z -= a; w -= a; return *this; }
		Vector4<T, FP>& Multiply(const T& a) { x *= a; y *= a; z *= a; w *= a; return *this; }
		Vector4<T, FP>& Divide(const T& a) { x /= a; y /= a; z /= a; w /= a; return *this; }
		Vector4<T, FP>& Add(const Vector4<T, FP>& a) { x += a.x; y += a.y; z += a.z; w += a.w; return *this; }
		Vector4<T, FP>& Subtract(const Vector4<T, FP>& a) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; return *this; }
		Vector4<T, FP>& Multiply(const Vector4<T, FP>& a) { x *= a.x; y *= a.y; z *= a.z; w *= a.w; return *this; }
		Vector4<T, FP>& Divide(const Vector4<T, FP>& a) { x /= a.x; y /= a.y; z /= a.z; w /= a.w; return *this; }

		Vector4<T, FP> operator+(const Vector4<T, FP>& a) const { return Added(a); }
		Vector4<T, FP> operator-(const Vector4<T, FP>& a) const { return Subtracted(a); }
		Vector4<T, FP> operator*(const Vector4<T, FP>& a) const { return Multiplied(a); }
		Vector4<T, FP> operator/(const Vector4<T, FP>& a) const { return Divided(a); }
		Vector4<T, FP> operator+(const T& a) const { return Added(a); }
		Vector4<T, FP> operator-(const T& a) const { return Subtracted(a); }
		Vector4<T, FP> operator*(const T& a) const { return Multiplied(a); }
		Vector4<T, FP> operator/(const T& a) const { return Divided(a); }
		Vector4<T, FP> operator-() const { return Vector4<T, FP>(-x, -y, -z, -w); }

		friend Vector4<T, FP> operator+(const T& a, const Vector4<T, FP>& t) { return t.Added(a); }
		friend Vector4<T, FP> operator-(const T& a, const Vector4<T, FP>& t) { return t.Subtracted(a); }
		friend Vector4<T, FP> operator*(const T& a, const Vector4<T, FP>& t) { return t.Multiplied(a); }
		friend Vector4<T, FP> operator/(const T& a, const Vector4<T, FP>& t) { return Vector4<T, FP>(a).Divided(t); }

		Vector4<T, FP>& operator+=(const Vector4<T, FP>& a) { return Add(a); }
		Vector4<T, FP>& operator-=(const Vector4<T, FP>& a) { return Subtract(a); }
		Vector4<T, FP>& operator*=(const Vector4<T, FP>& a) { return Multiply(a); }
		Vector4<T, FP>& operator/=(const Vector4<T, FP>& a) { return Divide(a); }
		Vector4<T, FP>& operator+=(const T& a) { return Add(a); }
		Vector4<T, FP>& operator-=(const T& a) { return Subtract(a); }
		Vector4<T, FP>& operator*=(const T& a) { return Multiply(a); }
		Vector4<T, FP>& operator/=(const T& a) { return Divide(a); }

		//Vector4<T, FP>& operator=(const Vector4<T, FP>& a) { x = a.x; y = a.y; z = a.z; w = a.w; return *this; }
		//Vector4<T, FP>& operator=(const T& a) { x = a; y = a; z = a; w = a; return *this; }

		bool operator==(const Vector4<T, FP>& a) const { return Equals(a); }
		bool operator!=(const Vector4<T, FP>& a) const { return !Equals(a); }

		T& operator[](const uint32& index) { return data[index]; }
		const T& operator[](const uint32& index) const { return data[index]; }
	};

	template <typename T, typename FP>
	static FP Dot(const Vector4<T, FP>& a, Vector4<T, FP>& b) { return a.Dot(b); }
	template <typename T, typename FP>
	static Vector4<T, FP> Normalize(const Vector4<T, FP>& a) { return a.Normalized(); }
	template <typename T, typename FP>
	static Vector4<T, FP> Lerp(const Vector4<T, FP>& a, const Vector4<T, FP>& b, const FP& c) { return a.Lerp(b, c); }
	template <typename T, typename FP>
	static Vector4<T, FP> Reflect(const Vector4<T, FP>& ray, const Vector4<T, FP>& normal) { return ray.Reflect(normal); }
	template <typename T, typename FP>
	static Vector4<T, FP> Refract(const Vector4<T, FP>& ray, const Vector4<T, FP>& normal, const FP& eta) { return ray.Refract(normal, eta); }
