#pragma once

#include "vector4.h"

	template<typename T>
	struct TQuaternion
	{
		union
		{
			struct { T x, y, z, w; };
			struct { Vector3<T, T> xyz; };
			struct { T data[4]; };
		};

		TQuaternion(const TQuaternion<T>& q) { x = q.x; y = q.y; z = q.z; w = q.w; }
		TQuaternion() { x = 0; y = 0; z = 0; w = 1; }
		TQuaternion(const T& a, const T& b, const T& c, const T& d) { x = a; y = b; z = c; w = d; }
		TQuaternion(const Vector4<T, T>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
		TQuaternion(const Vector3<T, T>& eulerAngles)
		{
			Vector3<T, T> c(Cos<T>(eulerAngles.x * T(0.5)), Cos<T>(eulerAngles.y * T(0.5)), Cos<T>(eulerAngles.z * T(0.5)));
			Vector3<T, T> s(Sin<T>(eulerAngles.x * T(0.5)), Sin<T>(eulerAngles.y * T(0.5)), Sin<T>(eulerAngles.z * T(0.5)));

			this->w = c.x * c.y * c.z + s.x * s.y * s.z;
			this->x = s.x * c.y * c.z - c.x * s.y * s.z;
			this->y = c.x * s.y * c.z + s.x * c.y * s.z;
			this->z = c.x * c.y * s.z - s.x * s.y * c.z;
		}

		TQuaternion(const T& angle, const Vector3<T, T>& axis)
		{
			xyz = axis * Sin(angle);
			w = Cos(angle);
		}

		static TQuaternion<T> Identity() { return TQuaternion<T>(0, 0, 0, 1); }

		Vector3<T, T> ToEuler() const
		{
			return Vector3<T, T>(Atan2(2 * x * w - 2 * y * z, 1 - 2 * x * x - 2 * z * z),
				Atan2(2 * y * w - 2 * x * z, 1 - 2 * y * y - 2 * z * z),
				Asin(2 * x * y + 2 * z * w));
		}

		TQuaternion<T> FromEuler(const Vector3<T, T>& angles)
		{
			TQuaternion<T> pitch(angles.x, Vector3<T, T>(1.0, 0.0, 0.0));
			TQuaternion<T> yaw(angles.y, Vector3<T, T>(0.0, 1.0, 0.0));
			TQuaternion<T> roll(angles.z, Vector3<T, T>(0.0, 0.0, 1.0));
			return pitch * yaw * roll;
		}

		T Dot(const TQuaternion<T>& a) const { return x * a.x + y * a.y + z * a.z + w * a.w; }
		T Length() const { return Sqrt<T>(x * x + y * y + z * z + w * w); }
		T LengthSquared() const { return x * x + y * y + z * z + w * w; }

		TQuaternion<T> Normalized() const { T l = Length(); return TQuaternion<T>(x / l, y / l, z / l, w / l); }
		TQuaternion<T> Added(const TQuaternion<T>& a) const { return TQuaternion<T>(x + a.x, y + a.y, z + a.z, w + a.w); }
		TQuaternion<T> Subtracted(const TQuaternion<T>& a) const { return TQuaternion<T>(x - a.x, y - a.y, z - a.z, w - a.w); }
		TQuaternion<T> Divided(const TQuaternion<T>& a) const { return TQuaternion<T>(x / a.x, y / a.y, z / a.z, w / a.w); }
		bool Equals(const TQuaternion<T>& a) const { return x == a.x && y == a.y && z == a.z && w == a.w; }

		TQuaternion<T> Multiplied(const TQuaternion<T>& q) const
		{
			TQuaternion<T> r;
			r.w = w * q.w - x * q.x - y * q.y - z * q.z;
			r.x = w * q.x + x * q.w + y * q.z - z * q.y;
			r.y = w * q.y + y * q.w + z * q.x - x * q.z;
			r.z = w * q.z + z * q.w + x * q.y - y * q.x;
			return r;
		}

		Vector3<T, T> Multiplied(const Vector3<T, T>& v) const
		{
			Vector3<T, T> qv(x, y, z);
			Vector3<T, T> uv(qv.Cross(v));
			Vector3<T, T> uuv(qv.Cross(uv));
			return v + ((uv * w) + uuv) * T(2);
		}

		TQuaternion<T>& Normalize() { T l = Length(); x /= l; y /= l; z /= l; w /= l; return *this; }
		TQuaternion<T>& Add(const TQuaternion<T>& a) { x += a.x; y += a.y; z += a.z; w += a.w; return *this; }
		TQuaternion<T>& Subtract(const TQuaternion<T>& a) { x -= a.x; y -= a.y; z -= a.z; w -= a.w; return *this; }
		TQuaternion<T>& Divide(const TQuaternion<T>& a) { x /= a.x; y /= a.y; z /= a.z; w /= a.w; return *this; }
		TQuaternion<T>& Multiply(const TQuaternion<T>& q)
		{
			TQuaternion<T> r;
			r.w = w * q.w - x * q.x - y * q.y - z * q.z;
			r.x = w * q.x + x * q.w + y * q.z - z * q.y;
			r.y = w * q.y + y * q.w + z * q.x - x * q.z;
			r.z = w * q.z + z * q.w + x * q.y - y * q.x;
			x = r.x; y = r.y; z = r.z; w = r.w;
			return *this;
		}

		TQuaternion<T> operator+(const TQuaternion<T>& a) const { return Added(a); }
		TQuaternion<T> operator-(const TQuaternion<T>& a) const { return Subtracted(a); }
		TQuaternion<T> operator*(const TQuaternion<T>& a) const { return Multiplied(a); }
		TQuaternion<T> operator/(const TQuaternion<T>& a) const { return Divided(a); }
		TQuaternion<T> operator-() const { return TQuaternion<T>(-x, -y, -z, -w); }

		Vector3<T, T> operator*(const Vector3<T, T>& a) const { return Multiplied(a); }

		TQuaternion<T>& operator+=(const TQuaternion<T>& a) { return Add(a); }
		TQuaternion<T>& operator-=(const TQuaternion<T>& a) { return Subtract(a); }
		TQuaternion<T>& operator*=(const TQuaternion<T>& a) { return Multiply(a); }
		TQuaternion<T>& operator/=(const TQuaternion<T>& a) { return Divide(a); }
		TQuaternion<T>& operator=(const TQuaternion<T>& a) { x = a.x; y = a.y; z = a.z; w = a.w; return *this; }

		TQuaternion<T> Rotate(const TQuaternion<T>& a) { return a * b; }
		TQuaternion<T> Inverse() { return Conjugated() / Dot(*this); }
		TQuaternion<T> Conjugated() { return TQuaternion<T>(-x, -y, -z, w); }
		TQuaternion<T>& Conjugate() { x = -x; y = -y; z = -z; return *this; }
		TQuaternion<T> Cross(const TQuaternion<T>& q2)
		{
			return TQuaternion<T>(
				w * q2.x + x * q2.w + y * q2.z - z * q2.y,
				w * q2.y + y * q2.w + z * q2.x - x * q2.z,
				w * q2.z + z * q2.w + x * q2.y - y * q2.x,
				w * q2.w - x * q2.x - y * q2.y - z * q2.z);
		}

		T& operator[](const uint32& index) { return data[index]; }
		const T& operator[](const uint32& index) const { return data[index]; }

		//TQuaternion<T> Lerp(const TQuaternion<T>& q, const T& a) { return (*this * (T(1) - a)) + (q * a); }

	};

	template <typename T>
	static T Dot(const TQuaternion<T>& x, const TQuaternion<T>& y) { return x.Dot(y); }
	template <typename T>
	static T Cross(const TQuaternion<T>& x, const TQuaternion<T>& y) { return x.Cross(y); }
	template <typename T>
	static TQuaternion<T> Normalize(const TQuaternion<T>& x) { return x.Normalized(); }
	/*template <typename T>
	TQuaternion<T> Lerp(const TQuaternion<T>& x, const TQuaternion<T>& y, const T& a) { return x.Lerp(y, a); }

	template <typename T>
	TQuaternion<T> Slerp(const TQuaternion<T>& x, const TQuaternion<T>& y, const T& a)
	{
		{
			TQuaternion<T> z = y;
			T cosTheta = x.Dot(y);
			if (cosTheta < T(0))
			{
				z = -y;
				cosTheta = -cosTheta;
			}
			if (cosTheta > T(1) - Epsilon<T>())
			{
				return TQuaternion<T>(
					Mix<T>(x.x, z.x, a),
					Mix<T>(x.y, z.y, a),
					Mix<T>(x.z, z.z, a),
					Mix<T>(x.w, z.w, a));
			}
			else
			{
				T angle = Acos<T>(cosTheta);
				return x * (Sin<T>((T(1) - a) * angle) + Sin<T>(a * angle) * z) / Sin<T>(angle);
			}
		}
	}*/

