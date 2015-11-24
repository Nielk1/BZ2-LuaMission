#include "MathUtils.h"
#include <math.h>
#include <memory.h>
#include <malloc.h>

// compute the cross product of two vectors
Vector Cross_Product (const Vector &A, const Vector &B)
{
	Vector Dst(0, 0, 0);
	Dst.x = A.y * B.z - A.z * B.y;
	Dst.y = A.z * B.x - A.x * B.z;
	Dst.z = A.x * B.y - A.y * B.x;
	return Dst;
}

// Create a matrix with its front vector aimed in a given direction
Matrix Build_Directinal_Matrix (const Vector &Position, const Vector &Direction)
{
	Matrix M;
	double xz2;
	
	M.posit = Position;
	M.positw = 1.0f;

	M.front = Normalize_Vector (Direction);
	M.frontw = 0.0f;
	
	xz2 = M.front.x * M.front.x + M.front.z * M.front.z;
	if ( xz2 < 0.02 )
	{
		M.right.x = 1.0f;
		M.right.y = 0.0f;
		M.right.z = 0.0f;
		M.rightw  = 0.0f;
	}
	else
	{
		M.up.x = 0.0f;
		M.up.y = 1.0f;
		M.up.z = 0.0f;
		M.right = Cross_Product (M.up, M.front);
		M.right = Normalize_Vector (M.right);
		M.rightw  = 0.0f;
	}
	
	M.up = Cross_Product (M.front, M.right);
	M.upw  = 0.0f;
	
	return M;
}

// Normalize Vector from SPMission DLL.
Vector Normalize_Vector (const Vector &A)
{
	float scale = 1.0f / sqrtf(A.x * A.x + A.y * A.y + A.z * A.z);
	
	Vector Dst(0, 0, 0);
	Dst.x = (A.x * scale);
	Dst.y = (A.y * scale);
	Dst.z = (A.z * scale);
	
	return Dst;
}

// INTERPOLATE_QUATERNION
// Interpolate between two quaternions

#define EPSILON	0.001f
#define HUGE_NUMBER  (1.0f/EPSILON)

Quaternion Interpolate_Quaternion(const Quaternion &q0, const Quaternion &q1, const float t)
{
	// quaternion to generate
	Quaternion q;

	// calculate cosine
	double dot = 
		q0.s   * q1.s   +
		q0.v.x * q1.v.x +
		q0.v.y * q1.v.y +
		q0.v.z * q1.v.z;

	// adjust signs (if necessary)
	if (dot < 0.0)
	{
		dot = -dot;
		q.s   = -q1.s;
		q.v.x = -q1.v.x;
		q.v.y = -q1.v.y;
		q.v.z = -q1.v.z;
	}
	else
	{
		q.s   = q1.s;
		q.v.x = q1.v.x;
		q.v.y = q1.v.y;
		q.v.z = q1.v.z;
	}

	// calculate coefficients
	double scale0, scale1;
	if (1.0 - dot > EPSILON) 
	{
		// spherical linear interpolation
		double omega = acos(dot);
		double scale = 1.0 / sin(omega);
		scale0 = sin((1.0 - t) * omega) * scale;
		scale1 = sin(t * omega) * scale;
	}
	else
	{
		// linear interpolation
		scale0 = 1.0 - t;
		scale1 = t;
	}

	// calculate final values
	q.s   = float(scale0 * q0.s   + scale1 * q.s  );
	q.v.x = float(scale0 * q0.v.x + scale1 * q.v.x);
	q.v.y = float(scale0 * q0.v.y + scale1 * q.v.y);
	q.v.z = float(scale0 * q0.v.z + scale1 * q.v.z);

	// return the quaternion
	return q;
}

Quaternion Normalize_Quaternion (const Quaternion &q)
{
	// get quaternion length
	double dot = 
		q.s   * q.s   + 
		q.v.x * q.v.x + 
		q.v.y * q.v.y + 
		q.v.z * q.v.z;

	// calculate scale factor
	dot = (dot == 0.0f) ? HUGE_NUMBER : 1.0f / sqrt(dot);

	// apply scale factor
	Quaternion Q;
	Q.s   = float(q.s   * dot);
	Q.v.x = float(q.v.x * dot);
	Q.v.y = float(q.v.y * dot);
	Q.v.z = float(q.v.z * dot);
	return Q;
}



