#include "..\..\source\fun3d\ScriptUtils.h"
#include "cintools.h"
#include "SPMission.h"
#include <crtdbg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#define MAX_ACTIONS 500
#define MAX_CAMERA_ACTIONS 30
#define MAX_POSITIONS 1000

struct ActionTime {
	int startTime;
	int endTime;
	int actionNumber;
	int actionType;  // 1= Actor, 2= sound, 3= camera
} AllActionTimes[MAX_ACTIONS];

struct ActorAction {
	int actionTimeNumber;  // which of the action times is mine
	int actionNumber;
	int actionType;
	char actorName[20];
	char targetName[20];
} ActorActions[100];
	
struct SoundAction {
	int actionTimeNumber;  // which of the action times is mine
	int actionNumber;
	int fadeToVolume;
	int startFade;
	char sound[10];
} SoundActions[30];

class CameraAction {
	public:
	int moveType; //1=dolly, 2=pan, 3=rotate, 4=chase
	float moveDuration;  //how long should this camera move take
	char attachObject[20]; //for chase and rotate. What object am I attached to?
	char viewObject[40]; //for chase and rotate. What object am I looking at?
	
	float rotation; //6.28 = a full rotation
	int startPosition; //0 = behind vehicle, 3.14 = in front of vehicle
	int xOffset; 
	int yOffset;
	int zOffset;
	Vector s_Position;
	Vector s_Direction;
	Vector e_Position;
	Vector e_Direction;
	char moveName[40];
	int seqNum;
	char soundName[40];
	CameraAction(void)
	{
		strcpy(viewObject,"");
		strcpy(moveName,"");
		strcpy(soundName,"");
	}
} CameraActions[MAX_CAMERA_ACTIONS];


/* Below are some variables declared to avoid repeating manage the cin move through
a series of frames.  These variables get cleaned up when a move ends.  We need to make sure
that we do this clean up if the player skips out of a cineractive.*/

//I've declared this as a global so that I do not need to search for the camera 
//move to play every frame.
CameraAction *playMe; 
Vector position;
Vector direction;
Vector changePosition;
Vector changeDirection;

class CameraPosition {
public:
	Vector position;
	Vector direction;
	CameraPosition(void)	{
		position.x = 0.0f;
		position.y = 0.0f;
		position.z = 0.0f;
		direction.x = 0.0f;
		direction.y = 0.0f;
		direction.z = 0.0f;
	}
} CameraPositions[MAX_POSITIONS];


float s_t;
float dt;
Matrix s_Mat;
Matrix e_Mat;


bool startMove = false; //this tells us if we need to setup for the move or just continue a move
bool moveDone = false; //This will return true when the current camera move is done
int moveCount = 0; //this keep track of where we are in the move so we know when to end it
int seqCount = 0;
float t = 0.0;
float end = 0.0;
float speed = 0.0;
bool viewObject;
bool doneEditSetup = false;
bool startRecord = true;
int recordTime = 0;
bool startPlay = true;
int playTime = 0;
Handle lookAtMe;
char cinSeries[20] = "";
int moveNumber;
bool doneCinSetup = false;

void DollyCamera(CameraAction *CamMove);

//add two vectors
Vector AddVectors(const Vector &A, const Vector &B)
{
	Vector v;
	v.x = A.x + B.x;
	v.y = A.y + B.y;
	v.z = A.z + B.z;
	return v;
}

// subtract two vectors
Vector SubVectors(const Vector &A, const Vector &B)
{
	Vector v;
	v.x = A.x - B.x;
	v.y = A.y - B.y;
	v.z = A.z - B.z;
	return v;
}

// divide a vector by a float
Vector DivVector(const Vector &A, float B)
{
	Vector v;
	v.x = A.x/B;
	v.y = A.y/B;
	v.z = A.z/B;
	return v;
}

//set one vector equal to another
Vector SetVector(const Vector &A)
{
	Vector v;
	v.x = A.x;
	v.y = A.y;
	v.z = A.z;
	return v;
}

//get the length of a vector
float VecLen(const Vector &A) {
	return (float)sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
}

void writevec(FILE *fp,Vector *v)
{
	fprintf(fp,"%f \n",v->x);
	fprintf(fp,"%f \n",v->y);
	fprintf(fp,"%f \n",v->z);
}
void readvec(FILE *fp,Vector *v)
{
	const len=128;
	char line[len];
	fgets(line,len,fp);
	v->x=(float)atof(line);
	fgets(line,len,fp);
	v->y=(float)atof(line);
	fgets(line,len,fp);
	v->z=(float)atof(line);
}

