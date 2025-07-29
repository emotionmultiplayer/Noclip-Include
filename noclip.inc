/**
*                                                    Noclip Include - v0.2.4 
* Include created by xWendorion
* Based on code from Southclaws
* The original code has been modified and enhanced, and has been refactored into an include file to simplify system integration.
*
* Improvements include:
* - Code optimization for better performance
* - Added flexibility and customizability for various use cases
* - Simplified structure for easier maintenance and integration
*
* This include aims to streamline the process of adding the system to your project.
*
*/
#if defined NoclipInc
    #endinput
#endif
#define NoclipInc

#include <open.mp>

// Modos da câmera
#define CAMERA_MODE_NONE    	0
#define CAMERA_MODE_FLY     	1

// Teclas de movimentação
#define MOVE_FORWARD    		1
#define MOVE_BACK       		2
#define MOVE_LEFT       		3
#define MOVE_RIGHT      		4
#define MOVE_FORWARD_LEFT       5
#define MOVE_FORWARD_RIGHT      6
#define MOVE_BACK_LEFT          7
#define MOVE_BACK_RIGHT         8

#define MOVE_SPEED              100.0
#define ACCEL_RATE              0.03

enum noclipenum
{
	cameramode,
	flyobject,
	mode,
	lrold,
	udold,
	lastmove,
	Float:accelmul
}
new noclipdata[MAX_PLAYERS][noclipenum];

forward NoclipInit();
public NoclipInit()
{
	for (new x = 0; x < MAX_PLAYERS; x++)
	{
		if (noclipdata[x][cameramode] == CAMERA_MODE_FLY)
		{
			CancelFlyMode(x);
		}
	}
	return 1;
}
forward NoclipConnect(playerid);
public NoclipConnect(playerid)
{
	noclipdata[playerid][cameramode]  = CAMERA_MODE_NONE;
	noclipdata[playerid][lrold]       = 0;
	noclipdata[playerid][udold]       = 0;
	noclipdata[playerid][mode]        = 0;
	noclipdata[playerid][lastmove]    = 0;
	noclipdata[playerid][accelmul]    = 0.0;
	return 1;
}
forward NoclipUpdate(playerid);
public NoclipUpdate(playerid)
{
	if (noclipdata[playerid][cameramode] == CAMERA_MODE_FLY)
	{
		new keys, ud, lr;
		GetPlayerKeys(playerid, KEY:keys, ud, lr);

		if (noclipdata[playerid][mode] && (GetTickCount() - noclipdata[playerid][lastmove] > 100))
		{
			MoveCamera(playerid);
		}

		if (noclipdata[playerid][udold] != ud || noclipdata[playerid][lrold] != lr)
		{
			if ((noclipdata[playerid][udold] != 0 || noclipdata[playerid][lrold] != 0) && ud == 0 && lr == 0)
			{
				StopPlayerObject(playerid, noclipdata[playerid][flyobject]);
				noclipdata[playerid][mode]     = 0;
				noclipdata[playerid][accelmul] = 0.0;
			}
			else
			{
				noclipdata[playerid][mode] = GetMoveDirectionFromKeys(ud, lr);
				MoveCamera(playerid);
			}
		}

		noclipdata[playerid][udold] = ud;
		noclipdata[playerid][lrold] = lr;
		return 0;
	}
	return 1;
}

stock FlyMode(playerid)
{
	new Float:X, Float:Y, Float:Z;
	GetPlayerPos(playerid, X, Y, Z);

	noclipdata[playerid][flyobject] = CreatePlayerObject(playerid, 19300, X, Y, Z, 0.0, 0.0, 0.0);
	TogglePlayerSpectating(playerid, true);
	AttachCameraToPlayerObject(playerid, noclipdata[playerid][flyobject]);

	SetPVarInt(playerid, "FlyMode", 1);
	noclipdata[playerid][cameramode] = CAMERA_MODE_FLY;

	return 1;
}