// convert a matrix to a quaternion and a position
void Matrix_to_QuatPos(const Matrix &M, Quaternion &Q, Vector &P)
{
	// get the trace of the matrix
	F32 w = M.right.x + M.up.y + M.front.z;

	// if the trace is positive...
	if (w > 0.0f)
	{
		w = (F32) sqrt(w + 1.0f);
		Q.s = w * 0.5f;
		
		w = 0.5f / w;
		Q.v.x = (M.front.y - M.up.z   ) * w;
		Q.v.y = (M.right.z - M.front.x) * w;
		Q.v.z = (M.up.x    - M.right.y) * w;
	}
	else
	{
		// treat matrix and quaterion as arrays
		F32 *m = (F32 *)&M;
		F32 *q = (F32 *)&Q.v;

		static unsigned next[] = { 1, 2, 0 };
		unsigned i = 0;
		if (m[1*4+1] > m[0*4+0]) i = 1;
		if (m[2*4+2] > m[i*4+i]) i = 2;
		unsigned j = next[i];
		unsigned k = next[j];
		w = (F32) sqrt(m[i*4+i] - m[j*4+j] - m[k*4+k] + 1.0f);
		q[i] = w * 0.5f;
		w   = 0.5f / w;
		Q.s  = (m[k*4+j] - m[j*4+k]) * w;
		q[j] = (m[j*4+i] + m[i*4+j]) * w;
		q[k] = (m[k*4+i] + m[i*4+k]) * w;
	}

	// get position
	P = M.posit;
}

// INTERPOLATE_POSITION
// Interpolate between two positions
Vector Interpolate_Position(const Vector &p0, const Vector &p1, const float t)
{
	// position to generate
	Vector p(0, 0, 0);

	// interpolate the positions
	p.x = p0.x + t * (p1.x - p0.x);
	p.y = p0.y + t * (p1.y - p0.y);
	p.z = p0.z + t * (p1.z - p0.z);

	// return the position
	return p;
}

// convert a quaternion and a position to a matrix
void QuatPos_to_Matrix(const Quaternion &Q, const Vector &P, Matrix &M)
{
	// calculate constants
	F32 n =  2.0f / (Q.v.x * Q.v.x + Q.v.y * Q.v.y + Q.v.z * Q.v.z + Q.s * Q.s);
	F32 xn = Q.v.x * n;	 F32 yn = Q.v.y * n;  F32 zn = Q.v.z * n;
	F32 sx = Q.s   * xn; F32 sy = Q.s   * yn; F32 sz = Q.s   * zn;
	F32 xx = Q.v.x * xn; F32 xy = Q.v.x * yn; F32 xz = Q.v.x * zn;
	F32 yy = Q.v.y * yn; F32 yz = Q.v.y * zn; F32 zz = Q.v.z * zn;
	
	// generate right vector
	M.right.x = 1.0f - (yy + zz);
	M.right.y =        (xy - sz);
	M.right.z =        (xz + sy);
	M.rightw  = 0.0f;
	
	// generate up vector
	M.up.x    =        (xy + sz);
	M.up.y    = 1.0f - (xx + zz);
	M.up.z    =        (yz - sx);
	M.upw     = 0.0f;

	// generate front vector
	M.front.x =        (xz - sy);
	M.front.y =        (yz + sx);
	M.front.z = 1.0f - (xx + yy);
	M.frontw  = 0.0f;

	// generate position vector
	M.posit.x = P.x;
	M.posit.y = P.y;
	M.posit.z = P.z;
	M.positw  = 1.0f;
}

// INTERPOLATE_MATRIX
// Interpolate a matrix from two matrices
// INTERPOLATE_MATRIX
// Interpolate a matrix from two matrices
Matrix Interpolate_Matrix(const Matrix &m0, const Matrix &m1, const float t)
{
	// matrix to generate
	Matrix m;

	// extract quaternion and position from first matrix
	Quaternion q0;
	Vector p0(0, 0, 0);
	Matrix_to_QuatPos(m0, q0, p0);

	// extract quaternion and position from second matrix
	Quaternion q1;
	Vector p1(0, 0, 0);
	Matrix_to_QuatPos(m1, q1, p1);

	// interpolate quaternion and position
	Quaternion q = Interpolate_Quaternion(q0, q1, t);
	Normalize_Quaternion(q);
	Vector p = Interpolate_Position(p0, p1, t);

	// store interpolated orientation
	QuatPos_to_Matrix(q, p, m);

	// return the matrix
	return m;
}

// Math stuff from Ken.
// Rotates a vector without offsetting position. 
Vector Vector_Rotate (const Matrix &M, const Vector &v)
{
	Vector out(0, 0, 0);

	out.x = M.right.x * v.x + M.up.x * v.y + M.front.x * v.z;
	out.y = M.right.y * v.x + M.up.y * v.y + M.front.y * v.z;
	out.z = M.right.z * v.x + M.up.z * v.y + M.front.z * v.z;

	return out;
}

