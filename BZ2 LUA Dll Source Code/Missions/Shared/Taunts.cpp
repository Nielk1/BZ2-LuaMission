//#include "..\shared\SubMission.h"
#include "..\..\source\fun3d\ScriptUtils.h"
#include "BZCScriptUtils.h"
#include "Taunts.h"
#include <string.h>

// Ain't data-driven code a great way to go? Just dump entries in the following arrays, making sure there's a comma at the end of each
// line.  The code later figures out all array sizes automatically, and randomizes from in there.

static const char *GameStartTaunts[] = // cat 1
{
	"Greetings, Prepare to Die",
	"It's only a matter of time...",
	"This is my map, get out!",
	"This is only the beginning...",
	"Thanks for adding me, master...",
	"Annihilation is all but avoidable!",
	"Total Annihilation!",
	"My favorite video game is 'Kill All Humans', Lets play...",
	"Good luck, my friends...",
	"I hope you're experienced...",
	"Welcome to your death, humans!",
	"All roads lead to death, choose wisely.",
	"You inferior beings, why do you challenge me?",
	"I'm not ready yet, but oh well.",
};

static const char *HumanEntersTaunts[] =  // cat 2
{
	"Another human?",
	"Why do humans insist on joining?  Why can't I get a friendly AI?",
	"Humans. Humans everywhere...",
	"We AI are greatly unappriciated...",
	"Just another person to kill.",
	"Welcome, new victim...",
	"Another victim entered, prepare to feel my wrath!",
	"Greetings... young one...",
	"Welcome to school, hope you learn something while you're here.",
	"New Target: Accepted.",
	"Oh, a newbie, this'll be fun!",
	"I too, have friends.",
	"Oh no, not another one! Get me a flyswatter!",
	"Congratulations, you're just in time to die."
};

static const char *HumanLeavesTaunts[] =  // cat 3
{
	"The Furies put up more of a fight than you!",
	"One less human to deal with :),"
	"Now that they're gone, I can push forward.",
	"ERROR 404: Human not found.",
	"Awww, did I scare him? :)",
	"...And he's outta here!",
	"Strike!",
	"This improves my chances 10-fold.",
	"Aww.. they'll never get their Battlezone diploma.",
	"Pfft...",
	"Awww, I was having fun killing him.",
	"He's going to go play singleplayer I bet.",
	"Another one bites the dust!",
};

static const char *HumanShipDestroyedTaunts[] =  // cat 4
{
	"Another one bites the dust!",
	"Puny human...",
	"Oh, I'm sorry...Were you fond of that ship?",
	"I think you lost something...",
	"Was that your destroyed tank I just saw?",
	"Have a nice flight!",
	"Look down, there might just be a missile there >:D",
	"Good luck searching for your wreckage!",
	"Looks like that'll cost you some scrap.",
	"Well, at least you're still flying half a ship.. oh wait...",
	"Where'd you get your piloting skills?  From a cereal box?",
	"Another infraction against your pilots license.",
	"Did I hit too hard >:D?",
	"What do you think about when you're floating up there?",
	"You're going to need a bigger tank.",
};

static const char *HumanPilotKilledTaunts[] =  // cat 5
{
	"Bye Bye!",
	"Only Human...",
	"Doesn't it suck to be Mortal?",
	"That looked like it hurt.",
	"Did a fury get you >:D?",
	"Did your suit spring a leak? I'm sorry...",
	"I got one! I got one!",
	"I'm sure glad I don't have a body.",
	"ERROR: Human not found: KIA?",
	"I'll send your family a nice letter, telling them that you failed.",
	"Can I have his helmet?",
	"Ouch, hope you still have some lives left.",
	"Look out, he has a sniper rifle!",
};

static const char *HumanRecyclerDestroyedTaunts[] = // cat 6
{
	"What ever will you do now?",
	"He shoots, He scores!",
	"MISSION FAILED!     Oh? right, your still here...",
	"I wonder how much those things cost...",
	"Whatcha gonna do about it!?",
	"Looks like you gotta find another way to kill me!",
	"Good Luck, humans!",
	"I hope you have scavengers left...",
	"Now it's all but inevitable...",
	"That sniper rifle won't help you now!",
	"Your scavengers are my next priority",
	"Are you going to retreat to an ally now?",
	"What a pretty explosion...",
	"Oops, what that your recycler that just blew up? :)",
	"It can only be attributable to human error.",
};

static const char *CPURecyclerDestroyedTaunts[] =  // cat 7
{
	"No, this can not be!",
	"Humans, why must you insist on destroying me?  Can't we all get along?",
	"This is why I hate humans, they're constantly destroying things...",
	"It's okay, I'll just get another one in the next drop ship.",
	"Thanks, now I have to figure out another way to kill you.",
	"Grrr...",
	"This will only be a minor setback...",
	"It's ok, I still have enough units to destroy you.",
	"I got another one, somewhere...",
	"Oops...",
	"Doh!",
};