/*Create a new group of cineractive files. */
void NewCinGroup() {
	IFace_ClearListBox("MoveManager.MoveList.List");
	IFace_GetString("script.newcin.name", cinSeries, 21); //This sets the name to global cinSeries.
	moveNumber = 0;
	for (int i = 0; i<30; i++) {
		strcpy(CameraActions[i].moveName, "");
	}
}


void LoadCameraArray(char *f)
{
	FILE *fp=fopen(f,"r");
	const len=128;
	char line[len];
	for (int i=0;i<MAX_CAMERA_ACTIONS;i++)
	{
		fgets(line,len,fp);
		CameraActions[i].moveType=atoi(line);
		fgets(line,len,fp);
		CameraActions[i].moveDuration=(float)atof(line);
		fgets(line,len,fp);
		strcpy(CameraActions[i].attachObject,line);
		if (CameraActions[i].attachObject[strlen(CameraActions[i].attachObject)-1]=='\r') {
			CameraActions[i].attachObject[strlen(CameraActions[i].attachObject)-1]='\0';
		}
		fgets(line,len,fp);
		strcpy(CameraActions[i].viewObject,line);
		if (CameraActions[i].viewObject[strlen(CameraActions[i].viewObject)-1]=='\n') {
			CameraActions[i].viewObject[strlen(CameraActions[i].viewObject)-1]=NULL;
		}
		fgets(line,len,fp);
		CameraActions[i].rotation=(float)atof(line);
		fgets(line,len,fp);
		CameraActions[i].startPosition=atoi(line);
		fgets(line,len,fp);
		CameraActions[i].xOffset=atoi(line);
		fgets(line,len,fp);
		CameraActions[i].yOffset=atoi(line);
		fgets(line,len,fp);
		CameraActions[i].zOffset=atoi(line);
		readvec(fp,&CameraActions[i].s_Position);
		readvec(fp,&CameraActions[i].s_Direction);
		readvec(fp,&CameraActions[i].e_Position);
		readvec(fp,&CameraActions[i].e_Direction);
		fgets(line,len,fp);
		strcpy(CameraActions[i].moveName,line);
		if (CameraActions[i].moveName[strlen(CameraActions[i].moveName)-1]=='\n') {
			CameraActions[i].moveName[strlen(CameraActions[i].moveName)-1]='\0';
		}
		fgets(line,len,fp);
		CameraActions[i].seqNum=atoi(line);
		fgets(line,len,fp);
		strcpy(CameraActions[i].soundName,line);
		if (CameraActions[i].soundName[strlen(CameraActions[i].soundName)-1]=='\n') {
			CameraActions[i].soundName[strlen(CameraActions[i].soundName)-1]='\0';
		}
	}
	fclose(fp);
}

/*Open an existing group of cineractive moves from disk and load all of the 
moves into the listbox.  This will get the user ready to AddCinMove to the end of
the list or to LoadCinMove for editing.*/

void LoadCinFiles() {
	char name[32];
	IFace_GetString("script.newcin.name", name, sizeof(name));
	if (strcmp(cinSeries, name) && (name != "")){
		IFace_GetString("script.newcin.name", cinSeries, sizeof(cinSeries));
		LoadCameraArray(cinSeries); //George look for saved file with cinSeries name.
	} 
	IFace_Deactivate("CinCamDolly");
	IFace_Deactivate("SequenceEdit");
	IFace_Activate("MoveManager");
	IFace_ClearListBox("MoveManager.MoveList.List");
	int i;
	for (i = 0; i < 30; i++) {
		IFace_AddTextItem("MoveManager.MoveList.List", CameraActions[i].moveName);
		if (!strcmp(CameraActions[i].moveName, "") || !strcmp(CameraActions[i].moveName, "0") ) {
			moveNumber = i;
			i = 31;
		}
	}
}

//Add a new cineractive move to the current list of cineractive moves.

void AddCinMove() {
	if (strcmp(cinSeries, "")) {
		EditSetup(); // Make sure all of the variables are cleared.
		IFace_Deactivate("MoveManager"); //Change the interface screens.
		IFace_Activate("CinCamDolly");
		int i;
		for (i = 0; i < 30; i++) { //set moveNumber for saving out to the array later.
			if (!strcmp(CameraActions[i].moveName, "")) {
				moveNumber = i;
				i = 31;
			}
		}
	}
}