// Rotates a vector without offsetting position, inverse.
Vector Vector_RotateInv (const Matrix &M, const Vector &v)
{
	Vector out(0, 0, 0);

	out.x = M.right.x * v.x + M.right.y * v.y + M.right.z * v.z;
	out.y = M.up.x    * v.x + M.up.y    * v.y + M.up.z    * v.z;
	out.z = M.front.x * v.x + M.front.y * v.y + M.front.z * v.z;

	return out;
}

Vector Vector_TransformInv (const Matrix &M, const Vector &v)
{
	float x = v.x - M.posit.x;
	float y = v.y - M.posit.y;
	float z = v.z - M.posit.z;

	Vector out(0, 0, 0);

	out.x = M.right.x * x + M.right.y * y + M.right.z * z;
	out.y = M.up.x * x    + M.up.y * y    + M.up.z * z;
	out.z = M.front.x * x + M.front.y * y + M.front.z * z;

	return out;
}

Vector Vector_Transform (const Matrix &M, const Vector &v)
{
	Vector out(0, 0, 0);

	out.x = M.right.x * v.x + M.up.x * v.y + M.front.x * v.z + M.posit.x;
	out.y = M.right.y * v.x + M.up.y * v.y + M.front.y * v.z + M.posit.y;
	out.z = M.right.z * v.x + M.up.z * v.y + M.front.z * v.z + M.posit.z;

	return out;
}
// Uses A * B
Matrix Matrix_Multiply(const Matrix &A, const Matrix &B)
{
	Matrix M;

	M.right.x = A.right.x * B.right.x + A.right.y * B.up.x + A.right.z * B.front.x + A.rightw * B.posit.x;
	M.right.y = A.right.x * B.right.y + A.right.y * B.up.y + A.right.z * B.front.y + A.rightw * B.posit.y;
	M.right.z = A.right.x * B.right.z + A.right.y * B.up.z + A.right.z * B.front.z + A.rightw * B.posit.z;
	M.rightw  = A.right.x * B.rightw  + A.right.y * B.upw  + A.right.z * B.frontw  + A.rightw * B.positw;

	M.up.x    = A.up.x    * B.right.x + A.up.y    * B.up.x + A.up.z    * B.front.x + A.upw * B.posit.x;
	M.up.y    = A.up.x    * B.right.y + A.up.y    * B.up.y + A.up.z    * B.front.y + A.upw * B.posit.y;
	M.up.z    = A.up.x    * B.right.z + A.up.y    * B.up.z + A.up.z    * B.front.z + A.upw * B.posit.z;
	M.upw     = A.up.x    * B.rightw  + A.up.y    * B.upw  + A.up.z    * B.frontw  + A.upw * B.positw;

	M.front.x = A.front.x * B.right.x + A.front.y * B.up.x + A.front.z * B.front.x + A.frontw * B.posit.x;
	M.front.y = A.front.x * B.right.y + A.front.y * B.up.y + A.front.z * B.front.y + A.frontw * B.posit.y;
	M.front.z = A.front.x * B.right.z + A.front.y * B.up.z + A.front.z * B.front.z + A.frontw * B.posit.z;
	M.frontw  = A.front.x * B.rightw  + A.front.y * B.upw  + A.front.z * B.frontw  + A.frontw * B.positw;

	M.posit.x = A.posit.x * B.right.x + A.posit.y * B.up.x + A.posit.z * B.front.x + A.positw * B.posit.x;
	M.posit.y = A.posit.x * B.right.y + A.posit.y * B.up.y + A.posit.z * B.front.y + A.positw * B.posit.y;
	M.posit.z = A.posit.x * B.right.z + A.posit.y * B.up.z + A.posit.z * B.front.z + A.positw * B.posit.z;
	M.positw  = A.posit.x * B.rightw  + A.posit.y * B.upw  + A.posit.z * B.frontw  + A.positw * B.positw;

	//Matrix_Sanity_Check (&M);

	return M;
}

// MATRIX_INVERSE
// Build the inverse of a matrix
Matrix Matrix_Inverse (const Matrix &M)
{
	Matrix I;

	I.right.x = M.right.x;
	I.right.y = M.up.x;
	I.right.z = M.front.x;
	I.rightw  = 0.0f;
	I.up.x    = M.right.y;
	I.up.y    = M.up.y;
	I.up.z    = M.front.y;
	I.upw     = 0.0f;

	I.front.x = M.right.z;
	I.front.y = M.up.z;
	I.front.z = M.front.z;
	I.frontw  = 0.0f;

	I.posit.x = - (
	M.posit.x * M.right.x +
	M.posit.y * M.right.y +
	M.posit.z * M.right.z
	);
	I.posit.y = - (
	M.posit.x * M.up.x +
	M.posit.y * M.up.y +
	M.posit.z * M.up.z
	);
	I.posit.z = - (
	M.posit.x * M.front.x +
	M.posit.y * M.front.y +
	M.posit.z * M.front.z
	);
	I.positw  = 1.0f;

	return I;
}

