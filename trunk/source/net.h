//-------------------------------------------------------------------------
/*
Copyright (C) 1997, 2005 - 3D Realms Entertainment

This file is part of Shadow Warrior version 1.2

Shadow Warrior is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Original Source: 1997 - Frank Maddin and Jim Norwood
Prepared for public release: 03/28/2005 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------

#define PACKET_TYPE_MASTER_TO_SLAVE                 0
#define PACKET_TYPE_SLAVE_TO_MASTER                 1
//#define PACKET_TYPE_PLAYER_NAME                     3
#define PACKET_TYPE_MESSAGE                         4
//#define PACKET_TYPE_GAME_INFO                       8
#define PACKET_TYPE_BROADCAST                       17
#define SERVER_GENERATED_BROADCAST					18

#define PACKET_TYPE_NEW_GAME                        30    
//#define PACKET_TYPE_NEW_LEVEL                       31
#define PACKET_TYPE_PLAYER_OPTIONS                  32
#define PACKET_TYPE_RTS                             33
#define PACKET_TYPE_DUMMY                           34
#define PACKET_TYPE_MENU_LEVEL_QUIT                 35
#define PACKET_TYPE_NAME_CHANGE                     36
#define PACKET_TYPE_VERSION                         38

#define PACKET_TYPE_NULL_PACKET                     127
#define PACKET_TYPE_PLAYER_READY                    250
#define PACKET_TYPE_DONT_USE                        255  // old logoff

#define BIT_CODEC TRUE
#define SYNC_TEST TRUE
#define MAXSYNCBYTES 16

typedef struct
    {
    BYTE PacketType;  // first byte is always packet type
    BYTE FirstPlayerIndex;
    BOOL AutoAim;
    BYTE Level;
    BYTE Episode;
    CHAR Skill;
    BYTE GameType;
    BOOL HurtTeammate;
    BOOL SpawnMarkers;
    BOOL TeamPlay;
    BYTE KillLimit;
    BYTE TimeLimit;
    BOOL Nuke;
    }PACKET_NEW_GAME,*PACKET_NEW_GAMEp;

typedef struct
    {
    BYTE PacketType;  // first byte is always packet type
    BOOL AutoRun;
    BYTE Color;
    char PlayerName[32];
    }PACKET_OPTIONS,*PACKET_OPTIONSp;

typedef struct
    {
    BYTE PacketType;  // first byte is always packet type
    char PlayerName[32];
    }PACKET_NAME_CHANGE,*PACKET_NAME_CHANGEp;
    
typedef struct
    {
    BYTE PacketType;  // first byte is always packet type
    BYTE RTSnum;
    }PACKET_RTS,*PACKET_RTSp;

typedef struct
    {
    BYTE PacketType;  // first byte is always packet type
    long Version;
    }PACKET_VERSION,*PACKET_VERSIONp;
    
extern BYTE syncstat[MAXSYNCBYTES];
extern BOOL PredictionOn;
extern PLAYER PredictPlayer;
extern PLAYERp ppp;
extern short predictangpos[MOVEFIFOSIZ];
extern long predictmovefifoplc;
extern BOOL Prediction;
extern short NumSyncBytes;
    
void InitPrediction(PLAYERp pp);
void DoPrediction(PLAYERp ppp);
void CorrectPrediction(long actualfifoplc);

//TENSW: safe packet senders
void netsendpacket(int ind, char *buf, int len);
void netbroadcastpacket(char *buf, int len);
short netgetpacket(short *ind, char *buf);


enum MultiGameTypes
    {
    MULTI_GAME_NONE,
    MULTI_GAME_COMMBAT,
    MULTI_GAME_COMMBAT_NO_RESPAWN, // JUST a place holder for menus. DO NOT USE!!!
    MULTI_GAME_COOPERATIVE,
    MULTI_GAME_AI_BOTS
    };

//extern SHORT MultiGameType;    // defaults to NONE

// global net vars
// not saved in .CFG file
// used for current game
typedef struct
    {
    LONG KillLimit;
    LONG TimeLimit;
    LONG TimeLimitClock;
    SHORT MultiGameType; // used to be a stand alone global
    BOOL TeamPlay;
    BOOL HurtTeammate;
    BOOL SpawnMarkers;
    BOOL AutoAim;
    BOOL NoRespawn; // for commbat type games
    BOOL Nuke;
    }gNET,*gNETp;

extern gNET gNet;

typedef struct
    {
    int Rules,
        Level,
        Enemy,
        Markers,
        Team,
        HurtTeam,
        Kill,
        Time,
        Color,
        Nuke;
    }AUTO_NET, *AUTO_NETp;
    
extern AUTO_NET Auto;
extern BOOL AutoNet;
    