/*When the user double clicks on a cineractive move name, this function will go through
and load the cineractive move editor with all of the data from that cineractive move. */

void LoadCinMove(char *cinMoveName) {
	EditSetup(); //make sure that all of the variables are initialized
	IFace_Deactivate("MoveManager");
	IFace_Activate("CinCamDolly");
	int i;
	for (i = 0; i < 30; i++) {
		if (!strcmp(cinMoveName, CameraActions[i].moveName)) {
			IFace_SetFloat("script.xPos.value", CameraActions[i].s_Position.x);
			IFace_SetFloat("script.yPos.value", CameraActions[i].s_Position.y);
			IFace_SetFloat("script.zPos.value", CameraActions[i].s_Position.z);
			IFace_SetFloat("script.xDir.value", CameraActions[i].s_Direction.x);
			IFace_SetFloat("script.yDir.value", CameraActions[i].s_Direction.y);
			IFace_SetFloat("script.zDir.value", CameraActions[i].s_Direction.z);
			IFace_SetFloat("script.xPosEnd.value", CameraActions[i].e_Position.x);
			IFace_SetFloat("script.yPosEnd.value", CameraActions[i].e_Position.y);
			IFace_SetFloat("script.zPosEnd.value", CameraActions[i].e_Position.z);
			IFace_SetFloat("script.xDirEnd.value", CameraActions[i].e_Direction.x);
			IFace_SetFloat("script.yDirEnd.value", CameraActions[i].e_Direction.y);
			IFace_SetFloat("script.zDirEnd.value", CameraActions[i].e_Direction.z);
			IFace_SetFloat("script.duration.value", CameraActions[i].moveDuration);
			IFace_SetString("script.name.data", CameraActions[i].moveName);	
			IFace_SetString("editor.object.label", CameraActions[i].viewObject);
			IFace_SetString("script.play.sound", CameraActions[i].soundName);
			moveNumber = i;
			i = 31;
		}
	}
}

/* This function will load a sequence and get it ready to be edited*/
void LoadSequence() {
	IFace_Deactivate("MoveManager");
	IFace_Activate("SequenceEdit");
	int i;
	for (i = 0; i < 10; i++) { 
		if (strcmp(CameraActions[i].moveName, "")) {
			char varName1[40];
			char varName2[40];
			sprintf_s(varName1, "script.n%d.name", i);
			sprintf_s(varName2, "script.n%d.value", i);
			IFace_SetString(varName1, CameraActions[i].moveName);
			IFace_SetFloat(varName2, (float)CameraActions[i].seqNum);
			moveNumber = 1000;
		}
		else {
			i = 31;
		}
	}
}

/* When playing back a sequence this function will loop through the array
to find the next move in the sequence. */

int FindNextSequence(int clipNum) {
	int i;
	bool foundClip = false;
	for (i = 0; i < 10; i++) {
		if (clipNum == CameraActions[i].seqNum) {
			return i;
			foundClip = true;
		}
	}
	if (foundClip == false) {
		return 1000;
	}

	_ASSERTE (0);
	return 1000;
}

/* This function will play a sequence of moves when sequence is set to true.
If sequence is false it will only play an individual sequence. */

bool PlaySequence(bool sequence) {
	if (strcmp(CameraActions[seqCount].moveName, "")) {
		if ((seqCount == 0) && (moveCount == 0)) {
			moveNumber = FindNextSequence(seqCount);
			if (CameraActions[moveNumber].moveDuration != 0.0) {
				CameraReady();
			}
		}
		if ((moveDone == false) && (moveNumber != 1000)) {
			DollyCamera(&CameraActions[moveNumber]);
			return true;
		}
		else {
			seqCount++;
			moveNumber = FindNextSequence(seqCount);
			moveDone = false;
			return true;
		}
	}
	else if (!strcmp(CameraActions[seqCount].moveName, "") || (moveNumber == 1000) || (CameraActions[moveNumber].moveDuration == 0.0)) {
		seqCount = 0;
		moveNumber = 1000;
		CameraFinish();
		return false;
	}

	//JRJ 
	return true;
}