Matrix Build_Orthogonal_Matrix (const Vector &Up, const Vector &Heading, const Vector &Position)
{
	Matrix M;

	M.up      = Normalize_Vector (Up);
	M.upw     = 0.0f;

	M.right   = Cross_Product(M.up, Heading);
	M.right   = Normalize_Vector (M.right);
	M.rightw  = 0.0f;

	M.front   = Cross_Product(M.right, M.up);
	M.frontw  = 0.0f;

	M.posit = Position;
//	M.posit.x = 0.0f;
//	M.posit.y = 0.0f;
//	M.posit.z = 0.0f;
	M.positw  = 1.0f;

	return M;
}

// BUILD_YAW_MATRIX
// Builds a matrix with only yaw (Y axis) rotation
// (yaw is in radians)
Matrix Build_Yaw_Matrix(const float Yaw, const Vector &Position)
{
	// get sine and cosine of yaw angle
	float s = portable_sin(Yaw), c = portable_cos(Yaw);

	// construct [Yaw] matrix
	Matrix M;
	M.right.x = c;
	M.right.y = 0.0f;
	M.right.z = -s;
	M.rightw  = 0.0f;

	M.up.x    = 0.0f;
	M.up.y    = 1.0f;
	M.up.z    = 0.0f;
	M.upw     = 0.0f;

	M.front.x = s;
	M.front.y = 0.0f;
	M.front.z = c;
	M.frontw  = 0.0f;

	M.posit = Position;
//	M.posit.x = 0.0f;
//	M.posit.y = 0.0f;
//	M.posit.z = 0.0f;
	M.positw  = 1.0f;

	return M;
}

// BUILD_POSITION_ROTATION_MATRIX
// Compute the matrix [M] = [Roll]*[Pitch]*[yaw]*[Tr]
Matrix Build_Position_Rotation_Matrix (const float pitch, const float yaw, const float roll, const Vector &Position)
{
	// compute sine and cosine of each angle
	float rs = portable_sin(roll), rc = portable_cos(roll);
	float ps = portable_sin(pitch), pc = portable_cos(pitch);
	float ys = portable_sin(yaw), yc = portable_cos(yaw);

	// build [Roll] * [Pitch] * [Yaw] matrix
	Matrix M;
	M.right.x = rs * ys * ps + rc * yc;
	M.right.y = rs * pc;
	M.right.z = rs * yc * ps - rc * ys;
	M.rightw  = 0.0f;

	M.up.x    = rc * ys * ps - rs * yc;
	M.up.y    = rc * pc;
	M.up.z    = rc * yc * ps + rs * ys;
	M.upw     = 0.0f;

	M.front.x = ys * pc;
	M.front.y = -ps;
	M.front.z = yc * pc;
	M.frontw  = 0.0f;

	M.posit = Position;
//	M.posit.x = x;
//	M.posit.y = y;
//	M.posit.z = z;
	M.positw  = 1.0f;

	return M;
}

Matrix Build_Axis_Rotation_Matrix (const float Angle, const Vector &Axis)
{
   const Quaternion Q(cosf(Angle * 0.5f), Vector_Scale(Axis, sinf(Angle * 0.5f)));
   
   Matrix M;
   QuatPos_to_Matrix(Q, Vector(0, 0, 0), M);
   return M;
}

int UnsignedToSigned(unsigned int x) 
{ 
	if (x <= INT_MAX)
		return static_cast<int>(x);

	//if (x >= INT_MIN)
	#pragma warning( push )
	#pragma warning( disable : 4308)
	if (x > INT_MIN - 1u) // MS compiler likes this more
	#pragma warning( pop )
		return static_cast<int>(x - INT_MIN) + INT_MIN;

	throw x; // Or whatever else you like
}

int GetRandomInt(const int Min, const int Max)
{
	if(Max < Min)
		return Min;

	//Math.Min(Math.floor(GetRandFloat(Max)),Max-1);
	int value = Min + int(GetRandomFloat(float(Max - Min + 1)));
	value = clamp(value, Min, Max);

	return value;
}

float GetRandomFloat(const float Min, const float Max)
{
	if(Max < Min)
		return Min;

	float value = Min + GetRandomFloat(Max - Min + 1.0f);
	value = clamp(value, Min, Max);

	return value;
}