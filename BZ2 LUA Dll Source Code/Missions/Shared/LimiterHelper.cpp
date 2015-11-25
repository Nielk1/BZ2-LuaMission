#include "BZ1Helper.h"
#include "LimiterHelper.h"
#include "SatelliteHelper.h"
#include "BZCScriptUtils.h"

LimiterHelper::LimiterHelper()
{
}

void LimiterHelper::AddLimiterObject(const Handle h, const int Team, const char *ODFName)
{
	if(ODFName == NULL)
		return;

	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

	// Ensure there's only one limiter objects per team.
	if(_strnicmp(ODFName,"BZClimitoffense",15) == 0) 
	{
		Handle Limiter = teamInfo.LimiterObject[0];
		//if((IsAround(LimiterObject[Team][0])) && (LimiterObject[Team][0] != h))
		if((IsAround(Limiter)) && (Limiter != h))
		{
			RemoveObject(Limiter);
			teamInfo.LimiterObject[0] = h;
		}
	}
	else if(_strnicmp(ODFName,"BZClimitdefense",15) == 0) 
	{
		Handle Limiter = teamInfo.LimiterObject[1];
		if((IsAround(Limiter)) && (Limiter != h))
		{
			RemoveObject(Limiter);
			teamInfo.LimiterObject[1] = h;
		}
	}
	else if(_strnicmp(ODFName,"BZClimitutility",15) == 0) 
	{
		Handle Limiter = teamInfo.LimiterObject[2];
		if((IsAround(Limiter)) && (Limiter != h))
		{
			RemoveObject(Limiter);
			teamInfo.LimiterObject[2] = h;
		}
	}
}

void LimiterHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging;

	for(int Team = 0; Team < MAX_TEAMS; Team++)
	{
		BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

		// If we're using limits...
		if(teamInfo.m_UseLimits)
		{
			int ProvideCount = 0;
			char ProvideName[16][MAX_ODF_LENGTH] = {0};

			char PlayerProvideName[MAX_TEAMS][MAX_ODF_LENGTH] = {0};
		//	sprintf_s(PlayerProvideName[Team], "No Provide For Team %d", Team); // Fill this with SOMETHING, or we get The Matrix in the Console. -GBD

			if(IsAround(GetPlayerHandle(Team)))
			{
				// If you are NOT the same ship you used to be?
				if(!IsPlayer(teamInfo.LimitPlayer))
				{
					// Set this here!
					if((IsAround(teamInfo.SatellitePlayer) && (IsAround(teamInfo.Player)))) // Satellite is on! look at Player, not RealPlayer.
						teamInfo.LimitPlayer = teamInfo.Player;
					else
						teamInfo.LimitPlayer = GetPlayerHandle(Team);

					Handle TempLimitPlayer = teamInfo.LimitPlayer;

					char TempODF1[MAX_ODF_LENGTH] = {0};
					char TempODF2[MAX_ODF_LENGTH] = {0};
					GetAndOpenODFNames(TempLimitPlayer, TempODF1, TempODF2);

					if(Logging)
						FormatLogMessage("COWBOY BEEBOP! Player is: %08x", TempLimitPlayer);

					if(GetODFInt(TempODF1, TempODF2, "GameObjectClass", "provideCount", &ProvideCount))
					{
						if(Logging)
							FormatLogMessage("Player ProvideCount is: %d", ProvideCount);

						// If provide count is not = 0, look at them.
						for (int j = 0; j < ProvideCount; j++)
						{
							char DesiredValue[MAX_ODF_LENGTH] = {0};
							sprintf_s(DesiredValue, "provideName%d", j+1);
							// Grab the ODF names.
							if(GetODFString(TempODF1, TempODF2, "GameObjectClass", DesiredValue, MAX_ODF_LENGTH, ProvideName[j]))
							{
								if(Logging)
									FormatLogMessage("Player's PrivdeName%d Is: %s", j+1, ProvideName[j]); 
								
								if( (_stricmp(ProvideName[j],"offense") == 0) || // If it's offense, we found one, stop the loop.
									(_stricmp(ProvideName[j],"defense") == 0) || // If it's defense, we found one, stop the loop.
									(_stricmp(ProvideName[j],"utility") == 0)) // If it's utility, we found one, stop the loop.
								{
									strcpy_s(PlayerProvideName[Team], ProvideName[j]);
									break;
								}
							}
						}
					}

					if(Logging)
						FormatLogMessage("Chosen PrivdeName Is: %s", PlayerProvideName[Team]); 

					switch (Hash(PlayerProvideName[Team]))
					{
					case 0x0d558691: /* "offense" */
						if(IsAround(teamInfo.LimiterObject[0]))
							RemoveObject(teamInfo.LimiterObject[0]);
						if(!IsAround(teamInfo.LimiterObject[1]))
							teamInfo.LimiterObject[1] = BuildObject("BZClimitdefense",Team,BZ1Helper::Where);
						if(!IsAround(teamInfo.LimiterObject[2]))
							teamInfo.LimiterObject[2] = BuildObject("BZClimitutility",Team,BZ1Helper::Where);

						if(Logging)
							FormatLogMessage("Created Defense + Utility");

						break;
					case 0x761a8c53: /* "defense" */
						if(IsAround(teamInfo.LimiterObject[1]))
							RemoveObject(teamInfo.LimiterObject[1]);
						if(!IsAround(teamInfo.LimiterObject[0]))
							teamInfo.LimiterObject[0] = BuildObject("BZClimitoffense",Team,BZ1Helper::Where);
						if(!IsAround(teamInfo.LimiterObject[2]))
							teamInfo.LimiterObject[2] = BuildObject("BZClimitutility",Team,BZ1Helper::Where);

						if(Logging)
							FormatLogMessage("Created Offense + Utility");

						break;
					case 0xbfc6b95f: /* "utility" */
						if(IsAround(teamInfo.LimiterObject[2]))
							RemoveObject(teamInfo.LimiterObject[2]);
						if(!IsAround(teamInfo.LimiterObject[0]))
							teamInfo.LimiterObject[0] = BuildObject("BZClimitoffense",Team,BZ1Helper::Where);
						if(!IsAround(teamInfo.LimiterObject[1]))
							teamInfo.LimiterObject[1] = BuildObject("BZClimitdefense",Team,BZ1Helper::Where);

						if(Logging)
							FormatLogMessage("Created Offense + Defense");

						break;
					default:
						if(!IsAround(teamInfo.LimiterObject[0]))
							teamInfo.LimiterObject[0] = BuildObject("BZClimitoffense",Team,BZ1Helper::Where);
						if(!IsAround(teamInfo.LimiterObject[1]))
							teamInfo.LimiterObject[1] = BuildObject("BZClimitdefense",Team,BZ1Helper::Where);
						if(!IsAround(teamInfo.LimiterObject[2]))
							teamInfo.LimiterObject[2] = BuildObject("BZClimitutility",Team,BZ1Helper::Where);

						if(Logging)
							FormatLogMessage("Created Offense, Defense, and Utility");

						break;
					}
				} // End player ship is different then it used to be.
			}
			else // No players around on this team, Remove old limiters.
			{
				for(int i = 0; i < 3; i++)
					if(IsAround(teamInfo.LimiterObject[i]))
						RemoveObject(teamInfo.LimiterObject[i]);
			}
		}
	}
}