bool PlayMove()	{
	if (moveDone == false) {
		if (moveCount == 0) {
			if (CameraActions[moveNumber].moveDuration != 0.0) { //Test to see that duration is not 0
				CameraReady();
			}
		}
		DollyCamera(&CameraActions[moveNumber]);	
		return true;
	}
	else {
		moveDone = false;
		CameraFinish();
		return false;
	}
}


bool PlayMovie(char name[20]) {
	if (!strcmp(name,"***")) {
		strcpy(name, cinSeries);
	}
	if (seqCount == 0) { //Load the array
		LoadCameraArray(name);
	}
	if (strcmp(CameraActions[seqCount].moveName, "")) {
		if ((seqCount == 0) && (moveCount == 0)) {
			moveNumber = FindNextSequence(seqCount);
			if (CameraActions[moveNumber].moveDuration != 0.0) {
				CameraReady();
			}
		}
		if (moveDone == false) { //Go here while playing the current move.
			if (moveNumber != 1000) {
				DollyCamera(&CameraActions[moveNumber]);
				return true;
			}
		}
		else if (moveDone == true) { //Go here to advance to the next move.
			seqCount++;
			moveNumber = FindNextSequence(seqCount);
			moveDone = false;
			return true;
		}
	}
	else if (!strcmp(CameraActions[seqCount].moveName, "") || (moveNumber == 1000) || (CameraActions[moveNumber].moveDuration == 0.0)) {
		seqCount = 0;
		moveNumber = 1000;
		CameraFinish();
		return false;
	}

	// JRJ 
	_ASSERTE (0);
	return false;
}


void SaveCameraArray(char *f)
{
	// this saves the whole array
	FILE *fp=fopen(f,"w+");
	for (int i=0;i<MAX_CAMERA_ACTIONS;i++)
	{
		fprintf(fp,"%d \n",CameraActions[i].moveType);
		fprintf(fp,"%f \n",CameraActions[i].moveDuration);
		if (CameraActions[i].attachObject[strlen(CameraActions[i].attachObject)-1]=='\n') {
			CameraActions[i].attachObject[strlen(CameraActions[i].attachObject)-1]='\0';
		}
		fprintf(fp,"%s\n",CameraActions[i].attachObject);
		if (CameraActions[i].viewObject[strlen(CameraActions[i].viewObject)-1]=='\n') {
			CameraActions[i].viewObject[strlen(CameraActions[i].viewObject)-1]='\0';
		}
		fprintf(fp,"%s\n",CameraActions[i].viewObject);
		fprintf(fp,"%f \n",CameraActions[i].rotation);
		fprintf(fp,"%d \n",CameraActions[i].startPosition);
		fprintf(fp,"%d \n",CameraActions[i].xOffset);
		fprintf(fp,"%d \n",CameraActions[i].yOffset);
		fprintf(fp,"%d \n",CameraActions[i].zOffset);
		// now do the vectors..
		writevec(fp,&CameraActions[i].s_Position);
		writevec(fp,&CameraActions[i].s_Direction);
		writevec(fp,&CameraActions[i].e_Position);
		writevec(fp,&CameraActions[i].e_Direction);
		fprintf(fp,"%s\n",CameraActions[i].moveName);
		fprintf(fp,"%d \n",CameraActions[i].seqNum);
		fprintf(fp,"%s\n",CameraActions[i].soundName);
	}
	fclose(fp);
	
}

void SaveMove(CameraAction *CamMove) { //This will save the start position.
	CamMove->s_Position.x = IFace_GetFloat("script.xPos.value");
	CamMove->s_Position.y = IFace_GetFloat("script.yPos.value");
	CamMove->s_Position.z = IFace_GetFloat("script.zPos.value");
	CamMove->s_Direction.x = IFace_GetFloat("script.xDir.value");
	CamMove->s_Direction.y = IFace_GetFloat("script.yDir.value");
	CamMove->s_Direction.z = IFace_GetFloat("script.zDir.value");
	CamMove->e_Position.x = IFace_GetFloat("script.xPosEnd.value");
	CamMove->e_Position.y = IFace_GetFloat("script.yPosEnd.value");
	CamMove->e_Position.z = IFace_GetFloat("script.zPosEnd.value");
	CamMove->e_Direction.x = IFace_GetFloat("script.xDirEnd.value");
	CamMove->e_Direction.y = IFace_GetFloat("script.yDirEnd.value");
	CamMove->e_Direction.z = IFace_GetFloat("script.zDirEnd.value");
	CamMove->moveDuration = IFace_GetFloat("script.duration.value");
	IFace_GetString("script.name.data", CamMove->moveName, 39);	
	IFace_GetString("editor.object.label", CamMove->viewObject, 39);
	IFace_GetString("script.play.sound", CamMove->soundName, 39);
}

