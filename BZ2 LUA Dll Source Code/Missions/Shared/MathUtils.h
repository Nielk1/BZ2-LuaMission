#ifndef _Math_
#define _Math_

#include "..\..\source\fun3d\ScriptUtils.h"
//#include "ScriptUtilsExtension.h"
#include <math.h>
#include <float.h>
//#include <algorithm>

struct Quaternion
{
	F32 s;
	Vector v;

	Quaternion() {}

	Quaternion(const float vs, const Vector vv)
		:s(vs), v(vv)
	{
	}
};

extern Vector Normalize_Vector(const Vector &A);
extern Vector Cross_Product(const Vector &A, const Vector &B);
extern Matrix Build_Directinal_Matrix (const Vector &Position, const Vector &Direction);
extern Matrix Interpolate_Matrix(const Matrix &m0, const Matrix &m1, const float t);
extern Quaternion Interpolate_Quaternion(const Quaternion &q0, const Quaternion &q1, const float t);
extern Quaternion Normalize_Quaternion (const Quaternion &q);
extern Quaternion Matrix_to_Quaternion(const Matrix &M, Quaternion &Q, Vector &P);
extern Vector Interpolate_Position(const Vector &p0, const Vector &p1, const float t);
extern void Matrix_to_QuatPos(const Matrix &M, Quaternion &Q, Vector &P);
extern void QuatPos_to_Matrix(const Quaternion &Q, const Vector &P, Matrix &M);
// Math stuff from Ken Miller.
extern Vector Vector_Transform(const Matrix &M, const Vector &v);
extern Vector Vector_TransformInv(const Matrix &M, const Vector &v);
extern Vector Vector_Rotate(const Matrix &M, const Vector &v);
extern Vector Vector_RotateInv (const Matrix &M, const Vector &v);
extern Matrix Matrix_Inverse(const Matrix &M);
extern Matrix Matrix_Multiply(const Matrix &A, const Matrix &B);
extern Matrix Build_Orthogonal_Matrix(const Vector &Up, const Vector &Heading, const Vector &Position = Vector(0,0,0));
extern Matrix Build_Yaw_Matrix(const float Yaw, const Vector &Position = Vector(0,0,0));
extern Matrix Build_Position_Rotation_Matrix (const float pitch, const float yaw, const float roll, const Vector &Position = Vector(0,0,0));
extern Matrix Build_Axis_Rotation_Matrix (const float Angle, const Vector &Axis);

