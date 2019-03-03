#pragma once

#include "vector3.h"
#include "quaternion.h"


	template <typename T>
	struct Matrix3
	{
		union
		{
			struct { Vector3<T, T> cols[3]; };
			struct { T data[9]; };
		};

		Matrix3(T* d) { memcpy(data, d, sizeof(data)); }
		Matrix3(const Vector3<T, T>& c1, const Vector3<T, T>& c2, const Vector3<T, T>& c3) { cols[0] = c1; cols[1] = c2; cols[2] = c3; }
		Matrix3(const T& diag = 1)
		{
			data[0] = diag; data[1] = 0; data[2] = 0;
			data[3] = 0; data[4] = diag; data[5] = 0;
			data[6] = 0; data[7] = 0; data[8] = diag;
		}
		//Matrix3(const Matrix3<T>& a) { Set(a); }
		Matrix3(const Matrix3<T>& a) = default;

		Vector3<T, T>& operator[](const uint32& index) { return cols[index]; }
		const Vector3<T, T>& operator[](const uint32& index) const { return cols[index]; }

		void Set(const Matrix3<T>& a) { cols[0] = a[0]; cols[1] = a[1]; cols[2] = a[2];	}

		Matrix3<T> Multiply(const Matrix3<T>& a)
		{
			T const SrcA00 = cols[0][0];
			T const SrcA01 = cols[0][1];
			T const SrcA02 = cols[0][2];
			T const SrcA10 = cols[1][0];
			T const SrcA11 = cols[1][1];
			T const SrcA12 = cols[1][2];
			T const SrcA20 = cols[2][0];
			T const SrcA21 = cols[2][1];
			T const SrcA22 = cols[2][2];

			T const SrcB00 = a[0][0];
			T const SrcB01 = a[0][1];
			T const SrcB02 = a[0][2];
			T const SrcB10 = a[1][0];
			T const SrcB11 = a[1][1];
			T const SrcB12 = a[1][2];
			T const SrcB20 = a[2][0];
			T const SrcB21 = a[2][1];
			T const SrcB22 = a[2][2];

			Matrix3<T> Result(1);
			Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02;
			Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02;
			Result[0][2] = SrcA02 * SrcB00 + SrcA12 * SrcB01 + SrcA22 * SrcB02;
			Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12;
			Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12;
			Result[1][2] = SrcA02 * SrcB10 + SrcA12 * SrcB11 + SrcA22 * SrcB12;
			Result[2][0] = SrcA00 * SrcB20 + SrcA10 * SrcB21 + SrcA20 * SrcB22;
			Result[2][1] = SrcA01 * SrcB20 + SrcA11 * SrcB21 + SrcA21 * SrcB22;
			Result[2][2] = SrcA02 * SrcB20 + SrcA12 * SrcB21 + SrcA22 * SrcB22;
			return Result;
		}

		T Determinant() const
		{
			return
				+ cols[0][0] * (cols[1][1] * cols[2][2] - cols[2][1] * cols[1][2])
				- cols[1][0] * (cols[0][1] * cols[2][2] - cols[2][1] * cols[0][2])
				+ cols[2][0] * (cols[0][1] * cols[1][2] - cols[1][1] * cols[0][2]);
		}

		Matrix3<T> Inverse() const
		{
			T OneOverDeterminant = T(1) / Determinant();

			Matrix3<T> Inverse;
			Inverse[0][0] = +(cols[1][1] * cols[2][2] - cols[2][1] * cols[1][2]) * OneOverDeterminant;
			Inverse[1][0] = -(cols[1][0] * cols[2][2] - cols[2][0] * cols[1][2]) * OneOverDeterminant;
			Inverse[2][0] = +(cols[1][0] * cols[2][1] - cols[2][0] * cols[1][1]) * OneOverDeterminant;
			Inverse[0][1] = -(cols[0][1] * cols[2][2] - cols[2][1] * cols[0][2]) * OneOverDeterminant;
			Inverse[1][1] = +(cols[0][0] * cols[2][2] - cols[2][0] * cols[0][2]) * OneOverDeterminant;
			Inverse[2][1] = -(cols[0][0] * cols[2][1] - cols[2][0] * cols[0][1]) * OneOverDeterminant;
			Inverse[0][2] = +(cols[0][1] * cols[1][2] - cols[1][1] * cols[0][2]) * OneOverDeterminant;
			Inverse[1][2] = -(cols[0][0] * cols[1][2] - cols[1][0] * cols[0][2]) * OneOverDeterminant;
			Inverse[2][2] = +(cols[0][0] * cols[1][1] - cols[1][0] * cols[0][1]) * OneOverDeterminant;

			return Inverse;
		}

		Matrix3<T>& operator*=(const Matrix3<T>& a) { Set(Multiply(a)); return *this; }
		//Matrix3<T>& operator=(const Matrix3<T>& a) { Set(a); return *this; }

		Matrix3<T>  operator*(const Matrix3<T>& a) const { return Multiply(a); }

		Matrix3<T> Transpose() const
		{
			Matrix3<T> r;
			r[0][0] = cols[0][0]; r[0][1] = cols[1][0]; r[0][2] = cols[2][0];
			r[1][0] = cols[0][1]; r[1][1] = cols[1][1]; r[1][2] = cols[2][1];
			r[2][0] = cols[0][2]; r[2][1] = cols[1][2]; r[2][2] = cols[2][2];
			return r;
		}

		static Matrix3<T> Identity() { return Matrix3<T>(1); }
		static Matrix3<T> Rotate(const TQuaternion<T>& q)
		{
			Matrix3<T> Result;
			T qxx(q.x * q.x);
			T qyy(q.y * q.y);
			T qzz(q.z * q.z);
			T qxz(q.x * q.z);
			T qxy(q.x * q.y);
			T qyz(q.y * q.z);
			T qwx(q.w * q.x);
			T qwy(q.w * q.y);
			T qwz(q.w * q.z);

			Result[0][0] = 1 - 2 * (qyy + qzz);
			Result[0][1] = 2 * (qxy + qwz);
			Result[0][2] = 2 * (qxz - qwy);

			Result[1][0] = 2 * (qxy - qwz);
			Result[1][1] = 1 - 2 * (qxx + qzz);
			Result[1][2] = 2 * (qyz + qwx);

			Result[2][0] = 2 * (qxz + qwy);
			Result[2][1] = 2 * (qyz - qwx);
			Result[2][2] = 1 - 2 * (qxx + qyy);
			return Result;
		}

		static Matrix3<T> Rotate(const T& angle, const Vector3<T, T>& v)
		{
			T const a = angle;
			T const c = Cos<T>(a);
			T const s = Sin<T>(a);

			Vector3<T, T> axis(v.Normalized());
			Vector3<T, T> temp((T(1) - c) * axis);

			Matrix3<T> Rotate;
			Rotate[0][0] = c + temp[0] * axis[0];
			Rotate[0][1] = 0 + temp[0] * axis[1] + s * axis[2];
			Rotate[0][2] = 0 + temp[0] * axis[2] - s * axis[1];

			Rotate[1][0] = 0 + temp[1] * axis[0] - s * axis[2];
			Rotate[1][1] = c + temp[1] * axis[1];
			Rotate[1][2] = 0 + temp[1] * axis[2] + s * axis[0];

			Rotate[2][0] = 0 + temp[2] * axis[0] + s * axis[1];
			Rotate[2][1] = 0 + temp[2] * axis[1] - s * axis[0];
			Rotate[2][2] = c + temp[2] * axis[2];

			return Rotate;
		}
	};

	template <typename T>
	static TQuaternion<T> FromMatrix3(const Matrix3<T>& m)
	{
		T fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
		T fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
		T fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
		T fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

		int biggestIndex = 0;
		T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
		if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourXSquaredMinus1;
			biggestIndex = 1;
		}
		if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourYSquaredMinus1;
			biggestIndex = 2;
		}
		if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourZSquaredMinus1;
			biggestIndex = 3;
		}

		T biggestVal = sqrt(fourBiggestSquaredMinus1 + T(1)) * T(0.5);
		T mult = static_cast<T>(0.25) / biggestVal;

		TQuaternion<T> Result();
		switch (biggestIndex)
		{
		case 0:
			Result.w = biggestVal;
			Result.x = (m[1][2] - m[2][1]) * mult;
			Result.y = (m[2][0] - m[0][2]) * mult;
			Result.z = (m[0][1] - m[1][0]) * mult;
			break;
		case 1:
			Result.w = (m[1][2] - m[2][1]) * mult;
			Result.x = biggestVal;
			Result.y = (m[0][1] + m[1][0]) * mult;
			Result.z = (m[2][0] + m[0][2]) * mult;
			break;
		case 2:
			Result.w = (m[2][0] - m[0][2]) * mult;
			Result.x = (m[0][1] + m[1][0]) * mult;
			Result.y = biggestVal;
			Result.z = (m[1][2] + m[2][1]) * mult;
			break;
		case 3:
			Result.w = (m[0][1] - m[1][0]) * mult;
			Result.x = (m[2][0] + m[0][2]) * mult;
			Result.y = (m[1][2] + m[2][1]) * mult;
			Result.z = biggestVal;
			break;
		}
		return Result;
	}

