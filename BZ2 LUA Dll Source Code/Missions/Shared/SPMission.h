#include "..\..\source\fun3d\ScriptUtils.h"

#include "DLLBase.h"

#include <crtdbg.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

class SPMission : public DLLBase {
public:
	SPMission(void);

	virtual void Setup(void) = 0;
	virtual void Execute(void) = 0;
	virtual void AddObject(Handle h)
	{
	}
	virtual void DeleteObject(Handle h)
	{
	}
	virtual void ProcessCommand(unsigned long crc)
	{
	}
	virtual EjectKillRetCodes PlayerEjected(void);
	virtual EjectKillRetCodes ObjectKilled(int DeadObjectHandle, int KillersHandle);
	bool Save(bool missionSave);
	virtual bool Load(bool missionSave);
	bool PostLoad(bool missionSave);

	bool *b_array;
	int b_count;

	float *f_array;
	int f_count;

	int *h_array;
	int h_count;

	int *i_array;
	int i_count;
};

#define IsOdf2(h,name) (_strnicmp(ODFName,name,strlen(name)) == 0)


// INTERPOLATE_MATRIX
// Interpolate a matrix from two matrices
Matrix Interpolate_Matrix(const Matrix &m0,
						  const Matrix &m1,
						  const float t);

Matrix Build_Directinal_Matrix (const Vector &Position, 
								const Vector &Direction);