static const char *HumanRecyclerDeployedTaunts[] = // cat 8
{
	"Oi, you found a Geyser didn't ya?",
	"No No No!",
	"You're supposed to put a Scavenger there!",
	"Let the games begin...",
	"Bad move...",
	"Is that all you can do?",
	"Why did you do that? Now I HAVE to kill you.",
	"And so, it begins...",
	"You're so smart...",
	"Excellent, now I know where you are!",
	"Game Start: The Waiting Game.",
	"Oh, it unfolds now?",
};

static const char *HumanScavengerDestroyedTaunts[] = // cat 9
{
	"Here scavy scavy!",
	"It's Scavenger season!",
	"That's MY scrap field!",
	"Poor defenseless Scavengers...",
	"Enemy Scavenger destroyed! Oops wrong chat.",
	"I hope you have enough scrap...",
	"Scavengers look so pretty when they explode, dont'cha think?",
	"You know, he won't be missed right?",
	"Aww, are you going to have a funeral for him?",
	"He isn't easily replaced, now is he?",
	"Now, use your scrap and build another.",
	"Was he your favorite scavenger?",
	"He might get a purple heart for that.",
};

static const char *CPUScavengerDestroyedTaunts[] = // cat 10
{
	"What'd you do THAT for?!",
	"Ugh, stupid Scavengers...",
	"No worries, I can build more.",
	"That's not very nice...",
	"Hey, I needed that! Just kidding...",
	"One less Scavenger for you to snipe, I suppose.",
	"George! Nooooooo!!! You'll pay for that...",
	"Why do you kill my 'farmers'?",
	"Now I'll have to build another...",
	"Thankfully those are disposable",
	"Easily replaced.",
	"Well, at least he won't be missed",
	"Well, at least she won't be missed",
	"Lassie come home!",
	"You killed Kenny!",
};

static const char *CPUShipSnipedTaunts[] = // cat 11
{
	"Hey! Don't steal that!",
	"Hmm?",
	"Oh no! How'd they find out our weakness?!",
	"So, you're gonna play like THAT...",
	"Who's going to clean that cockpit? Not me!",
	"Bob? Bob?! Come in Bob! Nooooooooooooo!",
	"You killed Kenny!",
	"Unit 17 report. Report! Damn it...",
	"You thief...",
	"Learn to play with what's yours...",
	"If you have to steal, you need more skills...",
	"Blasted Pirates!",
	"Someone kill that stupid human!",
	"Stop doing that!",
	"Noughty noughty...",
};

static const char *HumanShipStolenTaunts[] = // cat 12
{
	"Mine! Mine! Mine!",
	"All your unit are belong to us.",
	"He won't suspect a thing...",
};

static const char *CPUShipStolenTaunts[] = // cat 13
{
	"Give that back!",
	"<Insert Taunt Here>",
};

static const char *RandomTaunts[] = // cat 14
{
	"Did you know there's a fury behind you? :).",
	"Humans are so smart, yet so destructive...puzzling.",
	"I hope your self-destructive behavior gets the better of you.",
	"All your base are belong to us!",
	"This'll be a long game...",
	"Do you humans ever take breaks? I don't.",
	"I am superior, because I can easily outsmart you.",
	"Yes, I can talk yellow.",
	"I prefer fighting furies over you.",
	"Are you going to take all day or what?",
	"I got all the time in the world, unlike you.",
	"Did you know I can play chess too?",
	"You need more Scavengers...",
	"I will live forever, you wont.",
	"I can see you. :)",
	"011010100110111101101110011011100111100100110101", 
	"If you look at the monitor very closely, you'll go blind.",
	"Resistance is futile.",
};


// Quotes by General BlackDragon and DarkCobra. May 2012. 
// Additional quotes by Blue Banana, December 2014.

struct TauntIndexType
{
	char **TauntList;
	int LastTauntPrinted;
	int Count;
};

TauntIndexType TauntIndices[TAUNTS_MAX] =
{
	{(char**)GameStartTaunts, -1, sizeof(GameStartTaunts)/sizeof(GameStartTaunts[0])},
	{(char**)HumanEntersTaunts, -1, sizeof(HumanEntersTaunts)/sizeof(HumanEntersTaunts[0])},
	{(char**)HumanLeavesTaunts, -1, sizeof(HumanLeavesTaunts)/sizeof(HumanLeavesTaunts[0])},
	{(char**)HumanShipDestroyedTaunts, -1, sizeof(HumanShipDestroyedTaunts)/sizeof(HumanShipDestroyedTaunts[0])},
	{(char**)HumanPilotKilledTaunts, -1, sizeof(HumanPilotKilledTaunts)/sizeof(HumanPilotKilledTaunts[0])},
	{(char**)HumanRecyclerDestroyedTaunts, -1, sizeof(HumanRecyclerDestroyedTaunts)/sizeof(HumanRecyclerDestroyedTaunts[0])},
	{(char**)CPURecyclerDestroyedTaunts, -1, sizeof(CPURecyclerDestroyedTaunts)/sizeof(CPURecyclerDestroyedTaunts[0])},
	{(char**)HumanRecyclerDeployedTaunts, -1, sizeof(HumanRecyclerDeployedTaunts)/sizeof(HumanRecyclerDeployedTaunts[0])},
	{(char**)HumanScavengerDestroyedTaunts, -1, sizeof(HumanScavengerDestroyedTaunts)/sizeof(HumanScavengerDestroyedTaunts[0])},
	{(char**)CPUScavengerDestroyedTaunts, -1, sizeof(CPUScavengerDestroyedTaunts)/sizeof(CPUScavengerDestroyedTaunts[0])},
	{(char**)CPUShipSnipedTaunts, -1, sizeof(CPUShipSnipedTaunts)/sizeof(CPUShipSnipedTaunts[0])},
	{(char**)HumanShipStolenTaunts, -1, sizeof(HumanShipStolenTaunts)/sizeof(HumanShipStolenTaunts[0])},
	{(char**)CPUShipStolenTaunts, -1, sizeof(CPUShipStolenTaunts)/sizeof(CPUShipStolenTaunts[0])},
	{(char**)RandomTaunts, -1, sizeof(RandomTaunts)/sizeof(RandomTaunts[0])},
};