stock CancelFlyMode(playerid)
{
	new Float:camX, Float:camY, Float:camZ;
	GetPlayerCameraPos(playerid, camX, camY, camZ);

	DeletePVar(playerid, "FlyMode");
	CancelEdit(playerid);
	TogglePlayerSpectating(playerid, false);
	SetPlayerPos(playerid, camX, camY, camZ);

	DestroyPlayerObject(playerid, noclipdata[playerid][flyobject]);
	noclipdata[playerid][cameramode] = CAMERA_MODE_NONE;

	return 1;
}

stock GetMoveDirectionFromKeys(ud, lr)
{
	new direction = 0;

	if (lr < 0)
	{
		if (ud < 0)        direction = MOVE_FORWARD_LEFT;
		else if (ud > 0)   direction = MOVE_BACK_LEFT;
		else               direction = MOVE_LEFT;
	}
	else if (lr > 0)
	{
		if (ud < 0)        direction = MOVE_FORWARD_RIGHT;
		else if (ud > 0)   direction = MOVE_BACK_RIGHT;
		else               direction = MOVE_RIGHT;
	}
	else if (ud < 0)       direction = MOVE_FORWARD;
	else if (ud > 0)       direction = MOVE_BACK;

	return direction;
}

stock GetNextCameraPosition(move_mode, const Float:CP[3], const Float:FV[3], &Float:X, &Float:Y, &Float:Z)
{
	#define OFFSET_X (FV[0]*6000.0)
	#define OFFSET_Y (FV[1]*6000.0)
	#define OFFSET_Z (FV[2]*6000.0)

	switch (move_mode)
	{
		case MOVE_FORWARD:
		{
			X = CP[0] + OFFSET_X;
			Y = CP[1] + OFFSET_Y;
			Z = CP[2] + OFFSET_Z;
		}
		case MOVE_BACK:
		{
			X = CP[0] - OFFSET_X;
			Y = CP[1] - OFFSET_Y;
			Z = CP[2] - OFFSET_Z;
		}
		case MOVE_LEFT:
		{
			X = CP[0] - OFFSET_Y;
			Y = CP[1] + OFFSET_X;
			Z = CP[2];
		}
		case MOVE_RIGHT:
		{
			X = CP[0] + OFFSET_Y;
			Y = CP[1] - OFFSET_X;
			Z = CP[2];
		}
		case MOVE_BACK_LEFT:
		{
			X = CP[0] + (-OFFSET_X - OFFSET_Y);
			Y = CP[1] + (-OFFSET_Y + OFFSET_X);
			Z = CP[2] - OFFSET_Z;
		}
		case MOVE_BACK_RIGHT:
		{
			X = CP[0] + (-OFFSET_X + OFFSET_Y);
			Y = CP[1] + (-OFFSET_Y - OFFSET_X);
			Z = CP[2] - OFFSET_Z;
		}
		case MOVE_FORWARD_LEFT:
		{
			X = CP[0] + (OFFSET_X - OFFSET_Y);
			Y = CP[1] + (OFFSET_Y + OFFSET_X);
			Z = CP[2] + OFFSET_Z;
		}
		case MOVE_FORWARD_RIGHT:
		{
			X = CP[0] + (OFFSET_X + OFFSET_Y);
			Y = CP[1] + (OFFSET_Y - OFFSET_X);
			Z = CP[2] + OFFSET_Z;
		}
	}
}

stock MoveCamera(playerid)
{
	new Float:FV[3], Float:CP[3];
	GetPlayerCameraPos(playerid, CP[0], CP[1], CP[2]);
	GetPlayerCameraFrontVector(playerid, FV[0], FV[1], FV[2]);

	if (noclipdata[playerid][accelmul] <= 1.0)
		noclipdata[playerid][accelmul] += ACCEL_RATE;

	new Float:speed = MOVE_SPEED * noclipdata[playerid][accelmul];

	new Float:X, Float:Y, Float:Z;
	GetNextCameraPosition(noclipdata[playerid][mode], CP, FV, X, Y, Z);
	MovePlayerObject(playerid, noclipdata[playerid][flyobject], X, Y, Z, speed);

	noclipdata[playerid][lastmove] = GetTickCount();
	return 1;
}