void SaveSequence() {
	int i;
	for (i = 0; i < 10; i++) { //set moveNumber for saving out to the array later.
		char varName[40];
		sprintf_s(varName, "script.n%d.value", i);
		CameraActions[i].seqNum = (int)IFace_GetFloat(varName);
	}
	SaveCameraArray(cinSeries);
}

void EditSetup() {
	if (doneEditSetup == false) {
		IFace_CreateFloat("script.xPos.value", 0.0);
		IFace_CreateFloat("script.yPos.value", 0.0);
		IFace_CreateFloat("script.zPos.value", 0.0);
		IFace_CreateFloat("script.xDir.value", 0.0);
		IFace_CreateFloat("script.yDir.value", 0.0);
		IFace_CreateFloat("script.zDir.value", 0.0);
		IFace_CreateFloat("script.xPosEnd.value", 0.0);
		IFace_CreateFloat("script.yPosEnd.value", 0.0);
		IFace_CreateFloat("script.zPosEnd.value", 0.0);
		IFace_CreateFloat("script.xDirEnd.value", 0.0);
		IFace_CreateFloat("script.yDirEnd.value", 0.0);
		IFace_CreateFloat("script.zDirEnd.value", 0.0);
		IFace_CreateFloat("script.duration.value", 0.0);
		IFace_CreateString("script.name.data", "");
		IFace_CreateString("script.play.sound", "");
		doneEditSetup = true;
	}
	else if (doneEditSetup == true) {
		IFace_SetFloat("script.xPos.value", 0.0);
		IFace_SetFloat("script.yPos.value", 0.0);
		IFace_SetFloat("script.zPos.value", 0.0);
		IFace_SetFloat("script.xDir.value", 0.0);
		IFace_SetFloat("script.yDir.value", 0.0);
		IFace_SetFloat("script.zDir.value", 0.0);
		IFace_SetFloat("script.xPosEnd.value", 0.0);
		IFace_SetFloat("script.yPosEnd.value", 0.0);
		IFace_SetFloat("script.zPosEnd.value", 0.0);
		IFace_SetFloat("script.xDirEnd.value", 0.0);
		IFace_SetFloat("script.yDirEnd.value", 0.0);
		IFace_SetFloat("script.zDirEnd.value", 0.0);
		IFace_SetFloat("script.duration.value", 0.0);
		IFace_SetString("script.name.data", "");
		IFace_SetString("script.play.sound", "");
		IFace_SetString("editor.object.label", "");
	}
	//IFace_CreateString("script.object.label", " "); //Get Brad to copy eyedrop fucntion to dll.
}


//This does all of the setup for the cin tools.
void cinToolSetup() {
	if (!doneCinSetup) {
		//These are the commands for managing groups of moves.
		IFace_CreateCommand("script.create.cinfile");
		IFace_CreateCommand("script.load.cinfile");
		IFace_CreateCommand("script.load.move");
		IFace_CreateCommand("script.add.move");
		IFace_CreateCommand("script.run.sequence");
		IFace_CreateCommand("script.load.sequencer");

		//These are the commands for editing moves
		IFace_CreateCommand("script.setStart.values");
		IFace_CreateCommand("script.setEnd.values");
		IFace_CreateCommand("script.setDuration.values");
		IFace_CreateCommand("script.test");
		IFace_CreateCommand("script.save");
		IFace_CreateCommand("script.back.set");
		IFace_CreateCommand("script.sound");


		IFace_CreateString("script.newcin.name", "");
		IFace_CreateString("script.cinfiles.name", "");

		int i;
		for (i = 0; i < 10; i++) { //set moveNumber for saving out to the array later.
			char varName1[40];
			char varName2[40];
			sprintf_s(varName1, "script.n%d.name", i);
			sprintf_s(varName2, "script.n%d.value", i);
			IFace_CreateString(varName1, "");
			IFace_CreateFloat(varName2, 15);
		}

		IFace_Exec("bzshell_cam_move.cfg");
		IFace_Exec("bzshell_cin_cam_dolly.cfg");
		IFace_Exec("bzshell_sequence_edit.cfg");
		doneCinSetup = true;
	}
	IFace_Activate("MoveManager");		
}

