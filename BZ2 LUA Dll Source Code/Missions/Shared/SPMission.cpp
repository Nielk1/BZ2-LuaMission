#include "SPMission.h"
#include <math.h>

SPMission *mission;

EjectKillRetCodes SPMission::PlayerEjected(void)
{
	return DoEjectPilot;
}

EjectKillRetCodes SPMission::ObjectKilled(int DeadObjectHandle,int KillersHandle)
{
	if(!IsPlayer(DeadObjectHandle)) // J.Random AI-controlled Object is toast
		return DLLHandled;
	else // Player dead
		return DoEjectPilot;
}

SPMission::SPMission(void)
{
	b_array = NULL;
	b_count = 0;

	f_array = NULL;
	f_count = 0;

	h_array = NULL;
	h_count = 0;

	i_array = NULL;
	i_count = 0;	
}

bool SPMission::Load(bool missionSave)
{
	if (missionSave) {
		int i;

		// init bools
		for (i = 0; i < b_count; i++)
			b_array[i] = false;

		// init floats
		for (i = 0; i < f_count; i++)
			f_array[i] = 99999.0f;

		// init handles
		for (i = 0; i < h_count; i++)
			h_array[i] = 0;

		// init ints
		for (i = 0; i < i_count; i++)
			i_array[i] = 0;

		Setup();
		return true;
	}

	bool ret = true;

	// bools
	if(b_count)
		Read(b_array, b_count);

	// floats
	if(f_count)
		Read(f_array, f_count);

	// Handles
	if(h_count)
		Read(h_array, h_count);

	// ints
	if(i_count)
		Read(i_array, i_count);

	return ret;
}

bool SPMission::PostLoad(bool missionSave)
{
	if (missionSave)
		return true;

	bool ret = true;

	ConvertHandles(h_array, h_count);

	return ret;
}

bool SPMission::Save(bool missionSave)
{
	if (missionSave)
		return true;

	bool ret = true;

	// bools
	if(b_count)
		Write(b_array, b_count);

	// floats
	if(f_count)
		Write(f_array, f_count);

	// Handles
	if(h_count)
		Write(h_array, h_count);

	// ints
	if(i_count)
		Write(i_array, i_count);

	return ret;
}

Vector Normalize_Vector (const Vector &A)
{
	double scale = 
		1.0 / sqrt (A.x * A.x + A.y * A.y + A.z * A.z);
	
	Vector Dst;
	Dst.x = (float) (A.x * scale);
	Dst.y = (float) (A.y * scale);
	Dst.z = (float) (A.z * scale);
	
	return Dst;
}

// compute the cross product of two vectors
Vector Cross_Product (const Vector &A, const Vector &B)
{
	Vector Dst;
	Dst.x = A.y * B.z - A.z * B.y;
	Dst.y = A.z * B.x - A.x * B.z;
	Dst.z = A.x * B.y - A.y * B.x;
	return Dst;
}

// Create a matrix with its front vector aimed in a given direction
Matrix Build_Directinal_Matrix (const Vector &Position,
								const Vector &Direction)
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




// INTERPOLATE_QUATERNION
// Interpolate between two quaternions

struct Quaternion 
{
	float s;
	Vector v;
};

#define EPSILON	0.001f
#define HUGE_NUMBER  (1.0f/EPSILON)

static Quaternion Interpolate_Quaternion(
	const Quaternion &q0,
	const Quaternion &q1,
	const float t)
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
	q.s   = (float)(scale0 * q0.s   + scale1 * q.s  );
	q.v.x = (float)(scale0 * q0.v.x + scale1 * q.v.x);
	q.v.y = (float)(scale0 * q0.v.y + scale1 * q.v.y);
	q.v.z = (float)(scale0 * q0.v.z + scale1 * q.v.z);

	// return the quaternion
	return q;
}

static Quaternion Normalize_Quaternion (const Quaternion &q)
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
	Q.s   = (float)(q.s   * dot);
	Q.v.x = (float)(q.v.x * dot);
	Q.v.y = (float)(q.v.y * dot);
	Q.v.z = (float)(q.v.z * dot);
	return Q;
}



// convert a matrix to a quaternion and a position
static void Matrix_to_QuatPos(const Matrix &M, Quaternion &Q, Vector &P)
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
static Vector Interpolate_Position(const Vector &p0,
							const Vector &p1,
							const float t)
{
	// position to generate
	Vector p;

	// interpolate the positions
	p.x = p0.x + t * (p1.x - p0.x);
	p.y = p0.y + t * (p1.y - p0.y);
	p.z = p0.z + t * (p1.z - p0.z);

	// return the position
	return p;
}

// convert a quaternion and a position to a matrix
static void QuatPos_to_Matrix(const Quaternion &Q, const Vector &P, Matrix &M)
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
Matrix Interpolate_Matrix(
	const Matrix &m0,
	const Matrix &m1,
	const float t)
{
	// matrix to generate
	Matrix m;

	// extract quaternion and position from first matrix
	Quaternion q0;
	Vector p0;
	Matrix_to_QuatPos(m0, q0, p0);

	// extract quaternion and position from second matrix
	Quaternion q1;
	Vector p1;
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





