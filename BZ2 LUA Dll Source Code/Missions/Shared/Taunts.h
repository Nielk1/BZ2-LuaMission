#ifndef _TAUNTS_H_
#define _TAUNTS_H_

enum TauntTypes {
	TAUNTS_GameStart,
	TAUNTS_NewHuman,
	TAUNTS_LeftHuman,
	TAUNTS_HumanShipDestroyed,
	TAUNTS_HumanPilotKilled,
	TAUNTS_HumanRecyDestroyed,
	TAUNTS_CPURecyDestroyed,
	TAUNTS_HumanRecyclerDeployed,
	TAUNTS_HumanScavDestroyed,
	TAUNTS_CPUScavDestroyed,
	TAUNTS_CPUSniped,
	TAUNTS_HumanShipStolen,
	TAUNTS_CPUShipStolen,
	TAUNTS_Random,
	TAUNTS_MAX,
};

// Required setup call before you call DoTaunt(). pGameTime is used to
// squelch too-often messages. [It should be the # of calls to
// Execute]. pLastMessagePrintedAt is a scratchpad variable used to
// note things, should be saved. pTPS should be a pointer to the TPS
// in use. aCPUTeamName is an override for that name; if specified, it
// won't query network.session.svar2 for it.
void InitTaunts(int* pGameTime, int* pLastMessagePrintedAt, int* pTPS, const char* pCPUTeamName = NULL);

// Prints a taunt from the specified category. 
void DoTaunt(TauntTypes Taunt, int Team = 0);

#endif // _TAUNTS_H_