void RecordCamera(char name[20]) {
	if (startRecord == true) {
		recordTime = 0;
		startRecord = false;
	}
	if (recordTime < 1000) {
		GetCameraPosition(position, direction);
		CameraPositions[recordTime].position = position;
		CameraPositions[recordTime].direction = direction;
		recordTime++;
	}
	else if (recordTime = 1000) {
		recordTime = 0;
		startRecord = true;
		//And we need something to indicate to the player that we're done recording.
	}
}

void PlayRecording(char name[20]) {
	if (startPlay == true) {
		playTime = 0;
	}
	if ((CameraPositions[playTime].position.x != 0) && (playTime < 1000)) {
		SetCameraPosition(CameraPositions[playTime].position,CameraPositions[playTime].direction);
		playTime++;
	}
	else if (playTime == 1000) {
		//figure out how we will exit the loop.
	}
}

	




//This processes all of the interface commands for the cin tools.
void CinProcessCommand(unsigned long crc)
{
	unsigned long cmd1Crc = CalcCRC("script.setStart.values");
	unsigned long cmd2Crc = CalcCRC("script.setEnd.values");
	unsigned long cmd3Crc = CalcCRC("script.setDuration.values");
	unsigned long cmd4Crc = CalcCRC("script.test");
	unsigned long cmd5Crc = CalcCRC("script.save");
	unsigned long cmd6Crc = CalcCRC("script.create.cinfile");
	unsigned long cmd7Crc = CalcCRC("script.load.cinfile");
	unsigned long cmd8Crc = CalcCRC("script.load.move");
	unsigned long cmd9Crc = CalcCRC("script.add.move");
	unsigned long cmd10Crc = CalcCRC("script.back.set");
	unsigned long cmd11Crc = CalcCRC("script.run.sequence");
	unsigned long cmd12Crc = CalcCRC("script.load.sequencer");
	unsigned long cmd13Crc = CalcCRC("script.sound");

	if (crc == cmd1Crc) { //save start position
		Vector position, direction;
		GetCameraPosition(position, direction);
		IFace_SetFloat("script.xPos.value", position.x);
		IFace_SetFloat("script.yPos.value", position.y);
		IFace_SetFloat("script.zPos.value", position.z);
		IFace_SetFloat("script.xDir.value", direction.x);
		IFace_SetFloat("script.yDir.value", direction.y);
		IFace_SetFloat("script.zDir.value", direction.z);
	}

	if (crc == cmd2Crc) { //save end position
		Vector position, direction;
		GetCameraPosition(position, direction);
		IFace_SetFloat("script.xPosEnd.value", position.x);
		IFace_SetFloat("script.yPosEnd.value", position.y);
		IFace_SetFloat("script.zPosEnd.value", position.z);
		IFace_SetFloat("script.xDirEnd.value", direction.x);
		IFace_SetFloat("script.yDirEnd.value", direction.y);
		IFace_SetFloat("script.zDirEnd.value", direction.z);
	}

	if (crc == cmd3Crc) { //save duration
		float duration;
		duration = IFace_GetFloat("script.duration.value");

	}
	
	if (crc == cmd4Crc) { //test move 
		SaveMove(&CameraActions[moveNumber]);
	}

	if (crc == cmd5Crc) { //save dolly move 
		SaveMove(&CameraActions[moveNumber]); //Save the start cam position.
		CameraActions[moveNumber].moveType = 1; //Save the move type as a dolly.
		SaveCameraArray(cinSeries);
	 }

	if (crc == cmd6Crc) { //create new group of cineractive moves.
		NewCinGroup();
	}

	if (crc == cmd7Crc) { //load existing file
		LoadCinFiles();
	}

	if (crc == cmd8Crc) { //load move
		char moveName[40];
		IFace_GetSelectedItem("MoveManager.MoveList.List", moveName, sizeof(moveName));
		LoadCinMove(moveName);
	}

	if (crc == cmd9Crc) { //add cin move
		AddCinMove();
	}

	if (crc == cmd10Crc) { //back up from the cin editing screen
		LoadCinFiles();
	}

	if (crc == cmd11Crc) { //test sequence
		SaveSequence();
	}

	if (crc == cmd12Crc) { //load sequence
		LoadSequence();
	}

	if (crc == cmd13Crc) { //make sure the sound name gets saved
		SaveMove(&CameraActions[moveNumber]);
	}

}


