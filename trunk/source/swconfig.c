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

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"

//#include "settings.h"
#include "types.h"
#include "develop.h"
#include "scriplib.h"
#include "fx_man.h"
#undef MAXPLAYERS
#include "gamedefs.h"
#include "common.h"
#include "config.h"
#include "function.h"

short GamePlays = 0;

/*
===================
=
= SetGameDefaults
=
===================
*/

void SetGameDefaults( void )
   {
   }

extern BOOL DrawScreen;

void EncodePassword(char *pw)
    {
    long bak_DrawScreen = DrawScreen;
    long bak_randomseed = randomseed;
    long i;
    long len;

    DrawScreen = FALSE;
    randomseed = 1234L;
    
    strupr(pw);

    strupr(pw);
    
    len = strlen(pw);
    for (i = 0; i < len; i++)
        pw[i] += RANDOM_RANGE(26);
    
    randomseed = bak_randomseed;
    DrawScreen = bak_DrawScreen;
    }
   
void DecodePassword(char *pw)
    {
    long bak_DrawScreen = DrawScreen;
    long bak_randomseed = randomseed;
    long i;
    long len;

    DrawScreen = FALSE;
    randomseed = 1234L;
    
    strupr(pw);

    len = strlen(pw);
    for (i = 0; i < len; i++)
        pw[i] -= RANDOM_RANGE(26);
    
    randomseed = bak_randomseed;
    DrawScreen = bak_DrawScreen;
    }

/*
===================
=
= ReadGameSetup
=
===================
*/

void ReadGameSetup( int32 scripthandle )
    {
    long dummy;
    long ret;
    extern char WangBangMacro[10][64];

    for(dummy = 0;dummy < 10;dummy++)
        {
        sprintf(ds,"CommbatMacro#%d",dummy);
        SCRIPT_GetString( scripthandle, "Comm Setup",ds,WangBangMacro[dummy]);
        }

    SCRIPT_GetString( scripthandle, "Options","Rooster",gs.Password);
    DecodePassword(gs.Password);    
        
    // option stuff
    dummy = -1;
    ret = SCRIPT_GetNumber( scripthandle, "Options", "BorderNum",&dummy);
    if (dummy != -1) gs.BorderNum = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Brightness",&dummy);
    if (dummy != -1) gs.Brightness = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "BorderTile",&dummy);
    if (dummy != -1) gs.BorderTile = dummy;
        
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Bobbing",&dummy);
    if (dummy != -1) gs.Bobbing = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Tilting",&dummy);
    if (dummy != -1) gs.Tilting = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Shadows",&dummy);
    if (dummy != -1) gs.Shadows = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "AutoRun",&dummy);
    if (dummy != -1) gs.AutoRun = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Crosshair",&dummy);
    if (dummy != -1) gs.Crosshair = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "AutoAim",&dummy);
    if (dummy != -1) gs.AutoAim = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Messages",&dummy);
    if (dummy != -1) gs.Messages = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Talking",&dummy);
    if (dummy != -1) gs.Talking = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Ambient",&dummy);
    if (dummy != -1) gs.Ambient = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "FxOn",&dummy);
    if (dummy != -1) gs.FxOn = dummy;
       
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "MusicOn",&dummy);
    if (dummy != -1) gs.MusicOn = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Controls", "MouseAiming",&dummy);
    if (dummy != -1) gs.MouseAimingType = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetGameType",&dummy);
    if (dummy != -1) gs.NetGameType = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetLevel",&dummy);
    if (dummy != -1) gs.NetLevel = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetMonsters",&dummy);
    if (dummy != -1) gs.NetMonsters = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetHurtTeammate",&dummy);
    if (dummy != -1) gs.NetHurtTeammate = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetSpawnMarkers",&dummy);
    if (dummy != -1) gs.NetSpawnMarkers = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetTeamPlay",&dummy);
    if (dummy != -1) gs.NetTeamPlay = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetKillLimit",&dummy);
    if (dummy != -1) gs.NetKillLimit = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetTimeLimit",&dummy);
    if (dummy != -1) gs.NetTimeLimit = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "NetColor",&dummy);
    if (dummy != -1) gs.NetColor = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Voxels",&dummy);
    if (dummy != -1) gs.Voxels = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Stats",&dummy);
    if (dummy != -1) gs.Stats = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "MouseAimingOn",&dummy);
    if (dummy != -1) gs.MouseAimingOn = dummy;

    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "MouseInvert",&dummy);
    if (dummy != -1) gs.MouseInvert = dummy;
    
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "Kiwi",&dummy);
    if (dummy != -1) gs.ParentalLock = dummy;
    
    dummy = -1;
    SCRIPT_GetNumber( scripthandle, "Options", "PlayCD",&dummy);
    if (dummy != -1) gs.PlayCD = dummy;
   
#ifdef SW_SHAREWARE   
   dummy = -1;
   SCRIPT_GetNumber( scripthandle, "Options", "Chickens",&dummy);
   if (dummy != -1) GamePlays = dummy;

   {
        time_t start_time, new_time;

        printf
            (
            "\n"            
            "You have played Shadow Warrior %d times.  Please call and order the full\n"
            "version at 1(800)-3DREALMS or see the ORDER.FRM file.\n\n"
            ,GamePlays);
            
        start_time = time(NULL);
        
        while (TRUE)
            {
            new_time = time(NULL);
            
            if (difftime(new_time, start_time) > 1.0)
                break;
            }
   
    GamePlays++;
    }
#endif    
    }

/*
===================
=
= WriteGameSetup
=
===================
*/