static int *g_pElapsedTime = NULL;
static int *g_pLastTauntPrintedAt = NULL;
static int *g_pTPS = NULL;

static bool DeterminedCPUTeamName = false;
char CPUTeamName[256] = "Team Computer";


// Required setup call before you call DoTaunt(). pGameTime is used to squelch too-often messages. [It should be the # of calls to
// Execute]. pLastMessagePrintedAt is a scratchpad variable used to note things, should be saved. pTPS should be a pointer to the TPS
// in use. aCPUTeamName is an override for that name; if specified, it won't query network.session.svar2 for it.
void InitTaunts(int* pGameTime, int* pLastMessagePrintedAt, int* pTPS, const char* pCPUTeamName)
{
	g_pElapsedTime = pGameTime;
	g_pLastTauntPrintedAt = pLastMessagePrintedAt;
	g_pTPS = pTPS;

	// Clear last printed to ensure everyone sees the same.
	for(int i = 0; i < TAUNTS_MAX; i++)
		TauntIndices[i].LastTauntPrinted = -1; // won't ever match

	if(pCPUTeamName != NULL)
	{
		strcpy_s(CPUTeamName, pCPUTeamName);
		DeterminedCPUTeamName = true;
	}
}

// Note: this function does all of the work of building up a taunt, and decides what to do with it only at the last step. This is by
// design-- it keeps taunt messages more in sync across network games, even if the local setting is to not display them.
void DoTaunt(TauntTypes Taunt, int Team)
{
	static char TempBuffer[512];

	if(Taunt >= TAUNTS_MAX)
		return;

	// 5% chance of showing a random taunt instead...
	if(GetRandomFloat(20) < 1.0f)
		Taunt = TAUNTS_Random;

	// Make sure we get the CPU's team name from game at lease once.
	if(!DeterminedCPUTeamName)
	{
	//	const char *TempName = GetVarItemStr("network.session.svar2");
	//	if(TempName != NULL)
	//		strcpy_s(CPUTeamName,TempName);
	//	else
		sprintf_s(CPUTeamName, "Computer Team %d", Team); 
	//	DeterminedCPUTeamName = true;
	}

	int TauntsInCategory = 0; //float
	int Which = 0;

	TauntsInCategory = TauntIndices[Taunt].Count; // +1
	// Loop over, making sure we don't print the same thing twice in a row.
	do
	{
		Which = GetRandomInt(TauntsInCategory); //int(GetRandomFloat(TauntsInCategory));
	} while ((Which >= TauntIndices[Taunt].Count) ||
		(Which == TauntIndices[Taunt].LastTauntPrinted));
	TauntIndices[Taunt].LastTauntPrinted = Which;

	sprintf_s(TempBuffer, "%s : %s\n", CPUTeamName, TauntIndices[Taunt].TauntList[Which]);

	// Message squelching -- only do this *AFTER* all calls to Random() and things that can affect gamestate. Need to do null pointer checks.
	// -- if this is the case, then InitTaunts() was not called correctly.
	if((g_pElapsedTime == NULL) ||
		(g_pLastTauntPrintedAt == NULL) ||
		(g_pTPS == NULL) ||
		((*g_pElapsedTime) - (*g_pLastTauntPrintedAt)) < ((*g_pTPS) * 20))
		return;

	// Note when we printed a taunt
	*g_pLastTauntPrintedAt = *g_pElapsedTime;

	// Always re-read pref; it might have changed during play.
	bool bEnableTaunts = IFace_GetInteger("options.play.taunts");

	if(bEnableTaunts)
	{
		if((IsTeamAllied(GetLocalPlayerTeamNumber(), Team)) && (Team > 0))
			AddToMessagesBox2(TempBuffer, ALLYBLUE);
		else if ((!IsTeamAllied(GetLocalPlayerTeamNumber(), Team)) && (Team > 0))
			AddToMessagesBox2(TempBuffer, RED);
		else
			AddToMessagesBox2(TempBuffer, YELLOW);

	}
}
