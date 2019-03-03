#pragma once

#include "vector4.h"
#include "matrix3.h"

	template <typename T>
	struct Matrix4
	{
		union
		{
			struct { Vector4<T, T> cols[4]; };
			struct { T data[16]; };
		};

		Matrix4<T>()
		{
			data[0] = 1; data[1] = 0; data[2] = 0; data[3] = 0;
			data[4] = 0; data[5] = 1; data[6] = 0; data[7] = 0;
			data[8] = 0; data[9] = 0; data[10] = 1; data[11] = 0;
			data[12] = 0; data[13] = 0; data[14] = 0; data[15] = 1;
		}
		Matrix4<T>(T* d) { memcpy(data, d, sizeof(data)); }
		Matrix4<T>(const Vector4<T, T>& c1, const Vector4<T, T>& c2, const Vector4<T, T>& c3, const Vector4<T, T>& c4) { cols[0] = c1; cols[1] = c2; cols[2] = c3; cols[3] = c4; }
		Matrix4<T>(const T& diag)
		{
			data[0] = diag; data[1] = 0; data[2] = 0; data[3] = 0;
			data[4] = 0; data[5] = diag; data[6] = 0; data[7] = 0;
			data[8] = 0; data[9] = 0; data[10] = diag; data[11] = 0;
			data[12] = 0; data[13] = 0; data[14] = 0; data[15] = diag;
		}
		Matrix4<T>(const Matrix3<T>& a)
		{
			cols[0].xyz = a[0]; cols[0].w = 0;
			cols[1].xyz = a[1]; cols[1].w = 0;
			cols[2].xyz = a[2]; cols[2].w = 0;
			cols[3] = Vector4<T, T>(0, 0, 0, 1);
		}
		//Matrix4(const Matrix4<T>& a) { Set(a); }
		Matrix4(const Matrix4<T>& a) = default;

		Vector4<T, T>& operator[](const uint32& index) { return cols[index]; }
		const Vector4<T, T>& operator[](const uint32& index) const { return cols[index]; }

		void Set(const Matrix4<T>& a) { cols[0] = a[0]; cols[1] = a[1]; cols[2] = a[2];	cols[3] = a[3]; }

		Matrix3<T> ToMatrix3() const
		{
			Matrix3<T> r;
			r[0] = cols[0].xyz;
			r[1] = cols[1].xyz;
			r[2] = cols[2].xyz;
			return r;
		}

		Matrix4<T> Multiply(const Matrix4<T>& a) const
		{
			const Vector4<T, T>& SrcA0 = cols[0];
			const Vector4<T, T>& SrcA1 = cols[1];
			const Vector4<T, T>& SrcA2 = cols[2];
			const Vector4<T, T>& SrcA3 = cols[3];

			const Vector4<T, T>& SrcB0 = a[0];
			const Vector4<T, T>& SrcB1 = a[1];
			const Vector4<T, T>& SrcB2 = a[2];
			const Vector4<T, T>& SrcB3 = a[3];

			Matrix4<T> Result;
			Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
			Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
			Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
			Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];
			return Result;
		}

		Vector4<T, T> Multiply(const Vector4<T, T>& v) const
		{
			Vector4<T, T> Mov0(v[0]);
			Vector4<T, T> Mov1(v[1]);
			Vector4<T, T> Mul0 = cols[0] * Mov0;
			Vector4<T, T> Mul1 = cols[1] * Mov1;
			Vector4<T, T> Add0 = Mul0 + Mul1;
			Vector4<T, T> Mov2(v[2]);
			Vector4<T, T> Mov3(v[3]);
			Vector4<T, T> Mul2 = cols[2] * Mov2;
			Vector4<T, T> Mul3 = cols[3] * Mov3;
			Vector4<T, T> Add1 = Mul2 + Mul3;
			Vector4<T, T> Add2 = Add0 + Add1;
			return Add2;
		}

		Matrix4<T> Multiply(const T& a) const
		{
			Matrix4<T> r;
			r[0] = cols[0] * a;
			r[1] = cols[1] * a;
			r[2] = cols[2] * a;
			r[3] = cols[3] * a;
			return r;
		}

		Matrix4<T>& MultiplySelf(const T& a)
		{
			cols[0] *= a;
			cols[1] *= a;
			cols[2] *= a;
			cols[3] *= a;
			return *this;
		}

		T Determinant() const
		{
			T SubFactor00 = cols[2][2] * cols[3][3] - cols[3][2] * cols[2][3];
			T SubFactor01 = cols[2][1] * cols[3][3] - cols[3][1] * cols[2][3];
			T SubFactor02 = cols[2][1] * cols[3][2] - cols[3][1] * cols[2][2];
			T SubFactor03 = cols[2][0] * cols[3][3] - cols[3][0] * cols[2][3];
			T SubFactor04 = cols[2][0] * cols[3][2] - cols[3][0] * cols[2][2];
			T SubFactor05 = cols[2][0] * cols[3][1] - cols[3][0] * cols[2][1];

			Vector4<T, T> DetCof(
				+(cols[1][1] * SubFactor00 - cols[1][2] * SubFactor01 + cols[1][3] * SubFactor02),
				-(cols[1][0] * SubFactor00 - cols[1][2] * SubFactor03 + cols[1][3] * SubFactor04),
				+(cols[1][0] * SubFactor01 - cols[1][1] * SubFactor03 + cols[1][3] * SubFactor05),
				-(cols[1][0] * SubFactor02 - cols[1][1] * SubFactor04 + cols[1][2] * SubFactor05));

			return
				cols[0][0] * DetCof[0] + cols[0][1] * DetCof[1] +
				cols[0][2] * DetCof[2] + cols[0][3] * DetCof[3];
		}

		Matrix4<T> Inverse() const
		{
			T Coef00 = cols[2][2] * cols[3][3] - cols[3][2] * cols[2][3];
			T Coef02 = cols[1][2] * cols[3][3] - cols[3][2] * cols[1][3];
			T Coef03 = cols[1][2] * cols[2][3] - cols[2][2] * cols[1][3];

			T Coef04 = cols[2][1] * cols[3][3] - cols[3][1] * cols[2][3];
			T Coef06 = cols[1][1] * cols[3][3] - cols[3][1] * cols[1][3];
			T Coef07 = cols[1][1] * cols[2][3] - cols[2][1] * cols[1][3];

			T Coef08 = cols[2][1] * cols[3][2] - cols[3][1] * cols[2][2];
			T Coef10 = cols[1][1] * cols[3][2] - cols[3][1] * cols[1][2];
			T Coef11 = cols[1][1] * cols[2][2] - cols[2][1] * cols[1][2];

			T Coef12 = cols[2][0] * cols[3][3] - cols[3][0] * cols[2][3];
			T Coef14 = cols[1][0] * cols[3][3] - cols[3][0] * cols[1][3];
			T Coef15 = cols[1][0] * cols[2][3] - cols[2][0] * cols[1][3];

			T Coef16 = cols[2][0] * cols[3][2] - cols[3][0] * cols[2][2];
			T Coef18 = cols[1][0] * cols[3][2] - cols[3][0] * cols[1][2];
			T Coef19 = cols[1][0] * cols[2][2] - cols[2][0] * cols[1][2];

			T Coef20 = cols[2][0] * cols[3][1] - cols[3][0] * cols[2][1];
			T Coef22 = cols[1][0] * cols[3][1] - cols[3][0] * cols[1][1];
			T Coef23 = cols[1][0] * cols[2][1] - cols[2][0] * cols[1][1];

			Vector4<T, T> Fac0(Coef00, Coef00, Coef02, Coef03);
			Vector4<T, T> Fac1(Coef04, Coef04, Coef06, Coef07);
			Vector4<T, T> Fac2(Coef08, Coef08, Coef10, Coef11);
			Vector4<T, T> Fac3(Coef12, Coef12, Coef14, Coef15);
			Vector4<T, T> Fac4(Coef16, Coef16, Coef18, Coef19);
			Vector4<T, T> Fac5(Coef20, Coef20, Coef22, Coef23);

			Vector4<T, T> Vec0(cols[1][0], cols[0][0], cols[0][0], cols[0][0]);
			Vector4<T, T> Vec1(cols[1][1], cols[0][1], cols[0][1], cols[0][1]);
			Vector4<T, T> Vec2(cols[1][2], cols[0][2], cols[0][2], cols[0][2]);
			Vector4<T, T> Vec3(cols[1][3], cols[0][3], cols[0][3], cols[0][3]);

			Vector4<T, T> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
			Vector4<T, T> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
			Vector4<T, T> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
			Vector4<T, T> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

			Vector4<T, T> SignA(+1, -1, +1, -1);
			Vector4<T, T> SignB(-1, +1, -1, +1);
			Matrix4<T> Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

			Vector4<T, T> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

			Vector4<T, T> Dot0(cols[0] * Row0);
			T Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

			T OneOverDeterminant = static_cast<T>(1) / Dot1;

			return Inverse * OneOverDeterminant;
		}

		//inline Matrix4<T>& operator=(const Matrix4<T>& a) { Set(a); return *this; }

		Matrix4<T>& operator*=(const Matrix4<T>& a) { Set(Multiply(a)); return *this; }
		Matrix4<T>& operator*=(const T& a) { MultiplySelf(a); return *this; }

		Matrix4<T>  operator*(const T& a) const { return Multiply(a); }
		Matrix4<T>  operator*(const Matrix4<T>& a) const { return Multiply(a); }
		Vector4<T, T>  operator*(const Vector4<T, T>& a) const { return Multiply(a); }

		Matrix4<T> Transpose() const
		{
			Matrix4<T> r;
			r[0][0] = cols[0][0]; r[0][1] = cols[1][0]; r[0][2] = cols[2][0]; r[0][3] = cols[3][0];
			r[1][0] = cols[0][1]; r[1][1] = cols[1][1]; r[1][2] = cols[2][1]; r[1][3] = cols[3][1];
			r[2][0] = cols[0][2]; r[2][1] = cols[1][2]; r[2][2] = cols[2][2]; r[2][3] = cols[3][2];
			r[3][0] = cols[0][3]; r[3][1] = cols[1][3]; r[3][2] = cols[2][3]; r[3][3] = cols[3][3];
			return r;
		}

		static Matrix4<T> Identity() { return Matrix4<T>(1); }
		static Matrix4<T> Rotate(const TQuaternion<T>& q) { return Matrix4<T>(Matrix3<T>::Rotate(q)); }
		static Matrix4<T> Rotate(const T& angle, const Vector3<T, T>& v) { return Matrix4<T>(Matrix3<T>::Rotate(angle, v)); }
		static Matrix4<T> Translate(const Vector3<T, T>& v) 
		{
			Matrix4<T> Result;
			Result[3] = Vector4<T, T>(v.x, v.y, v.z, 1);
			return Result;
		}
		static Matrix4<T> Scale(const Vector3<T, T>& v)
		{
			Matrix4<T> Result;
			Result[0][0] = v.x;
			Result[1][1] = v.y;
			Result[2][2] = v.z;
			return Result;
		}
		static Matrix4<T> Ortho(const T& left, const T& right, const T& bottom, const T& top, const T& zNear, const T& zFar)
		{
			Matrix4<T> Result(1);
			Result[0][0] = static_cast<T>(2) / (right - left);
			Result[1][1] = static_cast<T>(2) / (top - bottom);
			Result[2][2] = -static_cast<T>(2) / (zFar - zNear);
			Result[3][0] = -(right + left) / (right - left);
			Result[3][1] = -(top + bottom) / (top - bottom);
			Result[3][2] = -(zFar + zNear) / (zFar - zNear);
			return Result;
		}
		static Matrix4<T> Frustum(const T& left, const T& right, const T& bottom, const T& top, const T& nearVal, const T& farVal)
		{
			Matrix4<T> Result(0);
			Result[0][0] = (static_cast<T>(2) * nearVal) / (right - left);
			Result[1][1] = (static_cast<T>(2) * nearVal) / (top - bottom);
			Result[2][0] = (right + left) / (right - left);
			Result[2][1] = (top + bottom) / (top - bottom);
			Result[2][2] = -(farVal + nearVal) / (farVal - nearVal);
			Result[2][3] = static_cast<T>(-1);
			Result[3][2] = -(static_cast<T>(2) * farVal * nearVal) / (farVal - nearVal);
			return Result;
		}
		static Matrix4<T> Perspective(const T& fovy, const T& aspect, const T& zNear, const T& zFar)
		{
			T const tanHalfFovy = Tan<T>(fovy / static_cast<T>(2));

			Matrix4<T> Result(static_cast<T>(0));
			Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
			Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
			Result[2][2] = -(zFar + zNear) / (zFar - zNear);
			Result[2][3] = -static_cast<T>(1);
			Result[3][2] = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
			return Result;
		}
		static Matrix4<T> LookAt(const Vector3<T, T>& eye, const Vector3<T, T>& center, const Vector3<T, T>& up)
		{
			Vector3<T, T> const f(Normalize(center - eye));
			Vector3<T, T> const s(Normalize(Cross(f, up)));
			Vector3<T, T> const u(Cross(s, f));

			Matrix4<T> Result(1);
			Result[0][0] = s.x;
			Result[1][0] = s.y;
			Result[2][0] = s.z;
			Result[0][1] = u.x;
			Result[1][1] = u.y;
			Result[2][1] = u.z;
			Result[0][2] = -f.x;
			Result[1][2] = -f.y;
			Result[2][2] = -f.z;
			Result[3][0] = -s.Dot(eye);
			Result[3][1] = -u.Dot(eye);
			Result[3][2] = f.Dot(eye);
			return Result;
		}
	};

	template <typename T>
	Matrix4<T> Inverse(const Matrix4<T>& a) { return a.Inverse(); }