/* DollyCamera will move the camera from a start position and
orientation to an end postition and orientation.  This version of 
Dolly does not sync to time. Rather it is sinced to the number of times
that we go through the simulation loop.  If we want to sync to time,
I would need to rewrite this to get the current time on each frame and 
recalculate the move distance each frame based on the progress that we
made from the last frame.*/

void DollyCamera(CameraAction *CamMove) {
	if (startMove == false) { 
		//Setup for dolly camera move.  Executes on first frame only.
		if (CamMove->moveDuration != 0.0) { //Checks to see valid move
			if (!strcmp(CamMove->viewObject,"")) { 
				//Setup for simulation between beginning and end position.
				s_t = 0;
				dt = 1.0f / CamMove->moveDuration;
				s_Mat = Build_Directinal_Matrix (CamMove->s_Position, CamMove->s_Direction);
				e_Mat = Build_Directinal_Matrix (CamMove->e_Position, CamMove->e_Direction);
				SetCameraPosition(CamMove->s_Position, CamMove->s_Direction);
				SetCameraPosition(CamMove->s_Position, CamMove->s_Direction);
				viewObject = false;
			}
			else { 
				//Setup for always looking at a target object.
				position = SubVectors(CamMove->e_Position, CamMove->s_Position);
				changePosition = DivVector(position, CamMove->moveDuration);

				lookAtMe = GetHandle(CamMove->viewObject);
				Vector target = GetPosition(lookAtMe);
				direction = SubVectors(target, CamMove->s_Position);
				direction = Normalize_Vector(direction);
				SetCameraPosition(CamMove->s_Position, direction);
				SetCameraPosition(CamMove->s_Position, direction);
				viewObject = true;
			}


			if (strcmp(CamMove->soundName,"")) {
				AudioMessage(CamMove->soundName);
			}
			startMove = true; 
		}
		else if (CamMove->moveDuration == 0.0) {
			moveDone = true;
			startMove = false;
			moveCount = 0;
		}
	}
	else if (startMove == true)  { //This will do the dolly move. Will exit when moveCount = duration.
		if ((moveCount < CamMove->moveDuration) && (CamMove->moveDuration != 0)) {
			GetCameraPosition(position, direction);
			if (viewObject == false) { //Simulate camera direction
				s_t	+= dt;
				Matrix Mat (Interpolate_Matrix(s_Mat, e_Mat, s_t));
				direction = Mat.front;
				position = Mat.posit;
			}
			else { 
				//This will keep the camera always looking at the target object.
				position = AddVectors(position, changePosition);
				Vector target = GetPosition(lookAtMe);
				direction = SubVectors(target, position);
				direction = Normalize_Vector(direction);
			}
		    SetCameraPosition(position, direction);
  			moveCount++; 
			moveDone = false;
		}
		else if (moveCount >= CamMove->moveDuration) {
			moveDone = true;
			startMove = false;
			moveCount = 0;
		}
	}

}


void ChaseObject(CameraAction *CamMove) {
	if (moveCount <= CamMove->moveDuration) {
		/*set the offset position from the attached object.*/
		Handle aObj = GetHandle(CamMove->attachObject);
		Vector attachObject = GetPosition(aObj);
		Vector front = GetFront(aObj);
		position.x = (attachObject.x + (-front.x*CamMove->xOffset));
		position.y = (attachObject.y + CamMove->yOffset);
		position.z = (attachObject.z + (-front.z*CamMove->zOffset));
  		
		//set the camera to look at the appropriate target.	
		Handle vObj = GetHandle(CamMove->viewObject);
		Vector viewObject = GetPosition(vObj);
		Vector distance = SubVectors(viewObject, position);
		distance = Normalize_Vector(distance);
 		direction = SetVector(distance);
		SetCameraPosition(position, direction);
		moveCount++;
		moveDone = false;
	}
	else if (moveCount > CamMove->moveDuration) {
		moveDone = true;
		startMove = false;
		moveCount = 0;
	}
}



CameraAction *findMoveData(char *moveName) {
	for (int i = 0; i < 30; i++) {
		if (strcmp(CameraActions[i].moveName,moveName) == 0)
			return &CameraActions[i];
	}
	return NULL;
}





