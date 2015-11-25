#ifndef _PortalHelper_
#define _PortalHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class PortalHelper
{
private:

	static PortalHelper *m_PortalHelper;

	struct PortalClass {
		Handle TeleporterObject; // This Teleporter Object.
		Handle TeleporterDestination; // This Teleporter's Destination Portal.
		Handle TeleporterEffect; // Handle for this portal's Effect generator.
		bool IsRandom; // Teleporter Is Random?
		bool RandomUsesChannels; // Does Random Use Channels?
		bool CanEnter; // Can this Teleporter be entered?
		bool CanExit; // Can this Teleporter be exited?
		bool ExitForwardOnly; // Ships exit Forward facing only?
		bool ShowChannel; // Show Channel in Name?
		bool NameFlash; // Bool for toggling name flash back and forth.
		bool UseCenter; // Use center or Tap location?
		bool KickAI; // Kick out AI reguardless of entry velocity?
		bool ContinuousFx; // Flag for if this portal uses a continous effect while "connected".
		int Channel; // Teleporter Channel #.
		int DestTeamFilter; // Team Filter for Destination Locking.
		int TargetTeamFilter; // Team Filter for What to teleport.
		int TeleportFilter; // Filter what types can be teleported, (vehicles, pilots, both).
		int EffectType; // Effect Type. 0 = None? (One Way), 1 = Continuous On, 2 = Build.
		int EffectNumber; // Which Effect number this is currently using.
		int EffectState; // Effect state. 0 = Off, 1 = Opening, 2 = On, 3 = Closing.
		int EffectTimer; // Remaining time to switch or remove the Effect generator.
		float EnterRange; // Range for things to be teleported.
		float ExitSpeed; // How fast things exit, if forced out.
	//	Matrix DestinationPosition; // Destination Position (rotated matrix)
		Vector Position; // This portal's position.

		Matrix ExitPoint; // Point things come out.
		Matrix GotoPoint; // Goto nav? (imaginary)
		int DestinationIndex; // Index for which Portal object we're locked onto.

		int InEffect; // Which Effect is used for Entering the portal. (during normal operations)
		int OutEffect; // Which Effect is used for Exiting the portal. (during normal operations)
		int OnEffect; // Which Effect is used for ContinousFx. (during normal operations)
		float ContinuousFxRange; // Distance from which a ship must be for this portal Effect to activate.

		char TeleporterName[MAX_ODF_LENGTH]; //std::string<>; // Save it's name for NameFlash.
	//	char EffectIn[MAX_ODF_LENGTH]; // Effect to spawn for Entering Portal.
	//	char EffectOut[MAX_ODF_LENGTH]; // Effect to spawn for Exiting Potral.
	//	char EffectVeh[MAX_ODF_LENGTH]; // Effect to spawn at the vehicle's location when Teleporting.
		char EffectBaseODF[MAX_ODF_LENGTH]; // Effect to spawn for Portal. Modified at runtime.

		PortalClass() { memset(this, 0, sizeof(PortalClass)); };
	};
	std::vector<PortalClass> PortalList;
	//static bool ShouldRemovePortal(const PortalClass &entry) { return !IsAround(entry.TeleporterObject); }

	struct TeleportTargetClass {
		Handle TeleportTarget; // This Teleported Object.
		Handle TeleportTargetDestination; // This Teleported Object's Destination Portal.
		float DestEnterRange; // Entrance range, used to tell if this object has left range and is removed from teleporter target list.
		float DistanceFromDestination; // Save its distance so we can remove it sanely.

		TeleportTargetClass() { memset(this, 0, sizeof(TeleportTargetClass)); };
	};
	std::vector<TeleportTargetClass> TeleportTargetList;
	static bool ShouldRemoveTeleportTarget(const TeleportTargetClass &entry) { return entry.DistanceFromDestination > entry.DestEnterRange+5.0f || !IsAround(entry.TeleportTarget) || !IsAround(entry.TeleportTargetDestination); }

public:
	PortalHelper();
	~PortalHelper();
	static PortalHelper &Get() { return *m_PortalHelper; }

	void AddPortalObject(const Handle h, const char* TempODF1, const char* TempODF2);
	void AddPortalSwitchObject(const Handle h, const char* TempODF1, const char* TempODF2);

	// Sets and Gets a portal's channel. Get returns -1 if not a valid portal object.
	void SetPortalChannel(const Handle h, const int Channel);
	int GetPortalChannel(const Handle h);

	// Portal Effect controllers. FxNumber is which BuildODF to make, 0, 1, or 2. Type: 0 = None? Oneway? 1 = Continuous On, 2 = Build (Open then Close)
	void SetPortalEffectStart(const Handle h, const int FxNumber = 0, const int Type = 0);
	void SetPortalEffectOn(const Handle h, const int FxNumber = 0, const int Type = 0);
	void SetPortalEffectEnd(const Handle h, const int FxNumber = 0, const int Type = 0);

	bool IsPortalEffectActive(const Handle h);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
	void ExecuteHandle(int HandleListIndex);
};

#endif