inline Matrix Build_Position_Rotation_Matrix (const Vector &Rotation, const Vector &Position = Vector(0,0,0)) { return Build_Position_Rotation_Matrix(Rotation.x, Rotation.y, Rotation.z, Position); }
inline float Dot_Product(const Vector &A, const Vector &B) { return A.x * B.x + A.y * B.y + A.z * B.z; }
inline Vector Sub_Vectors(Vector const &v1, Vector const &v2) { return Vector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
inline Vector Add_Vectors(Vector const &v1, Vector const &v2) { return Vector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
inline Vector Vector_Scale(const Vector &V, const float Mult) { return Vector(V.x * Mult, V.y * Mult, V.z * Mult); }
inline Vector Vector_Scale(const float Mult, const Vector &V) { return Vector(V.x * Mult, V.y * Mult, V.z * Mult); }
inline Vector Add_Mult_Vectors(const Vector &v1, const Vector &v2, float Mult) { return Vector(v1.x + v2.x * Mult, v1.y + v2.y * Mult, v1.z + v2.z * Mult); }
inline Vector Neg_Vector(const Vector &v1) { return Vector(-v1.x, -v1.y, -v1.z); }
inline Matrix Build_Identity_Matrix(const Vector &Pos) { return Matrix(Vector(1, 0, 0),Vector(0, 1, 0), Vector(0, 0, 1), Pos); }

inline bool const close_enough(const float &F1, const float &F2, const float Tolerance) { return fabsf(F2 - F1) < Tolerance; }
inline bool const close_enough(const Vector &V1, const Vector &V2, const float Tolerance) { return close_enough( V1.x, V2.x, Tolerance) && close_enough( V1.y, V2.y, Tolerance) && close_enough( V1.z, V2.z, Tolerance); }

extern int UnsignedToSigned(unsigned int x);

inline bool IsNullVector(const Vector v) { return ((v.x * v.x) + (v.y * v.y) + (v.z * v.z) < 0.1f); }

// Tests if the point v3 is Left, On, or Right of the 2D line between the two vectors v1 and v2. > 0 = left, 0 = on, < 0 = right.
template<typename V1, typename V2, typename V3> inline int IsLeftOnRight(const V1 &v1, const V2 &v2, const V3 &v3) { return int(((v2.x - v1.x) * (v3.z - v1.z) - (v3.x - v1.x) * (v2.z - v1.z))); }

extern int GetRandomInt(const int Min, const int Max);
inline int GetRandomInt(const int Max) { return GetRandomInt(0, Max); }
extern float GetRandomFloat(const float Min, const float Max);

// Get the 3D distance between two objects. BZ2's GetDistance call only checks 2d distance.
inline float GetDistance3DSquared(const Vector &v1, const Vector &v2) { return (v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y) + (v2.z - v1.z) * (v2.z - v1.z); }
inline float GetDistance3D(const Vector &v1, const Vector &v2) { return sqrtf((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y) + (v2.z - v1.z) * (v2.z - v1.z)); }
inline float GetDistance3DSquared(const Handle h1, const Handle h2) { if(!IsAround(h1) || !IsAround(h2)) return FLT_MAX; else return GetDistance3DSquared(GetPosition(h1), GetPosition(h2)); }
inline float GetDistance3D(const Handle h1, const Handle h2) { if(!IsAround(h1) || !IsAround(h2)) return FLT_MAX; else return GetDistance3D(GetPosition(h1), GetPosition(h2)); }
// GetDistance 2D, takes in optional Vector or Vector_2D.
template<typename V1, typename V2> inline float GetDistance2DSquared(const V1 &v1, const V2 &v2) { return (v2.x - v1.x) * (v2.x - v1.x) + (v2.z - v1.z) * (v2.z - v1.z); }
template<typename V1, typename V2> inline float GetDistance2D(const V1 &v1, const V2 &v2) { return sqrtf((v2.x - v1.x) * (v2.x - v1.x) + (v2.z - v1.z) * (v2.z - v1.z)); }
// Version that takes a Handle. // Same as GetDistance(h1, h2)
//inline float GetDistance2DSquared(const Handle h1, const Handle h2) { if(!IsAround(h1) || !IsAround(h2)) return FLT_MAX; else return GetDistance2DSquared(GetPosition(h1), GetPosition(h2)); }
//inline float GetDistance2D(const Handle h1, const Handle h2) { if(!IsAround(h1) || !IsAround(h2)) return FLT_MAX; else return GetDistance2D(GetPosition(h1), GetPosition(h2)); }
// Gets the length of a Vector.
inline float GetLength3DSquared(const Vector v) { return v.x * v.x + v.y * v.y + v.z * v.z; }
inline float GetLength3D(const Vector v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }
inline float GetLength2DSquared(const Vector v) { return v.x * v.x + v.z * v.z; }
inline float GetLength2D(const Vector v) { return sqrtf(v.x * v.x + v.z * v.z); }
// Square a value.
template<typename T> inline T Squared(const T x) { return x * x; }

// return the minimum of two values
template <class T>
inline T min(const T a, const T b) { return (a < b) ? (a) : (b); }

// return the maximum of two values
template <class T>
inline T max(const T a, const T b) { return (a < b) ? (b) : (a); }

// clamp a value between a minimum and maximum
template <class T>
inline T clamp(const T x, const T min, const T max)
{
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}

inline Vector operator-(const Vector &a, const Vector &b) { return Vector(a.x - b.x, a.y - b.y, a.z - b.z); }
inline Vector operator+(const Vector &a, const Vector &b) { return Vector(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vector operator*(const Vector &a, const Vector &b) { return Vector(a.x * b.x, a.y * b.y, a.z * b.z); }
inline Vector operator*(const float s, const Vector &v) { return Vector(s * v.x, s * v.y, s * v.z); }
inline Vector operator*(const Vector &v, const float s) { return Vector(v.x * s, v.y * s, v.z * s); }
inline Vector operator/(const Vector &a, const Vector &b) { return Vector(a.x / b.x, a.y / b.y, a.z / b.z); }
inline Vector operator/(const float s, const Vector &v) { return Vector(s / v.x, s / v.y, s / v.z); }
inline Vector operator/(const Vector &v, const float s) { return Vector(v.x / s, v.y / s, v.z / s); }

inline Vector operator*(const Matrix &M, const Vector &v) { return Vector_Transform(M, v); }
inline Matrix operator*(const Matrix &A, const Matrix &B) { return Matrix_Multiply(A, B); }

inline bool operator==(const Vector &a, const Vector &b){ return a.x == b.x && a.y == b.y && a.z == b.z; }
inline bool operator!=(const Vector &a, const Vector &b){ return !(a == b); }
inline bool operator==(const Matrix &a, const Matrix &b){ return a.front == b.front && a.up == b.up && a.right == b.right && a.posit == b.posit; }
inline bool operator!=(const Matrix &a, const Matrix &b){ return !(a == b); }

#endif
