#include "BZ1Helper.h"
#include "QuakeHelper.h"
#include "BZCScriptUtils.h"

QuakeHelper::QuakeHelper()
{
	f_count = &f_last - &f_first - 1;
	f_array = &f_first + 1;

	if(f_array)
		memset(f_array, 0, f_count*sizeof(float));
}

bool QuakeHelper::Load(bool missionSave)
{
	if (missionSave) {
		int i;

		// init floats
		if((f_array) && (f_count))
			for (i = 0; i < f_count; i++)
				f_array[i] = 0.0f;

		return true;
	}

	bool ret = true;

	// floats
	if (f_array != NULL)
		ret = ret && Read(f_array, f_count);

	return ret;
}

bool QuakeHelper::Save(bool missionSave)
{
	if (missionSave)
		return true;

	bool ret = true;

	// floats
	if (f_array != NULL)
		ret = ret && Write(f_array, f_count);

	return ret;
}

void QuakeHelper::AddQuakeObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	int QuakeTime = 0;
	// Look for custom object to drop via DayWrecker.
	float TempQuakeTime = 0.0f;
	GetODFFloat(TempODF1, TempODF2, "QuakeBombClass", "quakeTime", &TempQuakeTime, 0.0f);
	QuakeTime = ConvertToTurns(TempQuakeTime);

	if((QuakeTime > 0) && (!m_QuakeMagnitude))
	{
		// Look for magnitude.
		GetODFFloat(TempODF1, TempODF2, "QuakeBombClass", "quakeMagnitude", &m_QuakeMagnitude, 5.0f);

		m_QuakeDecay = m_QuakeMagnitude / QuakeTime; // Calculate time to falloff from this quake.

		m_QuakeMagnitude += m_MinQuakeMagnitude; // Add it to existing QuakeMagnitiude if we have a quake going on.
		StartEarthQuake(m_QuakeMagnitude);
	}
}

void QuakeHelper::DeleteQuakeObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team)
{
	// Day Wreckers, if they're there, make them explode with force. -GBD
	Vector TempExplosionPos(0, 0, 0);

	GetPosition2(h,TempExplosionPos); // Get their position.
	float HeightThreshold = 0.0f;
	GetODFFloat(TempODF1, TempODF2, "DayWreckerClass", "HeightThreshold", &HeightThreshold, 2.25f);

	//FormatLogMessage("Height Difference is: %f, Health is: %f", (TempExplosionPos.y - TerrainFindFloor(TempExplosionPos.x, TempExplosionPos.z)), GetHealth(h));

	if(TempExplosionPos.y - TerrainFindFloor(TempExplosionPos.x, TempExplosionPos.z) < HeightThreshold)
	{
		char ExplosionODF[MAX_ODF_LENGTH] = {0};
		// Look for custom object to drop via DayWrecker.
		GetODFString(TempODF1, TempODF2, "DayWreckerClass", "quakeObject", MAX_ODF_LENGTH, ExplosionODF);

		// If it lands on the ground, explosion position may be below ground slightly, compensate. 
		if(TerrainFindFloor(TempExplosionPos.x, TempExplosionPos.z) > TempExplosionPos.y)
			TempExplosionPos.y = TerrainFindFloor(TempExplosionPos.x, TempExplosionPos.z);

		if(DoesODFExist(ExplosionODF))
			BuildObject(ExplosionODF,Team,TempExplosionPos);
		else
			BuildObject("BZCwrckxpl",Team,TempExplosionPos);

	}
	/*
	Vector Normal;
	float Height = 0;
	TerrainGetHeightAndNormal(TempExplosionPos, Height, Normal, true);
	FormatLogMessage("Day Wrecker Explosion: %f, %f, %f", TempExplosionPos.x, TempExplosionPos.y, TempExplosionPos.z);
	FormatLogMessage("GroundHeight Position: %f, %f, %f", TempExplosionPos.x, TerrainFindFloor(TempExplosionPos.x, TempExplosionPos.z), TempExplosionPos.z);
	FormatLogMessage("Get THANorm Explosion: %f, %f, %f", TempExplosionPos.x, Height, TempExplosionPos.z);
	*/

//	if(DoesFileExist("gquak01.wav"))
//		AudioMessage("gquak01.wav"); // Play this for everyone, no matter how far away.

}

void QuakeHelper::Execute(BZ1Helper &bz1Helper)
{
	if(m_QuakeMagnitude > m_MinQuakeMagnitude)
		m_QuakeMagnitude -= m_QuakeDecay; //(m_QuakeMagnitude / m_QuakeTime);
	else
		m_QuakeMagnitude = m_MinQuakeMagnitude; // Make sure we don't go below our minimum.

	if(m_QuakeMagnitude > 0)
	{
		UpdateEarthQuake(m_QuakeMagnitude);
	}
	else
	{
		StopEarthQuake();
		m_QuakeMagnitude = 0.0f;
	}
}

// Quake functions that properly modulate the quake made by DayWreckers.
void QuakeHelper::DLLStartEarthQuake(const float Magnitude)
{
	// If we're already quaking, only update magnitude.
	if(!m_QuakeMagnitude)
	{
		StartEarthQuake(Magnitude);
		m_MinQuakeMagnitude = Magnitude;
	}
	else
	{
		DLLUpdateEarthQuake(Magnitude);
	}
}

// Updates the current minimum Quake, and updates game's quake if the existing quake from a daywrecker is less then the new magnitude.
void QuakeHelper::DLLUpdateEarthQuake(const float Magnitude)
{
	if(m_QuakeMagnitude > 0)
	{
		if(m_QuakeMagnitude > m_MinQuakeMagnitude) // We're currently doing a DayQuake from a day wrecker.
			m_QuakeMagnitude = (Magnitude + (m_QuakeMagnitude - m_MinQuakeMagnitude)); // Add the difference to our new Quake amount.
	}
	else // New behavior, if we're not quaking, start one.
	{
		DLLStartEarthQuake(Magnitude);
	}

	m_MinQuakeMagnitude = Magnitude; // Save our new Magnitude.
}

// Shut it down!
void QuakeHelper::DLLStopEarthQuake(void)
{
	m_MinQuakeMagnitude = 0.0f;
	StopEarthQuake();
}
