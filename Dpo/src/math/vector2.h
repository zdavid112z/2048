#pragma once

#include "pch.h"
#include "simpleMath.h"

template <class T, class FP>
struct Vector2
{
	union
	{
		struct { T x, y; };
		struct { T r, g; };
		struct { T s, t; };
		struct { T data[2]; };
	};

	static Vector2<T, FP> Up() { return Vector2<T, FP>((T)0, (T)1); }
	static Vector2<T, FP> Right() { return Vector2<T, FP>((T)1, (T)0); }
	static Vector2<T, FP> Zero() { return Vector2<T, FP>((T)0, (T)0); }
	static Vector2<T, FP> One() { return Vector2<T, FP>((T)1, (T)1); }

	Vector2() : x(0), y(0) {}
	Vector2(const T& a) : x(a), y(a) {}
	Vector2(const T& xx, const T& yy) : x(xx), y(yy) {}
	//Vector2(const Vector2<T, FP>& v) : x(v.x), y(v.y) {}
	Vector2(const Vector2<T, FP>& v) = default;

	FP Dot(const Vector2<T, FP>& a) const { return (FP)x * (FP)a.x + (FP)y * (FP)a.y; }
	FP Length() const { return Sqrt<T>((FP)x * (FP)x + (FP)y * (FP)y); }
	FP LengthSquared() const { return (FP)x * (FP)x + (FP)y * (FP)y; }
	FP Distance(const Vector2<T, FP>& a) const { return Sqrt<T>( ((FP)x - (FP)a.x) * ((FP)x - (FP)a.x) + ((FP)y - (FP)a.y) * ((FP)y - (FP)a.y)); }
	FP DistanceSquared(const Vector2<T, FP>& a) const { return ((FP)x - (FP)a.x) * ((FP)x - (FP)a.x) + ((FP)y - (FP)a.y) * ((FP)y - (FP)a.y); }

	Vector2<T, FP> Normalized() const { FP l = Length(); return Vector2<T, FP>((T)((FP)x / l), (T)((FP)y / l)); }
	Vector2<T, FP> Added(const T& a) const { return Vector2<T, FP>(x + a, y + a); }
	Vector2<T, FP> Subtracted(const T& a) const { return Vector2<T, FP>(x - a, y - a); }
	Vector2<T, FP> Multiplied(const T& a) const { return Vector2<T, FP>(x * a, y * a); }
	Vector2<T, FP> Divided(const T& a) const { return Vector2<T, FP>(x / a, y / a); }
	Vector2<T, FP> Added(const Vector2<T, FP>& a) const { return Vector2<T, FP>(x + a.x, y + a.y); }
	Vector2<T, FP> Subtracted(const Vector2<T, FP>& a) const { return Vector2<T, FP>(x - a.x, y - a.y); }
	Vector2<T, FP> Multiplied(const Vector2<T, FP>& a) const { return Vector2<T, FP>(x * a.x, y * a.y); }
	Vector2<T, FP> Divided(const Vector2<T, FP>& a) const { return Vector2<T, FP>(x / a.x, y / a.y); }
	bool Equals(const Vector2<T, FP>& a) const { return x == a.x && y == a.y; }

	Vector2<T, FP> Lerp(const Vector2<T, FP>& a, FP v) const { return Multiplied(FP(1) - v) + a * (v); }
	Vector2<T, FP> Reflect(const Vector2<T, FP>& normal) const { return Subtracted((FP)2 * Dot(normal) * normal); }
	Vector2<T, FP> Refract(const Vector2<T, FP>& normal, const FP& eta) const 
	{
		T ndoti = Dot(normal);
		T k = (FP)1 - eta * eta * ((FP)1 - ndoti * ndoti);
		if (k < (FP)0)
			return Zero();
		return Multiplied(eta) - (eta * ndoti + Sqrt<T>(k)) * normal;
	}