// CTW MODIFICATION ENTIRE FILE
// Converted all cases of "false" and "true" to "FALSE" and "TRUE"
// CTW MODIFICATION ENTIRE FILE END

void WriteGameSetup( int32 scripthandle)
   {
   long dummy;
   
   
   dummy = gs.BorderNum;
   SCRIPT_PutNumber( scripthandle, "Options", "BorderNum",dummy,FALSE,FALSE);
   dummy = gs.Brightness;
   SCRIPT_PutNumber( scripthandle, "Options", "Brightness",dummy,FALSE,FALSE);
   dummy = gs.BorderTile;
   SCRIPT_PutNumber( scripthandle, "Options", "BorderTile",dummy,FALSE,FALSE);
   dummy = gs.Bobbing;
   SCRIPT_PutNumber( scripthandle, "Options", "Bobbing",dummy,FALSE,FALSE);
   dummy = gs.Tilting;
   SCRIPT_PutNumber( scripthandle, "Options", "Tilting",dummy,FALSE,FALSE);
   dummy = gs.Shadows;
   SCRIPT_PutNumber( scripthandle, "Options", "Shadows",dummy,FALSE,FALSE);
   dummy = gs.AutoRun;
   SCRIPT_PutNumber( scripthandle, "Options", "AutoRun",dummy,FALSE,FALSE);
   dummy = gs.Crosshair;
   SCRIPT_PutNumber( scripthandle, "Options", "Crosshair",dummy,FALSE,FALSE);
   dummy = gs.AutoAim;
   SCRIPT_PutNumber( scripthandle, "Options", "AutoAim",dummy,FALSE,FALSE);
   dummy = gs.Messages;
   SCRIPT_PutNumber( scripthandle, "Options", "Messages",dummy,FALSE,FALSE);
   dummy = gs.Talking;
   SCRIPT_PutNumber( scripthandle, "Options", "Talking",dummy,FALSE,FALSE);
   dummy = gs.Ambient;
   SCRIPT_PutNumber( scripthandle, "Options", "Ambient",dummy,FALSE,FALSE);
   dummy = gs.FxOn;
   SCRIPT_PutNumber( scripthandle, "Options", "FxOn",dummy,FALSE,FALSE);
   dummy = gs.MouseAimingType;
   SCRIPT_PutNumber( scripthandle, "Controls", "MouseAiming",dummy,FALSE,FALSE);
   
   dummy = gs.MusicOn;
   SCRIPT_PutNumber( scripthandle, "Options", "MusicOn",dummy,FALSE,FALSE);
   
   dummy = gs.NetGameType;
   SCRIPT_PutNumber( scripthandle, "Options", "NetGameType",dummy,FALSE,FALSE);
   dummy = gs.NetLevel;
   SCRIPT_PutNumber( scripthandle, "Options", "NetLevel",dummy,FALSE,FALSE);
   dummy = gs.NetMonsters;
   SCRIPT_PutNumber( scripthandle, "Options", "NetMonsters",dummy,FALSE,FALSE);
   dummy = gs.NetHurtTeammate;
   SCRIPT_PutNumber( scripthandle, "Options", "NetHurtTeammate",dummy,FALSE,FALSE);
   dummy = gs.NetSpawnMarkers;
   SCRIPT_PutNumber( scripthandle, "Options", "NetSpawnMarkers",dummy,FALSE,FALSE);
   dummy = gs.NetTeamPlay;
   SCRIPT_PutNumber( scripthandle, "Options", "NetTeamPlay",dummy,FALSE,FALSE);
   dummy = gs.NetKillLimit;
   SCRIPT_PutNumber( scripthandle, "Options", "NetKillLimit",dummy,FALSE,FALSE);
   dummy = gs.NetTimeLimit;
   SCRIPT_PutNumber( scripthandle, "Options", "NetTimeLimit",dummy,FALSE,FALSE);
   dummy = gs.NetColor;
   SCRIPT_PutNumber( scripthandle, "Options", "NetColor",dummy,FALSE,FALSE);
   dummy = gs.Voxels;
   SCRIPT_PutNumber( scripthandle, "Options", "Voxels",dummy,FALSE,FALSE);
   dummy = gs.MouseAimingOn;
   SCRIPT_PutNumber( scripthandle, "Options", "MouseAimingOn",dummy,FALSE,FALSE);
   dummy = gs.MouseAimingOn;
   SCRIPT_PutNumber( scripthandle, "Options", "MouseAimingOn",dummy,FALSE,FALSE);
   dummy = gs.MouseInvert;
   SCRIPT_PutNumber( scripthandle, "Options", "MouseInvert",dummy,FALSE,FALSE);
   dummy = gs.Stats;
   SCRIPT_PutNumber( scripthandle, "Options", "Stats",dummy,FALSE,FALSE);
   
   EncodePassword(gs.Password);    
   SCRIPT_PutString( scripthandle, "Options","Rooster",gs.Password);
   DecodePassword(gs.Password);    
   
   dummy = gs.ParentalLock;
   SCRIPT_PutNumber( scripthandle, "Options", "Kiwi",dummy,FALSE,FALSE);

   dummy = gs.PlayCD;
   SCRIPT_PutNumber( scripthandle, "Options", "PlayCD",dummy,FALSE,FALSE);
   
#ifdef SW_SHAREWARE   
   dummy = GamePlays;
   SCRIPT_PutNumber( scripthandle, "Options", "Chickens",dummy,FALSE,FALSE);
#endif   
   }

void TermSetup(void)
{
extern BOOL BotMode;
CONFIG_WriteSetup();
RTS_Shutdown();
}
