#include "..\..\source\fun3d\ScriptUtils.h"

#include "DLLBase.h"
#include "..\Shared\BZ1Helper.h" //Add in BZ1 Stuff. -GBD

#include <crtdbg.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

//#define CENTIGREES_TO_RADIANS 0.0001745329f
//#define MAX_MINES 64 // number of mines to track. SP only.

class SPMission : public BZ1Helper {
public:
	SPMission(void);

	virtual void Setup(void) = 0;
	virtual void Execute(void);
	virtual void AddObject(Handle h);
	virtual void DeleteObject(Handle h);
	virtual void ProcessCommand(unsigned long crc)
	{
	}
	bool Save(bool missionSave);
	virtual bool Load(bool missionSave);
	bool PostLoad(bool missionSave);

	virtual void PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF);

	/*
	// Camera Pos function from Bob "BS-er" Stewert, moves the camera between two positions at the incrument speed.
	bool CameraPos(Handle me, Handle him, Vector &PosA, Vector &PosB, float Increment);
	// Code from WhosDr, Camera function that 
	bool CameraCircle(Handle me, Vector Offset, Handle him, Vector Speed);
	Vector FixRotation(Vector in); // Used in cameraCircle calculations.
	*/

	bool *b_array;
	int b_count;

	float *f_array;
	int f_count;

	int *h_array;
	int h_count;

	int *i_array;
	int i_count;
};

//#define IsOdf2(h,name) (_strnicmp(ODFName,name,strlen(name)) == 0)


// INTERPOLATE_MATRIX
// Interpolate a matrix from two matrices
//Matrix Interpolate_Matrix(const Matrix &m0, const Matrix &m1, const float t);

//Matrix Build_Directinal_Matrix (const Vector &Position, const Vector &Direction);
