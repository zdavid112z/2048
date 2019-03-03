#pragma once

#include "pch.h"
#include "Vector2.h"

	template <typename T, typename FP>
	struct Vector3
	{
		union
		{
			Vector2<T, FP> xy;
			struct { T x, y, z; };
			struct { T r, g, b; };
			struct { T data[3]; };
		};

		static Vector3<T, FP> Up() { return Vector3<T, FP>((T)0, (T)1, (T)0); }
		static Vector3<T, FP> Right() { return Vector3<T, FP>((T)1, (T)0, (T)0); }
		static Vector3<T, FP> Forward() { return Vector3<T, FP>((T)0, (T)0, (T)1); }
		static Vector3<T, FP> Zero() { return Vector3<T, FP>((T)0, (T)0, (T)0); }
		static Vector3<T, FP> One() { return Vector3<T, FP>((T)1, (T)1, (T)1); }

		Vector3() : x(0), y(0), z(0) {}
		Vector3(const Vector2<T, FP>& a, const T& b = 0) : x(a.x), y(a.y), z(b) {}
		Vector3(const T& a) : x(a), y(a), z(a) {}
		Vector3(const T& xx, const T& yy, const T& zz) : x(xx), y(yy), z(zz) {}
		//Vector3(const Vector3<T, FP>& v) : x(v.x), y(v.y), z(v.z) {}
		Vector3(const Vector3<T, FP>& v) = default;

		Vector3<T, FP> Cross(const Vector3<T, FP>& a) const { return Vector3<T, FP>(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x); };
		FP Dot(const Vector3<T, FP>& a) const { return (FP)x * (FP)a.x + (FP)y * (FP)a.y + (FP)z * (FP)a.z; }
		FP Length() const { return Sqrt<T>((FP)x * (FP)x + (FP)y * (FP)y + (FP)z * (FP)z); }
		FP LengthSquared() const { return (FP)x * (FP)x + (FP)y * (FP)y + (FP)z * (FP)z; }
		FP Distance(const Vector3<T, FP>& a) const { return Sqrt<T>( ((FP)x - (FP)a.x) * ((FP)x - (FP)a.x) + ((FP)y - (FP)a.y) * ((FP)y - (FP)a.y) + ((FP)z - (FP)a.z) * ((FP)z - (FP)a.z)); }
		FP DistanceSquared(const Vector3<T, FP>& a) const { return ((FP)x - (FP)a.x) * ((FP)x - (FP)a.x) + ((FP)y - (FP)a.y) * ((FP)y - (FP)a.y) + ((FP)z - (FP)a.z) * ((FP)z - (FP)a.z); }

		Vector3<T, FP> Normalized() const { FP l = Length(); return Vector3<T, FP>((T)((FP)x / l), (T)((FP)y / l), (T)((FP)z / l)); }
		Vector3<T, FP> Added(const T& a) const { return Vector3<T, FP>(x + a, y + a, z + a); }
		Vector3<T, FP> Subtracted(const T& a) const { return Vector3<T, FP>(x - a, y - a, z - a); }
		Vector3<T, FP> Multiplied(const T& a) const { return Vector3<T, FP>(x * a, y * a, z * a); }
		Vector3<T, FP> Divided(const T& a) const { return Vector3<T, FP>(x / a, y / a, z / a); }
		Vector3<T, FP> Added(const Vector3<T, FP>& a) const { return Vector3<T, FP>(x + a.x, y + a.y, z + a.z); }
		Vector3<T, FP> Subtracted(const Vector3<T, FP>& a) const { return Vector3<T, FP>(x - a.x, y - a.y, z - a.z); }
		Vector3<T, FP> Multiplied(const Vector3<T, FP>& a) const { return Vector3<T, FP>(x * a.x, y * a.y, z * a.z); }
		Vector3<T, FP> Divided(const Vector3<T, FP>& a) const { return Vector3<T, FP>(x / a.x, y / a.y, z / a.z); }
		bool Equals(const Vector3<T, FP>& a) const { return x == a.x && y == a.y && z == a.z; }

		Vector3<T, FP> Lerp(const Vector3<T, FP>& a, FP v) const { return Multiplied(FP(1) - v) + a * (v); }
		Vector3<T, FP> Reflect(const Vector3<T, FP>& normal) const { return Subtracted((FP)2 * Dot(normal) * normal); }
		Vector3<T, FP> Refract(const Vector3<T, FP>& normal, const FP& eta) const 
		{
			T ndoti = Dot(normal);
			T k = (FP)1 - eta * eta * ((FP)1 - ndoti * ndoti);
			if (k < (FP)0)
				return Zero();
			return Multiplied(eta) - (eta * ndoti + Sqrt<T>(k)) * normal;
		}

		Vector3<T, FP>& Normalize() { FP l = Length(); x /= l; y /= l; z /= l; return *this; }
		Vector3<T, FP>& Add(const T& a) { x += a; y += a; z += a; return *this; }
		Vector3<T, FP>& Subtract(const T& a) { x -= a; y -= a; z -= a; return *this; }
		Vector3<T, FP>& Multiply(const T& a) { x *= a; y *= a; z *= a; return *this; }
		Vector3<T, FP>& Divide(const T& a) { x /= a; y /= a; z /= a; return *this; }
		Vector3<T, FP>& Add(const Vector3<T, FP>& a) { x += a.x; y += a.y; z += a.z; return *this; }
		Vector3<T, FP>& Subtract(const Vector3<T, FP>& a) { x -= a.x; y -= a.y; z -= a.z; return *this; }
		Vector3<T, FP>& Multiply(const Vector3<T, FP>& a) { x *= a.x; y *= a.y; z *= a.z; return *this; }
		Vector3<T, FP>& Divide(const Vector3<T, FP>& a) { x /= a.x; y /= a.y; z /= a.z; return *this; }

		Vector3<T, FP> operator+(const Vector3<T, FP>& a) const { return Added(a); }
		Vector3<T, FP> operator-(const Vector3<T, FP>& a) const { return Subtracted(a); }
		Vector3<T, FP> operator*(const Vector3<T, FP>& a) const { return Multiplied(a); }
		Vector3<T, FP> operator/(const Vector3<T, FP>& a) const { return Divided(a); }
		Vector3<T, FP> operator+(const T& a) const { return Added(a); }
		Vector3<T, FP> operator-(const T& a) const { return Subtracted(a); }
		Vector3<T, FP> operator*(const T& a) const { return Multiplied(a); }
		Vector3<T, FP> operator/(const T& a) const { return Divided(a); }
		Vector3<T, FP> operator-() const { return Vector3<T, FP>(-x, -y, -z); }

		friend Vector3<T, FP> operator+(const T& a, const Vector3<T, FP>& t) { return t.Added(a); }
		friend Vector3<T, FP> operator-(const T& a, const Vector3<T, FP>& t) { return t.Subtracted(a); }
		friend Vector3<T, FP> operator*(const T& a, const Vector3<T, FP>& t) { return t.Multiplied(a); }
		friend Vector3<T, FP> operator/(const T& a, const Vector3<T, FP>& t) { return Vector3<T, FP>(a).Divided(t); }

		Vector3<T, FP>& operator+=(const Vector3<T, FP>& a) { return Add(a); }
		Vector3<T, FP>& operator-=(const Vector3<T, FP>& a) { return Subtract(a); }
		Vector3<T, FP>& operator*=(const Vector3<T, FP>& a) { return Multiply(a); }
		Vector3<T, FP>& operator/=(const Vector3<T, FP>& a) { return Divide(a); }
		Vector3<T, FP>& operator+=(const T& a) { return Add(a); }
		Vector3<T, FP>& operator-=(const T& a) { return Subtract(a); }
		Vector3<T, FP>& operator*=(const T& a) { return Multiply(a); }
		Vector3<T, FP>& operator/=(const T& a) { return Divide(a); }

		//Vector3<T, FP>& operator=(const Vector3<T, FP>& a) { x = a.x; y = a.y; z = a.z; return *this; }
		//Vector3<T, FP>& operator=(const T& a) { x = a; y = a; z = a; return *this; }

		bool operator==(const Vector3<T, FP>& a) const { return Equals(a); }
		bool operator!=(const Vector3<T, FP>& a) const { return !Equals(a); }

		T& operator[](const uint32& index) { return data[index]; }
		const T& operator[](const uint32& index) const { return data[index]; }
	};

	template <typename T, typename FP>
	static FP Dot(const Vector3<T, FP>& a, Vector3<T, FP>& b) { return a.Dot(b); }

	template <typename T, typename FP>
	static Vector3<T, FP> Cross(const Vector3<T, FP>& a, const Vector3<T, FP>& b) { return a.Cross(b); }

	template <typename T, typename FP>
	static Vector3<T, FP> Normalize(const Vector3<T, FP>& a) { return a.Normalized(); }
	template <typename T, typename FP>
	static Vector3<T, FP> Lerp(const Vector3<T, FP>& a, const Vector3<T, FP>& b, const FP& c) { return a.Lerp(b, c); }
	template <typename T, typename FP>
	static Vector3<T, FP> Reflect(const Vector3<T, FP>& ray, const Vector3<T, FP>& normal) { return ray.Reflect(normal); }
	template <typename T, typename FP>
	static Vector3<T, FP> Refract(const Vector3<T, FP>& ray, const Vector3<T, FP>& normal, const FP& eta) { return ray.Refract(normal, eta); }