	Vector2<T, FP>& Normalize() { FP l = Length(); x /= l; y /= l; return *this; }
	Vector2<T, FP>& Add(const T& a) { x += a; y += a; return *this; }
	Vector2<T, FP>& Subtract(const T& a) { x -= a; y -= a; return *this; }
	Vector2<T, FP>& Multiply(const T& a) { x *= a; y *= a; return *this; }
	Vector2<T, FP>& Divide(const T& a) { x /= a; y /= a; return *this; }
	Vector2<T, FP>& Add(const Vector2<T, FP>& a) { x += a.x; y += a.y; return *this; }
	Vector2<T, FP>& Subtract(const Vector2<T, FP>& a) { x -= a.x; y -= a.y; return *this; }
	Vector2<T, FP>& Multiply(const Vector2<T, FP>& a) { x *= a.x; y *= a.y; return *this; }
	Vector2<T, FP>& Divide(const Vector2<T, FP>& a) { x /= a.x; y /= a.y; return *this; }

	Vector2<T, FP> operator+(const Vector2<T, FP>& a) const { return Added(a); }
	Vector2<T, FP> operator-(const Vector2<T, FP>& a) const { return Subtracted(a); }
	Vector2<T, FP> operator*(const Vector2<T, FP>& a) const { return Multiplied(a); }
	Vector2<T, FP> operator/(const Vector2<T, FP>& a) const { return Divided(a); }
	Vector2<T, FP> operator+(const T& a) const { return Added(a); }
	Vector2<T, FP> operator-(const T& a) const { return Subtracted(a); }
	Vector2<T, FP> operator*(const T& a) const { return Multiplied(a); }
	Vector2<T, FP> operator/(const T& a) const { return Divided(a); }

	Vector2<T, FP> operator-() const { return Vector2<T, FP>(-x, -y); }

	friend Vector2<T, FP> operator+(const T& a, const Vector2<T, FP>& t) { return t.Added(a); }
	friend Vector2<T, FP> operator-(const T& a, const Vector2<T, FP>& t) { return t.Subtracted(a); }
	friend Vector2<T, FP> operator*(const T& a, const Vector2<T, FP>& t) { return t.Multiplied(a); }
	friend Vector2<T, FP> operator/(const T& a, const Vector2<T, FP>& t) { return Vector2<T, FP>(a).Divided(t); }

	Vector2<T, FP>& operator+=(const Vector2<T, FP>& a) { return Add(a); }
	Vector2<T, FP>& operator-=(const Vector2<T, FP>& a) { return Subtract(a); }
	Vector2<T, FP>& operator*=(const Vector2<T, FP>& a) { return Multiply(a); }
	Vector2<T, FP>& operator/=(const Vector2<T, FP>& a) { return Divide(a); }
	Vector2<T, FP>& operator+=(const T& a) { return Add(a); }
	Vector2<T, FP>& operator-=(const T& a) { return Subtract(a); }
	Vector2<T, FP>& operator*=(const T& a) { return Multiply(a); }
	Vector2<T, FP>& operator/=(const T& a) { return Divide(a); }

	//Vector2<T, FP>& operator=(const Vector2<T, FP>& a) { x = a.x; y = a.y; return *this; }
	//inline Vector2<T, FP>& operator=(const T& a) { x = a; y = a; return *this; }

	bool operator==(const Vector2<T, FP>& a) const { return Equals(a); }
	bool operator!=(const Vector2<T, FP>& a) const { return !Equals(a); }

	T& operator[](const uint32& index) { return data[index]; }
	const T& operator[](const uint32& index) const { return data[index]; }
};

template <typename T, typename FP>
static FP Dot(const Vector2<T, FP>& a, Vector2<T, FP>& b) { return a.Dot(b); }
template <typename T, typename FP>
static Vector2<T, FP> Normalize(const Vector2<T, FP>& a) { return a.Normalized(); }
template <typename T, typename FP>
static Vector2<T, FP> Lerp(const Vector2<T, FP>& a, const Vector2<T, FP>& b, const FP& c) { return a.Lerp(b, c); }
template <typename T, typename FP>
static Vector2<T, FP> Reflect(const Vector2<T, FP>& ray, const Vector2<T, FP>& normal) { return ray.Reflect(normal); }
template <typename T, typename FP>
static Vector2<T, FP> Refract(const Vector2<T, FP>& ray, const Vector2<T, FP>& normal, const FP& eta) { return ray.Refract(normal, eta); }