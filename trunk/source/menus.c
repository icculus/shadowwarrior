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

#include <stdio.h>                                                  
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "condef.h"
#include "build.h"
#include "proto.h"
#include "keys.h"
#include "names2.h"
#include "panel.h"
#include "game.h"
#include "tags.h"
#include "sector.h"
#include "sprite.h"
#include "weapon.h"
#include "player.h"
#include "jsector.h"
#include "control.h"
#include "menus.h"
#include "sw_strs.h"
#include "sndcards.h"
#include "control.h"
#include "pal.h"

#include "function.h"
#include "def.h"
#include "net.h"
#include "fx_man.h"
#include "music.h"
#include "text.h"
// CTW REMOVED
//#include "memcheck.h"
// CTW REMOVED END

//#define PLOCK_VERSION TRUE

extern BOOL cdvalid, enabled;

short TimeLimitTable[9] = {0,3,5,10,15,20,30,45,60}; 

short QuickLoadNum = -1;
char QuickLoadDescrDialog[32];
extern BOOL QuickSaveMode = FALSE;
extern BOOL SavePrompt = FALSE;
extern BOOL InMenuLevel, LoadGameOutsideMoveLoop, LoadGameFromDemo;
extern BYTE RedBookSong[40];
extern BOOL ExitLevel, NewGame;
extern short Level, Skill;
extern BOOL MusicInitialized, FxInitialized;
BOOL MNU_CheckUserMap(MenuItem *item);
BOOL MNU_SaveGameCheck(MenuItem_p item);
BOOL MNU_TeamPlayCheck(MenuItem *item);
BOOL MNU_CoopPlayCheck(MenuItem *item);
BOOL MNU_StatCheck(MenuItem *item);
BOOL MNU_LoadGameCheck(MenuItem *item);
BOOL MNU_TenCheck(MenuItem *item);
static BOOL MNU_TryMusicInit(void);

BOOL MNU_LoadSaveDraw(UserCall call, MenuItem * item);
BOOL MNU_LoadSaveMove(UserCall call, MenuItem * item);

BOOL MenuButtonAutoRun = FALSE;    
BOOL MenuButtonAutoAim = FALSE;    
// misc load-save vars
short LastSaveNum = 99;
char SaveGameDescr[10][80];
char BackupSaveGameDescr[80];
short screen_tile = -1;
int LoadGameFullHeader(short save_num, char *descr, short *level, short *skill);
void LoadGameDescr(short save_num, char *descr);
VOID StopFX(VOID);

BOOL MenuInputMode = FALSE;
SHORT MenuTextShade = 0;
BOOL passwordvalid = FALSE;

BOOL MNU_HurtTeammateCheck(MenuItem *item);
BOOL MNU_TeamPlayChange(void);
// CTW REMOVED
//BOOL MNU_Ten(void);
// CTW REMOVED END

char *MNU_LevelName[30] = {
    "L01: Seppuku Station",
    "L02: Zilla Construction", 
    "L03: Master Leep's Temple",
    "L04: Dark Woods of the Serpent",
    "L05: Rising Son", 
    "L06: Killing Fields",
    "L07: Hara-Kiri Harbor",
    "L08: Zilla's Villa", 
    "L09: Monastery", 
    "L10: Raider of the Lost Wang",
    "L11: Sumo Sky Palace", 
    "L12: Bath House",
    "L13: Unfriendly Skies",
    "L14: Crude Oil",
    "L15: Coolie Mines",
    "L16: Subpen 7",
    "L17: The Great Escape",
    "L18: Floating Fortress",
    "L19: Water Torture", 
    "L20: Stone Rain",  
    "L21: Shanghai Shipwreck",
    "L22: Auto Maul", 
    "L23: Heavy Metal (DM only)", 
    "L24: Ripper Valley (DM only)",
    "L25: House of Wang (DM only)",
    "L26: Lo Wang Rally (DM only)",
    "L27: Ruins of the Ronin (CTF)",
    "L28: Killing Fields (CTF)",
    " "
    };  
    
// Font pic table
unsigned short xlatfont[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 32
    2274, 2294, 2274, 2277, 2278, 2280, 2295, 2282, 2283, 2281, 2286, 2297, 2285,       // 45
    2299, 2301, 2271, 2262, 2263, 2264, 2265, 2266, 2267, 2268, 2269, 2270,     // 57
    2292, 2293, 2296, 2287, 2298, 2300, 2275, 2236, 2237, 2238, 2239, 2240,     // 69
    2241, 2242, 2243, 2244, 2245, 2246, 2247, 2248, 2249, 2250, 2251, 2252,     // 81
    2253, 2254, 2255, 2256, 2257, 2258, 2259, 2260, 2261, 2290, 2289, 2291,     // 93
    2279, 2284, 2295, 2236, 2237, 2238, 2239, 2240, 2241, 2242, 2243, 2244,     // 105
    2245, 2246, 2247, 2248, 2249, 2250, 2251, 2252, 2253, 2254, 2255, 2256,     // 117
    2257, 2258, 2259, 2260, 2261, 2282, 2288, 2283, 2272        // 126
    };

int slidersettings [sldr_max] =
    {
    0, SENSE_DEFAULT, FXVOL_DEFAULT, MUSIC_DEFAULT, SCRSIZE_DEFAULT,
        BRIGHTNESS_DEFAULT, BORDERTILE_DEFAULT, GAMETYPE_DEFAULT, NETLEVEL_DEFAULT,
        MONSTERS_DEFAULT, KILLLIMIT_DEFAULT, TIMELIMIT_DEFAULT, PLAYERCOLOR_DEFAULT
    };
    
short buttonsettings[btn_max];

// EXTERNS ////////////////////////////
#define XDIM    320
#define YDIM    200
extern char DefaultPalette[];
extern BOOL QuitFlag;

void TerminateGame(void);
void ResetKeys(void);

// GLOBALS ////////////////////////////

char playertextbuffer[80];              // Used for various input strings
char playerbuflen = 0;                  // Current length of the string in
                                        // the buffer
char maxtextlen;                        // max length allowed for current

VMODE vmode[7] = 
{
{300,200},
{640,400},
{640,480},
{800,600},
{1024,768},
{1280,1024},
{1600,1200},
};

MenuItem sound_i[] =
{
    {DefButton(btn_music, NULL, "Music"), OPT_XS,                OPT_LINE(0), 1, m_defshade, 0, MNU_TryMusicInit, MNU_MusicCheck},
    {DefSlider(sldr_musicvolume, NULL, "Music Volume"), OPT_XS,  OPT_LINE(1), 1, m_defshade, 0, MNU_TryMusicInit, MNU_MusicCheck},
    {DefInert(NULL, NULL), OPT_XSIDE,                               OPT_LINE(1), NULL, m_defshade, 0, NULL},
    
    {DefButton(btn_sound, NULL, "Sounds"), OPT_XS,               OPT_LINE(2), 1, m_defshade, 0, NULL, MNU_FxCheck},
    {DefSlider(sldr_sndfxvolume, NULL, "Sound Volume"), OPT_XS,  OPT_LINE(3), 1, m_defshade, 0, NULL, MNU_FxCheck},
    {DefInert(NULL, NULL), OPT_XSIDE,                               OPT_LINE(3), NULL, m_defshade, 0, NULL}, 
    
    //{DefButton(btn_talking, NULL, "Talking"), OPT_XS,            OPT_LINE(4), 1, m_defshade, 0, NULL, MNU_FxCheck},
    {DefButton(btn_ambience, NULL, "Ambience"), OPT_XS,          OPT_LINE(4), 1, m_defshade, 0, NULL, MNU_FxCheck},
    {DefButton(btn_flipstereo, NULL, "Flip Stereo"), OPT_XS,     OPT_LINE(5), 1, m_defshade, 0, NULL, MNU_FxCheck},
    //{DefButton(btn_playcd, NULL, "Play CD"), OPT_XS,         OPT_LINE(6), 1, m_defshade, 0, NULL},
    {mt_none}
};

MenuGroup soundgroup = {110,5,"^Sound",sound_i,pic_optionstitl,NULL,m_defshade};

MenuItem parental_i[] =
    {
    {DefButton(btn_parental, NULL, "Kid Mode"), OPT_XS, OPT_LINE(0), 1, m_defshade, 0, NULL},
    {DefOption(KEYSC_P, "Change Password"),              OPT_XS, OPT_LINE(1), 1, m_defshade, 0, MNU_ParentalCustom},
    {mt_none}
    };

MenuGroup parentalgroup = {65, 5, "^Kid Mode", parental_i, pic_newgametitl, NULL, m_defshade, MNU_DoParentalPassword};

MenuItem screen_i[] = 
    {
    {DefSlider(sldr_scrsize, NULL, "Screen Size"), OPT_XS,          OPT_LINE(0), 1, m_defshade, 0, NULL},//, MNU_BorderCheck},
    {DefInert(NULL, NULL), OPT_XSIDE,                               OPT_LINE(0), NULL, m_defshade, 0, NULL},
    
    {DefSlider(sldr_bordertile, NULL, "Border Tile"), OPT_XS,       OPT_LINE(1), 1, m_defshade, 0, NULL},//, MNU_BorderCheck},
    {DefInert(NULL, NULL), OPT_XSIDE,                               OPT_LINE(1), NULL, m_defshade, 0, NULL},

    {DefSlider(sldr_brightness, KEYSC_B, "Brightness"), OPT_XS,     OPT_LINE(2), 1, m_defshade, 0, NULL},
    {DefInert(NULL, NULL), OPT_XSIDE,                               OPT_LINE(2), NULL, m_defshade, 0, NULL},
    {mt_none}
    };

MenuGroup screengroup = {65, 5, "^Screen", screen_i, pic_newgametitl, NULL, m_defshade};

MenuItem mouse_i[] = 
    {
    {DefSlider(sldr_mouse, NULL, "Mouse Speed"), OPT_XS,            OPT_LINE(0), 1, m_defshade, 0, NULL, MNU_MouseCheck},
    {DefInert(NULL, NULL), OPT_XSIDE,                               OPT_LINE(0), NULL, m_defshade, 0, NULL},    // Blank line for mouse
    
    {DefButton(btn_mouse_aim, NULL, "Mouse Aiming"), OPT_XS,        OPT_LINE(1), 1, m_defshade, 0, NULL, MNU_MouseCheck},
    {DefButton(btn_mouse_invert, NULL, "Invert Mouse"), OPT_XS,     OPT_LINE(2), 1, m_defshade, 0, NULL, MNU_MouseCheck},
    {mt_none}
    };

MenuGroup mousegroup = {65, 5, "^Mouse", mouse_i, pic_newgametitl, NULL, m_defshade};

MenuItem options_i[] =
    {
    {DefLayer(NULL, "Screen Menu", &screengroup),OPT_XS,            OPT_LINE(0), 1, m_defshade,0,NULL, NULL},
    {DefLayer(NULL, "Mouse Menu", &mousegroup),OPT_XS,              OPT_LINE(1), 1, m_defshade,0,NULL, MNU_MouseCheck},
    {DefLayer(NULL, "Sound Menu", &soundgroup),OPT_XS,              OPT_LINE(2), 1, m_defshade,0,MNU_TryMusicInit, MNU_MusicFxCheck},
    #ifndef PLOCK_VERSION // No need for this in weener version
    {DefLayer(NULL, "Kid Mode", &parentalgroup),OPT_XS,        OPT_LINE(3), 1, m_defshade,0,NULL, NULL},
    #endif
    {DefButton(btn_messages, NULL, "Messages"), OPT_XS,             OPT_LINE(4), 1, m_defshade, 0, NULL},
//    {DefButton(btn_bobbing, NULL, "View Bobbing"), OPT_XS,          OPT_LINE(7), 1, m_defshade, 0, NULL},
    {DefButton(btn_shadows, NULL, "Shadows"), OPT_XS,               OPT_LINE(5), 1, m_defshade, 0, NULL},
    {DefButton(btn_auto_run, NULL, "Auto Run"), OPT_XS,             OPT_LINE(6), 1, m_defshade, 0, NULL},
    {DefButton(btn_crosshair, NULL, "Crosshair"), OPT_XS,           OPT_LINE(7), 1, m_defshade, 0, NULL},
    {DefButton(btn_auto_aim, NULL, "Auto-Aiming"), OPT_XS,          OPT_LINE(8), 1, m_defshade, 0, NULL},
    #ifndef SW_3DFX
    {DefButton(btn_voxels, NULL, "3D Sprites"), OPT_XS,          OPT_LINE(9), 1, m_defshade, 0, NULL},
    #endif
    {DefButton(btn_stats, NULL, "Level Stats"), OPT_XS,          OPT_LINE(10), 1, m_defshade, 0, NULL, MNU_StatCheck},
    {mt_none}
    };

MenuGroup optiongroup = {100, 5, "^Options", options_i, pic_optionstitl, NULL, m_defshade};

MenuItem skill_i[] =
    {
    {DefOption(KEYSC_E, "^Tiny grasshopper"), 30, 46, pic_easy, m_defshade, 0, MNU_StartGame},
    {DefOption(KEYSC_N, "^I Have No Fear"),       30, 62, pic_normal, m_defshade, 0, MNU_StartGame},
    {DefOption(KEYSC_H, "^Who Wants Wang"),     30, 78, pic_hard, m_defshade, 0, MNU_StartGame},
    {DefOption(KEYSC_I, "^No Pain, No Gain"),     30, 94, pic_impossible, m_defshade, 0, MNU_StartGame},
    {mt_none}
    };

MenuGroup skillgroup = {100, 5, "^Skill", skill_i, pic_newgametitl, NULL, m_defshade, NULL, NULL, 2};

MenuItem episode_i[] =
    {
    {DefLayer(KEYSC_S, "^Enter the Wang",  &skillgroup), 30, 46, pic_episode1, m_defshade, 0, MNU_EpisodeCustom},
    {DefLayer(KEYSC_F, "^Code of Honor", &skillgroup), 30, 78, pic_episode2, m_defshade, 0, MNU_EpisodeCustom, MNU_ShareWareCheck, MNU_ShareWareMessage},
    //{DefLayer(KEYSC_S, NULL, &skillgroup), 60, 30, pic_episode1, m_defshade, 0, MNU_EpisodeCustom},
    //{DefLayer(KEYSC_F, NULL, &skillgroup), 60, 46, pic_episode2, m_defshade, 0, MNU_EpisodeCustom},
    //{DefLayer(KEYSC_T, NULL, &skillgroup), 60, 62, pic_episode3, m_defshade, 0, MNU_EpisodeCustom},
    {mt_none}
    };

MenuGroup episodegroup = {100, 5, "^Episode", episode_i, pic_newgametitl, NULL, m_defshade, MNU_DoEpisodeSelect};

extern char UserMapName[80];

MenuItem network_extra_i[] = 
{
    {DefSlider(sldr_monsters, NULL, "Monsters"),OPT_XS, OPT_LINE(0), 1, m_defshade, 0, NULL, NULL},
    {DefInert(NULL, NULL),               OPT_XSIDE,OPT_LINE(0), NULL, m_defshade, 0, NULL},

    {DefButton(btn_teamplay,     NULL, "Team Play"),    OPT_XS, OPT_LINE(1), 1, m_defshade, 0, MNU_TeamPlayChange, MNU_TeamPlayCheck},
    {DefButton(btn_friendlyfire, NULL, "Hurt Teammate"),OPT_XS, OPT_LINE(2), 1, m_defshade, 0, NULL, MNU_HurtTeammateCheck},
    {DefButton(btn_nuke,         NULL, "Play with Nuke"),OPT_XS, OPT_LINE(3), 1, m_defshade, 0, NULL, NULL},
    {mt_none}
};

MenuGroup networkextragroup = {50, 5, "^Net Options", network_extra_i, pic_newgametitl, NULL, m_defshade};

MenuItem network_i[] =
    {
    {DefSlider(sldr_gametype, NULL, "Game Type"),   OPT_XS,         OPT_LINE(0), 1, m_defshade, 0, NULL, NULL},
    {DefInert(NULL, NULL),                          OPT_XSIDE-38,   OPT_LINE(0), NULL, m_defshade, 0, NULL},

    {DefSlider(sldr_netlevel, NULL, "Level"),       OPT_XS,         OPT_LINE(1), 1, m_defshade, 0, NULL, MNU_CheckUserMap},
    {DefInert(NULL, NULL),                          OPT_XSIDE-70,   OPT_LINE(1), NULL, m_defshade, 0, NULL, MNU_CheckUserMap},
    {DefInert(NULL, " "),                           OPT_XS,         OPT_LINE(2), pic_episode1, m_defshade, 0, NULL, MNU_CheckUserMap},

    {DefButton(btn_markers,      NULL, "Markers"),  OPT_XS,         OPT_LINE(3), 1, m_defshade, 0, NULL, NULL},

    {DefSlider(sldr_killlimit, NULL, "Kill Limit"),OPT_XS,          OPT_LINE(4), 1, m_defshade, 0, NULL, MNU_CoopPlayCheck},
    {DefInert(NULL, NULL),               OPT_XSIDE,                 OPT_LINE(4), NULL, m_defshade, 0, NULL, MNU_CoopPlayCheck},

    {DefSlider(sldr_timelimit, NULL, "Time Limit"),OPT_XS,          OPT_LINE(5), 1, m_defshade, 0, NULL, MNU_CoopPlayCheck},
    {DefInert(NULL, NULL),               OPT_XSIDE,                 OPT_LINE(5), NULL, m_defshade, 0, NULL, MNU_CoopPlayCheck},

    {DefSlider(sldr_playercolor, NULL, "Player Color"),OPT_XS,      OPT_LINE(6), 1, m_defshade, 0, NULL, NULL},
    {DefInert(NULL, NULL),               OPT_XSIDE,                 OPT_LINE(6), NULL, m_defshade, 0, NULL},

    {DefLayer(NULL, "Other Options", &networkextragroup),OPT_XS,    OPT_LINE(7), 1, m_defshade,0,NULL, NULL},
    
    {DefInert(NULL, UserMapName),         OPT_XSIDE,                OPT_LINE(8), pic_episode1, m_defshade, 0, NULL},
    {DefOption(KEYSC_S, "Start Game"),   OPT_XS,                    OPT_LINE(8), pic_episode1, m_defshade, 0, MNU_StartNetGame},
    
    {mt_none}
    
    };

MenuGroup networkgroup = {50, 5, "^Network Game", network_i, pic_newgametitl, NULL, m_defshade};


MenuItem load_i[] =
    {
#define SD_YSTART 26
#define SD_XSTART 5
#define SD_YOFF 13
#define SD_LINE(line) (SD_YSTART + (line * SD_YOFF))
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(0), NULL, m_defshade, 0, MNU_GetLoadCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(1), NULL, m_defshade, 0, MNU_GetLoadCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(2), NULL, m_defshade, 0, MNU_GetLoadCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(3), NULL, m_defshade, 0, MNU_GetLoadCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(4), NULL, m_defshade, 0, MNU_GetLoadCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(5), NULL, m_defshade, 0, MNU_GetLoadCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(6), NULL, m_defshade, 0, MNU_GetLoadCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(7), NULL, m_defshade, 0, MNU_GetLoadCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(8), NULL, m_defshade, 0, MNU_GetLoadCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(9), NULL, m_defshade, 0, MNU_GetLoadCustom},
    {mt_none}
    };

MenuItem save_i[] =
    {
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(0), NULL, m_defshade, 0, MNU_GetSaveCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(1), NULL, m_defshade, 0, MNU_GetSaveCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(2), NULL, m_defshade, 0, MNU_GetSaveCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(3), NULL, m_defshade, 0, MNU_GetSaveCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(4), NULL, m_defshade, 0, MNU_GetSaveCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(5), NULL, m_defshade, 0, MNU_GetSaveCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(6), NULL, m_defshade, 0, MNU_GetSaveCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(7), NULL, m_defshade, 0, MNU_GetSaveCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(8), NULL, m_defshade, 0, MNU_GetSaveCustom},
    {DefOption(NULL, NULL), SD_XSTART, SD_LINE(9), NULL, m_defshade, 0, MNU_GetSaveCustom},
    {mt_none}
    };

// No actual submenus for this, just quit text.
MenuGroup quitgroup = {0, 0, NULL, NULL, NULL, NULL, m_defshade, MNU_QuitCustom};
MenuGroup quickloadgroup = {0, 0, NULL, NULL, NULL, NULL, m_defshade, MNU_QuickLoadCustom};
MenuGroup ordergroup = {0, 0, NULL, NULL, NULL, NULL, m_defshade, MNU_OrderCustom};

// save and load function calls
MenuGroup SaveGameGroup = {100, 5, "^Save Game", save_i, pic_savegame, NULL, m_defshade, MNU_LoadSaveDraw, MNU_LoadSaveMove};
MenuGroup LoadGameGroup = {100, 5, "^Load Game", load_i, pic_loadgame, NULL, m_defshade, MNU_LoadSaveDraw, MNU_LoadSaveMove};

#define MAIN_YSTART 32
#define MAIN_YOFF 17
#define MAIN_XSTART 55
#define MAIN_LINE(line) (MAIN_YSTART + (MAIN_YOFF * line))
MenuItem main_i[] =
    {
    {DefLayer(KEYSC_N, "^New Game", &episodegroup),      MAIN_XSTART, MAIN_LINE(0), pic_newgame, m_defshade, 0, NULL},
// CTW REMOVED
//    {DefOption(KEYSC_P, "^Play it on TEN!"),             MAIN_XSTART, MAIN_LINE(1), pic_orderinfo, m_defshade, 0, MNU_Ten, MNU_TenCheck},
// CTW REMOVED END
// CTW MODIFICATION
// Moved all the remaining menu options up one index.
    {DefLayer(KEYSC_L, "^Load Game", &LoadGameGroup),    MAIN_XSTART, MAIN_LINE(1), pic_load, m_defshade, 0, NULL, MNU_LoadGameCheck},
    {DefLayer(KEYSC_S, "^Save Game", &SaveGameGroup),    MAIN_XSTART, MAIN_LINE(2), pic_save, m_defshade, 0, NULL, MNU_SaveGameCheck},
    {DefLayer(KEYSC_O, "^Options", &optiongroup),        MAIN_XSTART, MAIN_LINE(3), pic_options, m_defshade, 0, NULL},
    #ifndef SW_SHAREWARE
    {DefLayer(KEYSC_H, "^Cool Stuff", &ordergroup),         MAIN_XSTART, MAIN_LINE(4), pic_orderinfo, m_defshade, 0, NULL},
    #else
    {DefLayer(KEYSC_H, "^How to Order", &ordergroup),    MAIN_XSTART, MAIN_LINE(4), pic_orderinfo, m_defshade, 0, NULL},
    #endif
    {DefLayer(KEYSC_Q, "^Quit", &quitgroup),             MAIN_XSTART, MAIN_LINE(5), pic_quit, m_defshade, 0, NULL},
// CTW MODIFICATION END
    {mt_none}
    };

MenuGroup maingroup = {160, 15, NULL, main_i, pic_shadow_warrior, NULL, m_defshade};

CTLType ControlPanelType;

#define MaxLayers    10         // Maximum layers deep a menu can go
short       menuarrayptr;       // Index into menuarray
MenuGroup   *menuarray[MaxLayers], *currentmenu;
BOOL UsingMenus = FALSE;

#define MAXDIALOG       2       // Maximum number of dialog strings allowed
char *dialog[MAXDIALOG];

// Global menu setting values ////////////////////////////////////////////////////////////////////
// Mouse slider vars
#define SENSE_MIN               75
#define SENSE_MUL               10
int SENSITIVITY = SENSE_MIN + (SENSE_DEFAULT * SENSE_MUL);

// Sound vars
#define FX_MIN                  0
#define MUSIC_MIN               0
#define VOL_MUL                 16

// User input data for all devices
UserInput mnu_input, mnu_input_buffered, order_input_buffered;

// Menu function call back pointer for multiplay menus
BOOL(*cust_callback) (UserCall call, MenuItem_p item);
UserCall cust_callback_call;
MenuItem_p cust_callback_item;

// Prototypes ///////////////////////////////////////////////////////////////////////////////////

void MNU_DrawMenu(void);
void MNU_DrawString(short x, short y, char *string, short shade, short pal);
static void MNU_ClearDialog(void);
static BOOL MNU_Dialog(void);

// F U N C T I O N S ////////////////////////////////////////////////////////////////////////////

// CUSTOM ROUTINES ////////////////////////////////////////////////////////////////////////////////
// CTW REMOVED
/*
BOOL
MNU_Ten(void)
    {
    TEN_Setup();
    
    return(FALSE);
    }
*/
// CTW REMOVED END
BOOL 
MNU_DoEpisodeSelect(UserCall call, MenuItem * item)
    {
    short w,h;
    char TempString[80];
    char *extra_text;

    extra_text = "Four levels (Shareware Version)";
    MNU_MeasureString(extra_text, &w, &h);
    MNU_DrawString(30, 63, extra_text, 1, 16);
    extra_text = "Eighteen levels (Full Version Only)";
    MNU_MeasureString(extra_text, &w, &h);
    MNU_DrawString(30, 96, extra_text, 1, 16);

    }
    
BOOL
MNU_DoParentalPassword(UserCall call, MenuItem_p item)
    {
    short w,h;
    signed char MNU_InputString(char *, short);
    static BOOL cur_show;
    char TempString[80];
    char *extra_text;


    extra_text = "This mode should remove most of the";
    MNU_MeasureString(extra_text, &w, &h);
    MNU_DrawString(TEXT_XCENTER(w), 60, extra_text, 1, 16);
    extra_text = "offensive content.  We still recommend";
    MNU_MeasureString(extra_text, &w, &h);
    MNU_DrawString(TEXT_XCENTER(w), 70, extra_text, 1, 16);
    extra_text = "you review the game prior to play.";
    MNU_MeasureString(extra_text, &w, &h);
    MNU_DrawString(TEXT_XCENTER(w), 80, extra_text, 1, 16);

    // get input
    if(MenuInputMode)
    {
    switch(MNU_InputString(MessageInputString, 80))
        {
        case -1: // Cancel Input (pressed ESC) or Err
            KB_ClearKeysDown();
            KB_FlushKeyboardQueue();
            MenuInputMode = FALSE;
            memset(MessageInputString, '\0', sizeof(MessageInputString));
            break;
        case FALSE: // Input finished (RETURN)
            if (MessageInputString[0] == '\0')
                {
                MenuInputMode = FALSE;
                KB_ClearKeysDown();
                KB_FlushKeyboardQueue();
                memset(MessageInputString, '\0', sizeof(MessageInputString));
                }
            else
                {
                MenuInputMode = FALSE;
                KB_ClearKeysDown();
                KB_FlushKeyboardQueue();

                if(gs.Password[0] != '\0' && passwordvalid == FALSE)
                    {
                    if(!stricmp(gs.Password,MessageInputString))
                        {
                        passwordvalid = TRUE;
                        if (currentmenu->cursor == 0 && gs.ParentalLock == TRUE)
                            {
                            buttonsettings[btn_parental] = gs.ParentalLock = FALSE;
                            if (!InMenuLevel)
                            JS_ToggleLockouts();
                            }
                            
                        if (currentmenu->cursor == 1) // Is it on the password line?
                            {
                            MenuInputMode = TRUE;
                            KB_ClearKeysDown();
                            KB_FlushKeyboardQueue();
                            }
                        //memset(gs.Password, '\0', sizeof(gs.Password));
                        } 
                    } else
                    {
                    if (currentmenu->cursor == 1) // Is it on the password line?
                       {
                       strcpy(gs.Password,MessageInputString);
                       passwordvalid = FALSE;
                       }
                    }

                memset(MessageInputString, '\0', sizeof(MessageInputString));
                }
            break;
        case TRUE: // Got input
            break;
        }

        //CON_Message("Password = '%s'",gs.Password);
        //CON_Message("Passwordvalid = %d",passwordvalid);

        if(gs.Password[0] != '\0' && passwordvalid == FALSE && currentmenu->cursor == 1)
            {
            sprintf(TempString,"Enter Old Password");
            MNU_MeasureString(TempString, &w, &h);
            MNU_DrawString(TEXT_XCENTER(w), MESSAGE_LINE-10, TempString,1,16);
            }
        else 
        if (passwordvalid == TRUE && currentmenu->cursor == 1)
            {
            sprintf(TempString,"Enter New Password");
            MNU_MeasureString(TempString, &w, &h);
            MNU_DrawString(TEXT_XCENTER(w), MESSAGE_LINE-10, TempString,1,16);
            }   
        else
            {
            sprintf(TempString,"Enter Password");
            MNU_MeasureString(TempString, &w, &h);
            MNU_DrawString(TEXT_XCENTER(w), MESSAGE_LINE-10, TempString,1,16);
            }   
        
        MNU_MeasureString(MessageInputString, &w, &h);

        cur_show ^= 1;    
        if (cur_show)
            {
            MNU_DrawString(TEXT_XCENTER(w), MESSAGE_LINE, MessageInputString,1,16);
            rotatesprite((TEXT_XCENTER(w)+w+7)<<16,(MESSAGE_LINE+3)<<16,64<<9,0,COINCURSOR+((totalclock>>3)%7),0,0,MenuDrawFlags,0,0,xdim-1,ydim-1);
            }
        else
            {
            MNU_DrawString(TEXT_XCENTER(w), MESSAGE_LINE, MessageInputString,1,16);
            rotatesprite((TEXT_XCENTER(w)+w+7)<<16,(MESSAGE_LINE+3)<<16,64<<9,0,COINCURSOR+((totalclock>>3)%7),0,0,MenuDrawFlags,0,0,xdim-1,ydim-1);
            }    

    }

    return(TRUE);
    }

static BOOL
MNU_ParentalCustom(void)
    {

    if (MenuInputMode)
        {
        // toggle edit mode
        MenuInputMode = FALSE;
        memset(MessageInputString, '\0', sizeof(MessageInputString));
        }
    else
        {
        // clear keyboard buffer
        while (KB_KeyWaiting())
            {
            if (KB_Getch() == 0)
                KB_Getch();
            }

        // toggle edit mode
        MenuInputMode = TRUE;
        }

    return (TRUE);
    }

BOOL 
MNU_OrderCustom(UserCall call, MenuItem * item)
    {
    static signed char on_screen = 0,last_screen = 0;
    UserInput order_input;
    static long limitmove=0;
    UserInput tst_input;
    BOOL select_held = FALSE;
    long zero = 0;
    static BOOL DidOrderSound = FALSE;
    short choose_snd;
    static int wanghandle;

    #ifndef SW_SHAREWARE    
    static short OrderScreen[] = 
        {
        5262,
        5261,
        4979,
        5111,
        5118,
        5113,
        //5111,
        //5118,
        //4979,
        //5261,
        //5262
        };
    #else
    static short OrderScreen[] = 
        {
        5262,
        5110,
        5112,
        5113,
        5111,
        5118,
        4979
        };
    #endif
    
    // Ignore the special touchup calls
    if (call == uc_touchup)
        return (TRUE);

    if (cust_callback == NULL)
        {
        if (call != uc_setup)
            return (FALSE);
        }

    #ifdef SW_SHAREWARE    
    if (on_screen == 0 && !DidOrderSound)        
        {        
        DidOrderSound = TRUE;        
        choose_snd = STD_RANDOM_RANGE(1000);        
        if (choose_snd > 500 && !FX_SoundActive(wanghandle))        
            wanghandle = PlaySound(DIGI_WANGORDER1, &zero, &zero, &zero, v3df_dontpan);        
        else        
        if (!FX_SoundActive(wanghandle))            
            wanghandle = PlaySound(DIGI_WANGORDER2, &zero, &zero, &zero, v3df_dontpan);        
        }            
    #endif    
                
    order_input.button0 = order_input.button1 = FALSE;
    order_input.dir = dir_None;
    
    // Zero out the input structure
    tst_input.button0 = tst_input.button1 = FALSE;
    tst_input.dir = dir_None;
    
    if(!select_held)
        {
        CONTROL_GetUserInput(&tst_input);
        //order_input_buffered.dir = tst_input.dir;
        // Support a few other keys too
        if (KEY_PRESSED(KEYSC_SPACE)||KEY_PRESSED(KEYSC_ENTER))
            {
            KEY_PRESSED(KEYSC_SPACE) = FALSE;
            KEY_PRESSED(KEYSC_ENTER) = FALSE;
            tst_input.dir = dir_South;
            }
        }
    
    if (order_input_buffered.button0 || order_input_buffered.button1 || order_input_buffered.dir != dir_None)    
        {
        if(tst_input.button0 == order_input_buffered.button0 && 
           tst_input.button1 == order_input_buffered.button1 && 
           tst_input.dir == order_input_buffered.dir) 
            {
            select_held = TRUE;
            } 
        else
            {
            if (labs(totalclock - limitmove) > 7)
                {
                order_input.button0 = order_input_buffered.button0;
                order_input.button1 = order_input_buffered.button1;
                order_input.dir = order_input_buffered.dir;

                order_input_buffered.button0 = tst_input.button0;
                order_input_buffered.button1 = tst_input.button1;
                order_input_buffered.dir = tst_input.dir;

                limitmove = totalclock;
                }
            }    
        } 
    else
        {
        select_held = FALSE;    
        order_input_buffered.button0 = tst_input.button0;
        order_input_buffered.button1 = tst_input.button1;
        order_input_buffered.dir = tst_input.dir;
        }

    if (!KEY_PRESSED(KEYSC_ESC))
        {
        cust_callback = MNU_OrderCustom;
        cust_callback_call = call;
        cust_callback_item = item;
        }
    else
        {    
        KEY_PRESSED(KEYSC_ESC) = FALSE;
        cust_callback = NULL;
        DidOrderSound = FALSE;
        on_screen = 0;
        ExitMenus();
        }

    if (order_input.dir == dir_North)
        {
        on_screen--;
        }
    else 
    if (order_input.dir == dir_South)
        {
        on_screen++;
        }
    else 
    if (order_input.dir == dir_West)
        {
        on_screen--;
        }
    else    
    if (order_input.dir == dir_East)
        {
        on_screen++;
        }

// CTW MODIFICATION
// I reversed the logic in here to allow the user to loop around.
// Yeah... I changed default behavior just because I wanted to. 
// AND YOU CAN'T STOP ME SUCKER!!!
    if(on_screen < 0) 
        on_screen = SIZ(OrderScreen)-1;
        
    if(on_screen > SIZ(OrderScreen)-1) 
        on_screen = 0;
// CTW MODIFICATION END
        
    rotatesprite(0,0,RS_SCALE,0,OrderScreen[on_screen],0,0,
        (ROTATE_SPRITE_CORNER|ROTATE_SPRITE_SCREEN_CLIP|ROTATE_SPRITE_NON_MASK|ROTATE_SPRITE_IGNORE_START_MOST),
        0, 0, xdim-1, ydim-1);

    //KB_ClearKeysDown();

    return (TRUE);
    }

short EpisodeMenuSelection;

void 
ExitMenus(void)
    {
    ControlPanelType = ct_mainmenu;
    UsingMenus = FALSE;
    
    if (LoadGameOutsideMoveLoop)
        return;
        
    ResumeGame();
    SetRedrawScreen(&Player[myconnectindex]);
    }

static BOOL
MNU_StartGame(void)
    {
    PLAYERp pp = Player + screenpeek;
    int handle = 0;
    long zero = 0;

    // always assumed that a demo is playing

    ready2send = 0;
    Skill = currentmenu->cursor;
    
    if (EpisodeMenuSelection >= 1)
        Level = 5;
    else    
        Level = 1;
        
    ExitMenus();
    DemoPlaying = FALSE;
    ExitLevel = TRUE;
    NewGame = TRUE;
    DemoMode = FALSE;
    CameraTestMode = FALSE;
    
    //InitNewGame();
    
    if(Skill == 0)
        handle = PlaySound(DIGI_TAUNTAI3,&zero,&zero,&zero,v3df_none);
    else
    if(Skill == 1)
        handle = PlaySound(DIGI_NOFEAR,&zero,&zero,&zero,v3df_none);
    else
    if(Skill == 2)
        handle = PlaySound(DIGI_WHOWANTSWANG,&zero,&zero,&zero,v3df_none);
    else
    if(Skill == 3)
        handle = PlaySound(DIGI_NOPAIN,&zero,&zero,&zero,v3df_none);

    if (handle > FX_Ok)
        while(FX_SoundActive(handle))
            ;
    
    return (TRUE);
    }

VOID ResetMenuInput(VOID)
    {
    cust_callback = NULL;
    InputMode = FALSE;
    }    

BOOL
MNU_StartNetGame(void)
    {
    extern BOOL ExitLevel, ShortGameMode, DemoInitOnce, FirstTimeIntoGame;
    extern short Level, Skill;
	// CTW REMOVED
	//extern int gTenActivated;
	// CTW REMOVED END
    long pnum;
    
    // always assumed that a demo is playing

    ready2send = 0;
    // Skill can go negative here
    Skill = gs.NetMonsters-1;
    Level = gs.NetLevel + 1;
    if (!AutoNet)
        ExitMenus();
    DemoPlaying = FALSE;
    ExitLevel = TRUE;
    NewGame = TRUE;
    // restart demo for multi-play mode
    DemoInitOnce = FALSE;
    ResetMenuInput();

	// TENSW: return if a joiner
	if (/* CTW REMOVED gTenActivated && */!AutoNet && FirstTimeIntoGame)
		return TRUE;
    
    // need to set gNet vars for self
    // everone else gets a packet to set them
    gNet.AutoAim            = gs.AutoAim;
    gNet.SpawnMarkers       = gs.NetSpawnMarkers;
    gNet.HurtTeammate       = gs.NetHurtTeammate;
    gNet.Nuke               = gs.NetNuke;
    gNet.KillLimit          = gs.NetKillLimit*10;
    gNet.TimeLimit          = TimeLimitTable[gs.NetTimeLimit]*60*120;
        
    if (ShortGameMode)
        {
        gNet.KillLimit /= 10;
        gNet.TimeLimit /= 2;
        }
        
    gNet.TimeLimitClock     = gNet.TimeLimit;
    gNet.TeamPlay           = gs.NetTeamPlay;
    gNet.MultiGameType      = gs.NetGameType+1;
    
    if (gNet.MultiGameType == MULTI_GAME_COMMBAT_NO_RESPAWN)
        {
        gNet.MultiGameType = MULTI_GAME_COMMBAT;
        gNet.NoRespawn = TRUE;
        }
    else    
        {
        gNet.NoRespawn = FALSE;
        }
    
    if (CommEnabled)
        {
        PACKET_NEW_GAME p;
        
        p.PacketType = PACKET_TYPE_NEW_GAME;
        p.Level = Level;
        p.Skill = Skill;
        p.GameType = gs.NetGameType;
        p.AutoAim = gs.AutoAim;
        p.HurtTeammate = gs.NetHurtTeammate;
        p.TeamPlay = gs.NetTeamPlay;
        p.SpawnMarkers = gs.NetSpawnMarkers;
        p.KillLimit = gs.NetKillLimit;
        p.TimeLimit = gs.NetTimeLimit;
        p.Nuke = gs.NetNuke;
        
		netbroadcastpacket((char *)(&p), sizeof(p));		// TENSW
        }

        
    return (TRUE);
    }

    
static BOOL
MNU_EpisodeCustom(void)
    {
    EpisodeMenuSelection = currentmenu->cursor;

    return (TRUE);
    }

BOOL
MNU_QuitCustom(UserCall call, MenuItem_p item)
    {
    int select;
    int ret;
    extern BOOL DrawScreen;

    // Ignore the special touchup calls
    if (call == uc_touchup)
        return (TRUE);

    if (cust_callback == NULL)
        {
        if (call != uc_setup)
            return (FALSE);

        memset(dialog, 0, sizeof(dialog));

        dialog[0] = S_QUITYN;
        }
    
    ret = MNU_Dialog();
    
    if (DrawScreen)    
        return(TRUE);

    if (!ret)
        {
        if (!mnu_input.button1 && !KB_KeyPressed(sc_N))
            {
            cust_callback = MNU_QuitCustom;
            cust_callback_call = call;
            cust_callback_item = item;
            }
        else
            {    
            cust_callback = NULL;
            ExitMenus();
            }
        }
    else
        {
        cust_callback = NULL;
        ExitMenus();
        }

    if (KB_KeyPressed(sc_Y))
        {
        if (CommPlayers >= 2)
            MultiPlayQuitFlag = TRUE;
        else    
            QuitFlag = TRUE;
        
        ExitMenus();
        }

    KB_ClearKeysDown();

    return (TRUE);
    }

BOOL
MNU_QuickLoadCustom(UserCall call, MenuItem_p item)
    {
    int select;
    extern BOOL ReloadPrompt;
    long bak;
    PLAYERp pp = Player + myconnectindex;
    extern short GlobInfoStringTime;
    extern BOOL DrawScreen;
    long ret;

    if (cust_callback == NULL)
        {
        if (call != uc_setup)
            return (FALSE);
    
        memset(dialog, 0, sizeof(dialog));
    
        dialog[0] = "Load saved game";
        sprintf(QuickLoadDescrDialog,"\"%s\" (Y/N)?",SaveGameDescr[QuickLoadNum]);
        dialog[1] = QuickLoadDescrDialog;
        }

    // Ignore the special touchup calls
    if (call == uc_touchup)
        return (TRUE);

    ret = MNU_Dialog();

    if (DrawScreen)
        {
        return(TRUE);
        }

    if (ret == FALSE)
        {
        if (KB_KeyPressed(sc_N) || KB_KeyPressed(sc_Space) || KB_KeyPressed(sc_Enter))
            {    
            cust_callback = NULL;
            if (ReloadPrompt)
                {
                ReloadPrompt = FALSE;
                bak = GlobInfoStringTime;
                GlobInfoStringTime = 999;
                PutStringInfo(pp, "Press SPACE to restart");
                GlobInfoStringTime = bak;
                }
            
            KB_ClearKeysDown();
            ExitMenus();
            }
        else
            {
            cust_callback = MNU_QuickLoadCustom;
            cust_callback_call = call;
            cust_callback_item = item;
            }
        }
    else
        {
        // Y pressed
        cust_callback = NULL;
        
        KB_ClearKeysDown();
        LoadSaveMsg("Loading...");

        PauseAction();
        
        ReloadPrompt = FALSE;
        if (LoadGame(QuickLoadNum) == -1)
            {
            ResumeAction();
            return (FALSE);
            }

        ready2send = 1;
        LastSaveNum = -1;
        
        // do a load game here
        KB_ClearKeysDown();
        ExitMenus();
        }
        
    KB_ClearKeysDown();

    return (TRUE);
    }
    
// MENU FUNCTIONS /////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////
// Set some global menu related defaults
////////////////////////////////////////////////
void 
MNU_InitMenus(void)
    {
    pClearTextLine(Player + myconnectindex, TEXT_INFO_LINE(0));
        
    slidersettings[sldr_mouse] = gs.MouseSpeed/(MOUSE_SENS_MAX_VALUE/SLDR_MOUSESENSEMAX);
    
    slidersettings[sldr_sndfxvolume] = gs.SoundVolume / (FX_VOL_MAX_VALUE/SLDR_SNDFXVOLMAX);
    slidersettings[sldr_musicvolume] = gs.MusicVolume / (MUSIC_VOL_MAX_VALUE/SLDR_MUSICVOLMAX);
    slidersettings[sldr_scrsize] = gs.BorderNum;
    slidersettings[sldr_brightness] = gs.Brightness;
    slidersettings[sldr_bordertile] = gs.BorderTile;
    
    buttonsettings[btn_auto_run] = gs.AutoRun;
    buttonsettings[btn_auto_aim] = gs.AutoAim;
    buttonsettings[btn_messages] = gs.Messages;
    buttonsettings[btn_crosshair] = gs.Crosshair;
    //    buttonsettings[btn_bobbing] = gs.Bobbing;
    buttonsettings[btn_shadows] = gs.Shadows; 
    
    buttonsettings[btn_mouse_aim] = gs.MouseAimingType;      
    buttonsettings[btn_mouse_invert] = gs.MouseInvert;    
    buttonsettings[btn_sound] = gs.FxOn;      
    buttonsettings[btn_music] = gs.MusicOn;   
    buttonsettings[btn_talking] = gs.Talking; 
    
    buttonsettings[btn_voxels] = gs.Voxels;     
    buttonsettings[btn_ambience] = gs.Ambient;     
    buttonsettings[btn_playcd] = gs.PlayCD;     
    buttonsettings[btn_flipstereo] = gs.FlipStereo;
    buttonsettings[btn_stats] = gs.Stats;

    slidersettings[sldr_gametype] = gs.NetGameType;
    slidersettings[sldr_netlevel] = gs.NetLevel;
    slidersettings[sldr_monsters] = gs.NetMonsters;
    slidersettings[sldr_killlimit] = gs.NetKillLimit;
    slidersettings[sldr_timelimit] = gs.NetTimeLimit;
    slidersettings[sldr_playercolor] = gs.NetColor;
    
    buttonsettings[btn_nuke] = gs.NetNuke;
    buttonsettings[btn_markers] = gs.NetSpawnMarkers;
    buttonsettings[btn_teamplay] = gs.NetTeamPlay;
    buttonsettings[btn_friendlyfire] = gs.NetHurtTeammate;
    buttonsettings[btn_parental] = gs.ParentalLock;

    // Distinguish between Single or Multiplay for new game menu types
    if(numplayers > 1)
        main_i[0].child = &networkgroup;
    else
//        #ifdef SW_SHAREWARE
        main_i[0].child = &episodegroup;        
//        #else
//        main_i[0].child = &skillgroup;        
//        #endif    
    }

////////////////////////////////////////////////
// Measure the pixel width of a graphic string
////////////////////////////////////////////////
static char lg_xlat_num[] = {0,1,2,3,4,5,6,7,8,9};
#define FONT_LARGE_ALPHA 3706
#define FONT_LARGE_DIGIT 3732

void
MNU_MeasureStringLarge(char *string, short *w, short *h)
    {
    short ndx, width, height;
    char c;
    short pic;

    width = 0;
    height = *h;

    for (ndx = 0; (c = string[ndx]) != 0; ndx++)
        {
        if (isalpha(c))
            {
            c = toupper(c);
            pic = FONT_LARGE_ALPHA + (c - 'A');
            }
        else    
        if (isdigit(c))
            {
            pic = FONT_LARGE_DIGIT + lg_xlat_num[(c - '0')];
            }
        else
        if (c == ' ')
            {
            width += 10;                 // Special case for space char
            continue;
            }
        else        
            {
            continue;
            }    
        
        width += tilesizx[pic]+1;
        if (height < tilesizx[pic])
            height = tilesizy[pic];
        }

    *w = width;
    *h = height;
    }

////////////////////////////////////////////////
// Draw a string using a graphic font
////////////////////////////////////////////////
void 
MNU_DrawStringLarge(short x, short y, char *string)
    {
    int ndx, offset;
    char c;
    short pic;
    
    offset = x;

    for (ndx = 0; (c = string[ndx]) != 0; ndx++)
        {
        if (isalpha(c))
            {
            c = toupper(c);
            pic = FONT_LARGE_ALPHA + (c - 'A');
            }
        else    
        if (isdigit(c))
            {
            pic = FONT_LARGE_DIGIT + lg_xlat_num[(c - '0')];
            }
        else
        if (c == ' ')
            {
            offset += 10;
            continue;
            }
        else        
            {
            continue;
            }    
        
        rotatesprite(offset << 16, y << 16, MZ, 0, pic, MenuTextShade, 0, MenuDrawFlags|ROTATE_SPRITE_CORNER, 0, 0, xdim - 1, ydim - 1);
        offset += tilesizx[pic] + 1;
        }

    }

    
////////////////////////////////////////////////
// Measure the pixel width of a graphic string
////////////////////////////////////////////////
void
MNU_MeasureString(char *string, short *w, short *h)
    {
    short ndx, width, height;
    char c;
    short ac;
    
    if (string[0] == '^')
        {
        MNU_MeasureStringLarge(&string[1], w, h);
        return;
        }

    width = 0;
    height = *h;

    for (ndx = 0; (c = string[ndx]) != 0; ndx++)
        {
        ac = c - '!' + STARTALPHANUM;
        if( (ac < STARTALPHANUM || ac > ENDALPHANUM)  && c != asc_Space )
            break;

        if (c > asc_Space && c < 127)
            {
            width += tilesizx[ac];
            if (height < tilesizx[ac])
                height = tilesizy[ac];
            }
        else 
        if (c == asc_Space)
            width += 4;                 // Special case for space char
        }

    *w = width;
    *h = height;
    }

////////////////////////////////////////////////
// Draw a string using a graphic font
//
// MenuTextShade and MenuDrawFlags
////////////////////////////////////////////////
void 
MNU_DrawString(short x, short y, char *string, short shade, short pal)
{
    int ndx, offset;
    char c;
    short ac;

    if (string[0] == '^')
    {
        MNU_DrawStringLarge(x,y, &string[1]);
        return;
    }
        
    offset = x;

    for (ndx = 0; (c = string[ndx]) != 0; ndx++)
    {
        ac = c - '!' + STARTALPHANUM;
        if( (ac < STARTALPHANUM || ac > ENDALPHANUM)  && c != asc_Space )
            break;

        if (c > asc_Space && c < 127)
        {
            rotatesprite(offset<<16,y<<16,MZ,0,ac, shade, pal, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
            offset += tilesizx[ac];
        } else 
        if (c == asc_Space)
            offset += 4;                // Special case for space char
    }

}
/*   Original code
void 
MNU_DrawString(short x, short y, char *string)
{
    int ndx, offset;
    char c;

    if (string[0] == '^')
    {
        MNU_DrawStringLarge(x,y, &string[1]);
        return;
    }
        
    offset = x;

    for (ndx = 0; (c = string[ndx]) != 0; ndx++)
    {
        if (c > asc_Space && c < 127)
        {
            rotatesprite(offset << 16, y << 16, MZ, 0, xlatfont[c], MenuTextShade, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
            offset += tilesizx[xlatfont[c]];
        } else 
        if (c == asc_Space)
            offset += 4;                // Special case for space char
    }

}
*/

////////////////////////////////////////////////
// Measure the pixel width of a small font string
////////////////////////////////////////////////
void
MNU_MeasureSmallString(char *string, short *w, short *h)
    {
    short ndx, width, height;
    char c;
    short ac;
    
    width = 0;
    height = *h;

    for (ndx = 0; (c = string[ndx]) != 0; ndx++)
        {
        ac = (c - '!') + 2930;
        if( (ac < 2930 || ac > 3023) && c != asc_Space )
            break;

        if (c > asc_Space && c < 127)
            {
            width += tilesizx[ac];
            if (height < tilesizx[ac])
                height = tilesizy[ac];
            }
        else 
        if (c == asc_Space)
            width += 4;                 // Special case for space char
        }

    *w = width;
    *h = height;
    }

////////////////////////////////////////////////
// Draw a string using a small graphic font
////////////////////////////////////////////////
void 
MNU_DrawSmallString(short x, short y, char *string, short shade, short pal)
{
    int ndx;
    char c;
    short ac,offset;


    offset = x;

    for (ndx = 0; (c = string[ndx]) != 0; ndx++)
    {
        ac = c - '!' + 2930;
        if( (ac < 2930 || ac > 3023)  && c != asc_Space )
            break;

        if (c > asc_Space && c < 127)
        {
            rotatesprite(offset<<16,y<<16,MZ,0,ac, shade, pal, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);

            offset += tilesizx[ac];
            
        } else 
        if (c == asc_Space)
            {
            offset += 4;                // Special case for space char
            }
    }

}

////////////////////////////////////////////////
//  Get an input string from user using small font
////////////////////////////////////////////////

signed char 
MNU_InputSmallString(char *name, short pix_width)
    {
    char ch;
    short w, h;
    UserInput con_input;

#define ascii_backspace 8
#define ascii_esc 27
#define ascii_return 13

    if (!MoveSkip4 && !MessageInputMode)
        {
        con_input.dir = dir_None;
        CONTROL_GetUserInput(&con_input);

        if (con_input.dir == dir_North)
            {
            CON_CommandHistory(1);
            }
        else 
        if (con_input.dir == dir_South)
            {
            CON_CommandHistory(-1);
            }
        }

    while (KB_KeyWaiting())
        {
        ch = KB_Getch();

        // skip any extended key
        if (ch == 0)
            {
            ch = KB_Getch();
            if (ch == 104) // extended enter
                ch = ascii_return;
            else    
                continue;
            }

        if (ch == ascii_backspace)
            {
            name[strlen(name) - 1] = '\0';
            continue;
            }
        else 
        if (ch == ascii_esc)
            {
            return (-1);
            }
        else 
        if (ch == ascii_return)
            {
            return (FALSE);
            }
        else 
        if (!isprint(ch))
            continue;

        MNU_MeasureSmallString(name, &w, &h);
        if (w < pix_width)
            {
            if(strlen(name) < 256) // Dont let it go too far!
                sprintf(name, "%s%c", name, ch);
            }
        }

    return (TRUE);

    }

////////////////////////////////////////////////
// Draw dialog text on screen
////////////////////////////////////////////////
static BOOL
MNU_Dialog(void)
    {
    short ndx, linecnt, w[MAXDIALOG], h, x, y;

    linecnt = 0;
    h = 8;

    for (ndx = 0; ndx < MAXDIALOG && dialog[ndx]; ndx++)
        {
        MNU_MeasureString(dialog[ndx], &w[ndx], &h);
        ASSERT(w[ndx] < XDIM);
        linecnt++;
        }

    y = ((YDIM - ((h * linecnt) + (linecnt * 2))) / 2);

    for (ndx = 0; ndx < linecnt; ndx++)
        {
        x = ((XDIM - w[ndx]) / 2);
        MNU_DrawString(x, y, dialog[ndx],1,16);
        y += (h + 3);
        }

    mnu_input.button0 = mnu_input.button1 = FALSE;
    CONTROL_ClearUserInput(&mnu_input);
    CONTROL_GetUserInput(&mnu_input);

    if (KB_KeyPressed(sc_Y))
        return (TRUE);
    else
        return (FALSE);
    }

////////////////////////////////////////////////
//  Get an input string from user
////////////////////////////////////////////////

signed char 
MNU_InputString(char *name, short pix_width)
    {
    char ch;
    short w, h;

#define ascii_backspace 8
#define ascii_esc 27
#define ascii_return 13

    while (KB_KeyWaiting())
        {
        ch = KB_Getch();

        ////DSPRINTF(ds, "%c %d", ch, ch);
        //MONO_PRINT(ds);

        // skip most extended keys
        if (ch == 0)
            {
            ch = KB_Getch();
            
            ////DSPRINTF(ds, "extended key %c %d", ch, ch);
            //MONO_PRINT(ds);
             
            if (ch == 104) // extended enter
                ch = ascii_return;
            else    
                continue;
            }

        if (ch == ascii_backspace)
            {
            name[strlen(name) - 1] = '\0';
            continue;
            }
        else 
        if (ch == ascii_esc)
            {
            return (-1);
            }
        else 
        if (ch == ascii_return)
            {
            return (FALSE);
            }
        else 
        if (!isprint(ch))
            continue;

        MNU_MeasureString(name, &w, &h);
        if (w < pix_width)
            {
            sprintf(name, "%s%c", name, ch);
            }
        }

    return (TRUE);

    }

#define SS_XSTART 146L
#define SS_YSTART SD_YSTART
#define SS_BORDER_SIZE 5L

VOID LoadSaveMsg(char *msg)
    {
    short w,h;
    
    flushperms();
    DrawMenuLevelScreen();
    strcpy((char*)ds, (char*)msg);
    MNU_MeasureString(ds, &w, &h);
    MNU_DrawString(TEXT_XCENTER(w), 170, ds, 1, 16);
    nextpage();
    }
        

////////////////////////////////////////////////
//  Load Game menu
//  This function gets called whenever you
//  press enter on one of the load game
//  spots.
//  I'm figuring it need to do the following:
//  . Load the game if there is one by calling: MNU_LoadGameCustom.
////////////////////////////////////////////////
static BOOL
MNU_GetLoadCustom(void)
    {
    short load_num;

    load_num = currentmenu->cursor;

    // no saved game exists - don't do anything
    if (SaveGameDescr[load_num][0] == '\0')
        return (FALSE);

    if (InMenuLevel || DemoMode || DemoPlaying)
        {
        LoadSaveMsg("Loading...");

        if (LoadGame(load_num) == -1)
            return (FALSE);

        QuickLoadNum = load_num;

        ExitMenus();
        ExitLevel = TRUE;
        LoadGameOutsideMoveLoop = TRUE;
        if (DemoMode || DemoPlaying)
            LoadGameFromDemo = TRUE;
    
        return(TRUE);
        }
        
    LoadSaveMsg("Loading...");

    PauseAction();

    if (LoadGame(load_num) == -1)
        {
        ResumeAction();
        return (FALSE);
        }

    QuickLoadNum = load_num;
        
    ready2send = 1;
    LastSaveNum = -1;
    ExitMenus();

    if (DemoMode)
        {
        ExitLevel = TRUE;
        DemoPlaying = FALSE;
        }

    return (TRUE);
    }

////////////////////////////////////////////////
//  Save Game menu
//  This function gets called whenever you
//  press enter on one of the load game
//  spots.
//  I'm figuring it need to do the following:
//  . Call MNU_GetInput to allow string input of description.
//  . Save the game if there is one by calling: MNU_SaveGameCustom.
////////////////////////////////////////////////
static BOOL
MNU_GetSaveCustom(void)
    {
    short save_num;
    extern BOOL InMenuLevel, LoadGameOutsideMoveLoop;

    save_num = currentmenu->cursor;
    
    if (InMenuLevel)
        return(FALSE);

    if (MenuInputMode)
        {
        PauseAction();
        
        LoadSaveMsg("Saving...");
        
        if (SaveGame(save_num) != -1)
            {
            QuickLoadNum = save_num;
            
            LoadGameGroup.cursor = save_num;
            LastSaveNum = -1;
            ResumeAction();
            ExitMenus();
            }

        // toggle edit mode
        MenuInputMode = FALSE;
        }
    else
        {
        strcpy(BackupSaveGameDescr, SaveGameDescr[save_num]);

        // clear keyboard buffer
        while (KB_KeyWaiting())
            {
            if (KB_Getch() == 0)
                KB_Getch();
            }

        // toggle edit mode
        MenuInputMode = TRUE;
        }

    return (TRUE);
    }

////////////////////////////////////////////////
//  Load/Save Touchup function
//  This function gets called each frame by DrawMenus
////////////////////////////////////////////////

static BOOL
MNU_DrawLoadSave(short game_num)
    {
    // screen border
    rotatesprite(SS_XSTART << 16, SS_YSTART << 16, MZ, 0, pic_loadsavescreen,
        0, 0, MenuDrawFlags | ROTATE_SPRITE_CORNER, 0, 0, xdim - 1, ydim - 1);

    // description box
    rotatesprite((SD_XSTART) << 16, (SD_YSTART) << 16, MZ, 0, pic_savedescr,
        0, 0, MenuDrawFlags | ROTATE_SPRITE_CORNER, 0, 0, xdim - 1, ydim - 1);

    // cursor for text boxes
    rotatesprite((SD_XSTART + 3) << 16, (SD_LINE(game_num) + 1) << 16, MZ, 0, pic_loadsavecursor,
        0, 0, MenuDrawFlags | ROTATE_SPRITE_CORNER, 0, 0, xdim - 1, ydim - 1);

    return(TRUE);
    }

static char SaveGameInfo1[80];
static char SaveGameInfo2[80];

BOOL
MNU_LoadSaveMove(UserCall call, MenuItem_p item)
    {
    short i;
    short game_num;
    short tile;
    static short SaveGameEpisode, SaveGameLevel, SaveGameSkill;
    BOOL GotInput = FALSE;
    
    if (!UsingMenus)
        return(TRUE);

    game_num = currentmenu->cursor;

    // read all descr first time through - LastSaveNum starts at 99
    if (LastSaveNum == 99)
        {
        memset(SaveGameDescr, '\0', sizeof(SaveGameDescr));

        for (i = 0; i < 10; i++)
            LoadGameDescr(i, SaveGameDescr[i]);
        }

    // cursor has moved - read header
    if (game_num != LastSaveNum)
        {
        screen_tile = LoadGameFullHeader(game_num, SaveGameDescr[game_num],
            &SaveGameLevel, &SaveGameSkill);

        sprintf(SaveGameInfo1, "Level %d, Skill %d", SaveGameLevel, SaveGameSkill+1);
        SaveGameInfo2[0] = NULL;
        }

    if (QuickSaveMode)
        {
        QuickSaveMode = FALSE;
        MenuInputMode = TRUE;
        strcpy(BackupSaveGameDescr, SaveGameDescr[game_num]);
        KB_ClearKeysDown();
        KB_FlushKeyboardQueue();
        }

    LastSaveNum = game_num;

    // input mode check
    if (MenuInputMode)
        {
        MenuItem *item = &currentmenu->items[currentmenu->cursor];
        
        if (SavePrompt)
            {
            if (KB_KeyPressed(sc_Y))
                {
                SavePrompt = FALSE;
                // use input
                item->custom();
                }
            else    
            if (KB_KeyPressed(sc_N))
                {
                strcpy(SaveGameDescr[game_num], BackupSaveGameDescr);
                SavePrompt = FALSE;
                MenuInputMode = FALSE;
                }
            }
        else    
        // get input
        switch (MNU_InputString(SaveGameDescr[game_num], 114))
            {
        case -1:                        // Cancel Input (pressed ESC) or Err
            strcpy(SaveGameDescr[game_num], BackupSaveGameDescr);
            MenuInputMode = FALSE;
            KB_ClearKeysDown();
            break;
        case FALSE:                     // Input finished (RETURN)
            // no input
            if (SaveGameDescr[game_num][0] == '\0')
                {
                strcpy(SaveGameDescr[game_num], BackupSaveGameDescr);
                MenuInputMode = FALSE;
                }
            else
                {
                GotInput = TRUE;
                }
            break;
        case TRUE:                      // Got input
            break;
            }
        
        if (GotInput)    
            {
            if (BackupSaveGameDescr[0])
                SavePrompt = TRUE;
                
            if (!SavePrompt)    
                {
                // use input
                item->custom();
                }
            }
        }

    return (TRUE);
    }
    
BOOL
MNU_LoadSaveDraw(UserCall call, MenuItem_p item)
    {
    short i;
    short game_num;
    short tile;

    game_num = currentmenu->cursor;

    // misc drawing
    MNU_DrawLoadSave(game_num);

    // print game descriptions
    for (i = 0; i < 10; i++)
        {
        if (i == game_num && MenuInputMode && !SavePrompt)
            {
            static BOOL cur_show;
            char tmp[sizeof(SaveGameDescr[0])];

            cur_show ^= 1;
            if (cur_show)
                {
                // add a cursor to the end
                sprintf(tmp, "%s%s", SaveGameDescr[i], "_");
                }
            else
                strcpy(tmp, SaveGameDescr[i]);

            MNU_DrawString(SD_XSTART + 4, SD_YSTART + (i * SD_YOFF) + 2, tmp, 1, 16);
            }
        else 
        if (SaveGameDescr[i][0] != '\0')
            {
            MNU_DrawString(SD_XSTART + 4, SD_YSTART + (i * SD_YOFF) + 2, SaveGameDescr[i], 1, 16);
            }
        }

    if (screen_tile != -1)
        {
        // draw 160x100 save screen
        rotatesprite((SS_XSTART + SS_BORDER_SIZE) << 16, (SS_YSTART + SS_BORDER_SIZE) << 16, (1 << 16), 0 + 512, screen_tile,
            0, 0, MenuDrawFlags | ROTATE_SPRITE_CORNER | ROTATE_SPRITE_NON_MASK | ROTATE_SPRITE_YFLIP, 0, 0, xdim - 1, ydim - 1);

        // draw info string
        MNU_DrawString(SS_XSTART + 13, SS_YSTART + 100 + 10, SaveGameInfo1, 1, 16);
        MNU_DrawString(SS_XSTART + 13, SS_YSTART + 100 + 18, SaveGameInfo2, 1, 16);
        
        if (SavePrompt)    
            {
            MNU_DrawString(SS_XSTART + SS_BORDER_SIZE + 5, SS_YSTART + SS_BORDER_SIZE + 47, "Overwrite previous", 1, 16);
            MNU_DrawString(SS_XSTART + SS_BORDER_SIZE + 5, SS_YSTART + SS_BORDER_SIZE + 47 + 12, "  saved game (Y/N)", 1, 16);
            }
        }
    else
        {
        // draw 160x100 black pic
        rotatesprite((SS_XSTART + SS_BORDER_SIZE) << 16, (SS_YSTART + SS_BORDER_SIZE) << 16, (1 << 16), 0, pic_loadsavescreenbak,
            0, 0, MenuDrawFlags | ROTATE_SPRITE_CORNER | ROTATE_SPRITE_NON_MASK, 0, 0, xdim - 1, ydim - 1);
            
        MNU_DrawString(SS_XSTART + SS_BORDER_SIZE + 60, SS_YSTART + SS_BORDER_SIZE + 47, "Empty", 1, 16);
        }
    

    return (TRUE);
    }
    
BOOL
MNU_ShareWareCheck(MenuItem *item)
    {
    #ifdef SW_SHAREWARE
        SET(item->flags, mf_disabled);
    #endif

    return (TRUE);
    }

BOOL
MNU_CheckUserMap(MenuItem *item)
    {
    if (UserMapName[0] == '\0')
        RESET(item->flags, mf_disabled);
    else    
        SET(item->flags, mf_disabled);
    return (TRUE);
    }
    
BOOL
MNU_ShareWareMessage(MenuItem *item)
    {
    char *extra_text;
    short w,h;

    #ifdef SW_SHAREWARE
        extra_text = "Be sure to call 800-3DREALMS today";
        MNU_MeasureString(extra_text, &w, &h);
        MNU_DrawString(TEXT_XCENTER(w), 110, extra_text, 1, 16);
        extra_text = "and order the game.";
        MNU_MeasureString(extra_text, &w, &h);
        MNU_DrawString(TEXT_XCENTER(w), 120, extra_text, 1, 16);
        extra_text = "You are only playing the first ";
        MNU_MeasureString(extra_text, &w, &h);
        MNU_DrawString(TEXT_XCENTER(w), 130, extra_text, 1, 16);
        extra_text = "four levels, and are missing most";
        MNU_MeasureString(extra_text, &w, &h);
        MNU_DrawString(TEXT_XCENTER(w), 140, extra_text, 1, 16);
        extra_text = "of the game, weapons and monsters.";
        MNU_MeasureString(extra_text, &w, &h);
        MNU_DrawString(TEXT_XCENTER(w), 150, extra_text, 1, 16);
        extra_text = "See the ordering information.";
        MNU_MeasureString(extra_text, &w, &h);
        MNU_DrawString(TEXT_XCENTER(w), 160, extra_text, 1, 16);
        SET(item->flags, mf_disabled);
    #endif
    return (TRUE);
    }
    
BOOL
MNU_SaveGameCheck(MenuItem *item)
    {
    extern BOOL InMenuLevel;
    extern BOOL DemoMode;
    
    if (CommEnabled || numplayers > 1 || DemoMode)
        {
        SET(item->flags, mf_disabled);
        return(TRUE);
        }
    
    if (InMenuLevel)
        SET(item->flags, mf_disabled);
    else
        {
        if (TEST(Player[myconnectindex].Flags, PF_DEAD))    
            SET(item->flags, mf_disabled);
        else    
            RESET(item->flags, mf_disabled);
        }    
    
    
    return (TRUE);
    }

BOOL
MNU_TenCheck(MenuItem *item)
    {
    if (CommEnabled || numplayers > 1)
        {
        SET(item->flags, mf_disabled);
        return(TRUE);
        }
    
    return (TRUE);
    }
    
BOOL
MNU_LoadGameCheck(MenuItem *item)
    {

    if (CommEnabled || numplayers > 1)
        {
        SET(item->flags, mf_disabled);
        return(TRUE);
        }
    
    return (TRUE);
    }

BOOL
MNU_StatCheck(MenuItem *item)
    {
    if (CommEnabled || numplayers > 1)
        {
        SET(item->flags, mf_disabled);
        return(TRUE);
        }
    
    return (TRUE);
    }

BOOL
MNU_HurtTeammateCheck(MenuItem *item)
    {
    switch (gs.NetGameType+1)
        {
        // deathmatch and deathmatch no respawn
        case MULTI_GAME_COMMBAT:
        case MULTI_GAME_COMMBAT_NO_RESPAWN:
            if (gs.NetTeamPlay)
                RESET(item->flags, mf_disabled);
            else
                SET(item->flags, mf_disabled);
            break;
        // co-op    
        case MULTI_GAME_COOPERATIVE:
            RESET(item->flags, mf_disabled);
            break;        
        }    
            
    return (TRUE);
    }

BOOL
MNU_TeamPlayCheck(MenuItem *item)
    {
    switch (gs.NetGameType+1)
        {
        // co-op    
        case MULTI_GAME_COOPERATIVE:
            SET(item->flags, mf_disabled);
            break;        
        default:
            RESET(item->flags, mf_disabled);
            break;
        }    
            
    return (TRUE);
    }

BOOL
MNU_CoopPlayCheck(MenuItem *item)
    {
    switch (gs.NetGameType+1)
        {
        // co-op    
        case MULTI_GAME_COOPERATIVE:
            SET(item->flags, mf_disabled);
            break;        
        default:
            RESET(item->flags, mf_disabled);
            break;
        }    
            
    return (TRUE);
    }
    
BOOL
MNU_TeamPlayChange(void)
    {
    // if team play changes then do a pre process again
    MNU_ItemPreProcess(currentmenu);
    return (TRUE);
    }
    
BOOL
MNU_MouseCheck(MenuItem *item)
    {
    if (ControllerType != controltype_keyboardandmouse)
        {
        SET(item->flags, mf_disabled);
        }
    else
        {
        RESET(item->flags, mf_disabled);
        }    
        
    return (TRUE);
    }

// This is only called when Enter is pressed    
static BOOL
MNU_TryMusicInit(void)
    {
    if (!cdvalid)
        {
        enabled = TRUE; // Let it in to try and reset
        CDAudio_Init();
        if (cdvalid)
            {
            if (currentmenu->cursor == 0)
                MNU_MusicCheck(&currentmenu->items[currentmenu->cursor+1]);
            CDAudio_Play(RedBookSong[Level], TRUE);
            }
        }

    return (TRUE);
    }

BOOL    
MNU_MusicCheck(MenuItem *item)
    {
    #ifdef SW_SHAREWARE
    if (MusicDevice == NumSoundCards || !MusicInitialized)
        {
        SET(item->flags, mf_disabled);
        }
    else
        {
        RESET(item->flags, mf_disabled);
        }    
    #else
    // Redbook audio stuff
    if (!cdvalid)
        {
        //enabled = TRUE; // Let it in to try and reset
        //CDAudio_Init();
        //if (cdvalid)
        //    {
        //    if (currentmenu->cursor == 0)
        //        MNU_MusicCheck(&currentmenu->items[currentmenu->cursor+1]);
        //    CDAudio_Play(RedBookSong[Level], TRUE);
        //    RESET(item->flags, mf_disabled);
        //    }
        //else    
        SET(item->flags, mf_disabled); // Just don't let CD Redbook ever be invalid!
        }
    else
        {
        RESET(item->flags, mf_disabled);
        }    
    #endif    
        
    return (TRUE);
    }
    
BOOL
MNU_FxCheck(MenuItem *item)
    {
    if (FXDevice == NumSoundCards || !FxInitialized)
        {
        SET(item->flags, mf_disabled);
        }
    else
        {
        RESET(item->flags, mf_disabled);
        }    
        
    return (TRUE);
    }

BOOL
MNU_MusicFxCheck(MenuItem *item)
    {
    if (FXDevice == NumSoundCards && MusicDevice == NumSoundCards)
        {
        SET(item->flags, mf_disabled);
        }
    else
        {
        RESET(item->flags, mf_disabled);
        }    
        
    return (TRUE);
    }
    
////////////////////////////////////////////////
// Do a toggle button
////////////////////////////////////////////////
void 
MNU_DoButton(MenuItem_p item, BOOL draw)
    {
    long x, y;
    BOOL state;
    long last_value;
    short shade = MENU_SHADE_DEFAULT;
    extern char LevelSong[];
    char *extra_text = NULL;
    PLAYERp pp = &Player[myconnectindex];
    long button_x,zero=0;
    int handle=0;
    extern BOOL MusicInitialized,FxInitialized;

    button_x = OPT_XSIDE;
    
    x = item->x;
    y = item->y;

    if (TEST(item->flags, mf_disabled))
        {
        shade = MENU_SHADE_INACTIVE;
        }

    if (!draw)    
        {
        switch (item->button)
            {
        case btn_nuke:
            gs.NetNuke = state = buttonsettings[item->button];
            break;        
        case btn_voxels:
            gs.Voxels = state = buttonsettings[item->button];
            break;        
        case btn_stats:
            gs.Stats = state = buttonsettings[item->button];
            break;        
        case btn_markers:
            gs.NetSpawnMarkers = state = buttonsettings[item->button];
            break;        
        case btn_teamplay:
            gs.NetTeamPlay = state = buttonsettings[item->button];
            break;        
        case btn_friendlyfire:
            gs.NetHurtTeammate = state = buttonsettings[item->button];
            break;        
        case btn_crosshair:
            gs.Crosshair = state = buttonsettings[item->button];
            break;
        case btn_auto_aim:
            last_value = gs.AutoAim;
            gs.AutoAim = state = buttonsettings[item->button];
            if (gs.AutoAim != last_value)
                MenuButtonAutoAim = TRUE;
            break;
        case btn_messages:
            gs.Messages = state = buttonsettings[item->button];
            break;
        case btn_auto_run:
            last_value = gs.AutoRun;
            gs.AutoRun = state = buttonsettings[item->button];
            if (gs.AutoRun != last_value)
                MenuButtonAutoRun = TRUE;
            break;
        case btn_mouse_aim:
            last_value = gs.MouseAimingType;
            gs.MouseAimingType = state = buttonsettings[item->button];
            if (gs.MouseAimingType != last_value)
                {
                //RESET(pp->Flags, PF_MOUSE_AIMING_ON);
                //gs.MouseAimingOn = FALSE;
                }
            //extra_text = gs.MouseAimingType ? "Momentary" : "Toggle";       
            break;
        case btn_mouse_invert:
            gs.MouseInvert = state = buttonsettings[item->button];
            break;
//        case btn_bobbing:
//            gs.Bobbing = state = buttonsettings[item->button];
//            break;
        case btn_sound:

            if (!FxInitialized)
                 break;

            last_value = gs.FxOn;
            gs.FxOn = state = buttonsettings[item->button];
            if (gs.FxOn != last_value)
                {
                if (!gs.FxOn)
                    StopFx();
                }    
            break;
        case btn_music:
            #ifdef SW_SHAREWARE
            if (!MusicInitialized)
                 break;
            #endif 

            last_value = gs.MusicOn;
            gs.MusicOn = state = buttonsettings[item->button];
            if (gs.MusicOn != last_value)
                {
                BOOL bak;
                
                if (gs.MusicOn)
                    {
                    #ifdef SW_SHAREWARE
                    bak = DemoMode;
                    PlaySong(LevelSong);    
                    DemoMode = bak;
                    #else
                    if (!cdvalid)
                        {
                        enabled = TRUE;
                        CDAudio_Init();
                        if (cdvalid)             
                            {
                            MNU_MusicCheck(item);
                            MNU_MusicCheck(&currentmenu->items[currentmenu->cursor+1]);
                            CDAudio_Play(RedBookSong[Level], TRUE);
                            }    
                        } else
                        {
                        CDAudio_Stop();
                        CDAudio_Play(RedBookSong[Level], TRUE);
                        }
                    #endif
                    }
                else
                    {
                    #ifdef SW_SHAREWARE
                    bak = DemoMode;
                    StopSong();
                    DemoMode = bak;
                    #else
                    CDAudio_Stop();
                    #endif

                    #ifdef SW_SHAREWARE
                    handle = PlaySound(DIGI_NOLIKEMUSIC,&zero,&zero,&zero,v3df_none);

                    if (handle > FX_Ok)
                        while(FX_SoundActive(handle))
                            ;
                    #endif
                    }
                }    
            break;
        case btn_talking:
            gs.Talking = state = buttonsettings[item->button];
            break;
        case btn_playcd:
            last_value = gs.PlayCD;
            gs.PlayCD = state = buttonsettings[item->button];
            break;
        case btn_ambience:
            last_value = gs.Ambient;
            gs.Ambient = state = buttonsettings[item->button];
            if (gs.Ambient != last_value)
                {
                if(!InMenuLevel)
                    {
                    if (gs.Ambient)
                        StartAmbientSound();
                    else    
                        StopAmbientSound();
                    }    
                }    
            break;
        case btn_flipstereo:
            last_value = gs.FlipStereo;
            gs.FlipStereo = state = buttonsettings[item->button];
            if (gs.FlipStereo != last_value)
                FlipStereo();
            break;
        case btn_shadows:
            gs.Shadows = state = buttonsettings[item->button];
            break;

        case btn_parental:
            if(gs.Password[0] != '\0' && gs.ParentalLock == TRUE)
            {
                if(passwordvalid)
                {
                    state = buttonsettings[btn_parental] = gs.ParentalLock = FALSE;
                    if (!InMenuLevel)
                    JS_ToggleLockouts();
                } else
                    {
                    state = buttonsettings[btn_parental] = gs.ParentalLock = TRUE;
                    MenuInputMode = TRUE;
                    memset(MessageInputString, '\0', sizeof(MessageInputString));
                    KB_ClearKeysDown();
                    KB_FlushKeyboardQueue();
                    }
            } else
            {
                gs.ParentalLock = state = buttonsettings[item->button];
                if (!InMenuLevel)
                JS_ToggleLockouts();
            }
            break;

        default:
            state = buttonsettings[item->button];
            break;
            }
        }    
    
    if (!draw)    
        return;
    
    switch (item->button)
        {
        case btn_mouse_aim:
            extra_text = gs.MouseAimingType ? "Momentary" : "Toggle";       
            break;
        }
        
    
    state = buttonsettings[item->button];
        
    // Draw the button
    if (item->text)
        {
        if (state)
            {
            // set
            rotatesprite(button_x << 16, y << 16, MZ, 0, pic_radiobuttn2, shade, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
            }
        else
            {
            // not set
            rotatesprite(button_x << 16, y << 16, MZ, 0, pic_radiobuttn1, shade, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
            }
            
        MenuTextShade = shade;    
        MNU_DrawString(x, y, item->text, MenuTextShade, 16);
        
        if (extra_text)
            MNU_DrawString(OPT_XSIDE + tilesizx[pic_radiobuttn1] + 6, y, extra_text, MenuTextShade, 16);
        MenuTextShade = MENU_SHADE_DEFAULT;    
        }
    else
        {
        if (state)
            rotatesprite(x << 16, y << 16, MZ, 0, pic_radiobuttn2, 2, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
        else
            rotatesprite(x << 16, y << 16, MZ, 0, pic_radiobuttn1, 2, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
            
        x += tilesizx[pic_radiobuttn1] + 4;
        
        // Draw the menu item text
        rotatesprite(x << 16, y << 16, MZ, 0, item->pic, 2, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
        }

    }

//char *gametype[] = {"War [Respawn]","Cooperative","War [No Respawn]"};
char *gametype[] = {"WangBang (spawn)","WangBang (no spawn)","Cooperative"};
char *playercolors[] = {"Brown","Gray","Purple","Red","Yellow","Olive","Green","Blue"};
char *monsterskills[] = {"No Monsters","Easy","Normal","Hard","Insane!"};

void 
MNU_DoSlider(short dir, MenuItem_p item, BOOL draw)
    {
    short offset, i, barwidth;
    long x, y, knobx;
    short shade = MENU_SHADE_DEFAULT;
    char *extra_text=NULL;
    char tmp_text[256];

    memset(tmp_text,0,256);

    if (TEST(item->flags, mf_disabled))
        {
        shade = MENU_SHADE_INACTIVE;
        dir = 0;
        }
        
    switch (item->slider)
        {
    case sldr_mouse:
        barwidth = SLDR_MOUSESENSEMAX;
        offset = slidersettings[sldr_mouse] += dir;
        
        if (TEST(item->flags, mf_disabled))
            break;
        
        offset = max(offset, 0);
        offset = min(offset, SLDR_MOUSESENSEMAX-1);
        
        slidersettings[sldr_mouse] = offset;
        
        gs.MouseSpeed = offset * (MOUSE_SENS_MAX_VALUE/SLDR_MOUSESENSEMAX);
        CONTROL_SetMouseSensitivity(gs.MouseSpeed<<2);
        break;
        
    case sldr_sndfxvolume:
        barwidth = SLDR_SNDFXVOLMAX;
        offset = slidersettings[sldr_sndfxvolume] += dir;
        
        if (TEST(item->flags, mf_disabled))
            break;
        
        offset = max(offset, 0);
        offset = min(offset, SLDR_SNDFXVOLMAX-1);
        
        slidersettings[sldr_sndfxvolume] = offset;
        gs.SoundVolume = FX_MIN + (offset * VOL_MUL);
        FX_SetVolume(gs.SoundVolume);
        break;
        
    case sldr_musicvolume:
        barwidth = SLDR_MUSICVOLMAX;
        offset = slidersettings[sldr_musicvolume] += dir;
        if (TEST(item->flags, mf_disabled))
            break;
        
        offset = max(offset, 0);
        offset = min(offset, SLDR_MUSICVOLMAX-1);
        
        slidersettings[sldr_musicvolume] = offset;
        gs.MusicVolume = MUSIC_MIN + (offset * VOL_MUL);
        #ifndef SW_SHAREWARE
        CDAudio_SetVolume(gs.MusicVolume);
        #else
        MUSIC_SetVolume(gs.MusicVolume);
        #endif    
        break;
        
    case sldr_scrsize:
        {
        short bnum;
        
        barwidth = SLDR_SCRSIZEMAX;
        slidersettings[sldr_scrsize] = gs.BorderNum;
        slidersettings[sldr_scrsize] -= dir;
        offset = slidersettings[sldr_scrsize];
        
        if (TEST(item->flags, mf_disabled))
            break;
        
    ////DSPRINTF(ds,"BorderNum %d",gs.BorderNum);
    //MONO_PRINT(ds);
        
        offset = max(offset, 0);
        offset = min(offset, SLDR_SCRSIZEMAX - 1);
        
        bnum = offset;
        
        offset = (SLDR_SCRSIZEMAX-1) - offset;
        slidersettings[sldr_scrsize] = offset;
        
        if (!BorderAdjust)
            gs.BorderNum = bnum;
            
        SetBorder(&Player[myconnectindex], bnum);
        
        break;
        }
        
    case sldr_brightness:
        barwidth = SLDR_BRIGHTNESSMAX;
        offset = slidersettings[sldr_brightness] += dir;
        
        if (TEST(item->flags, mf_disabled))
            break;
            
        offset = max(offset, 0);
        offset = min(offset, SLDR_BRIGHTNESSMAX - 1);
        slidersettings[sldr_brightness] = offset;
        
        if (gs.Brightness != offset)
            {
            gs.Brightness = offset;
            COVERsetbrightness(gs.Brightness,(char *)palette);
            COVERsetbrightness(gs.Brightness,(char *)palette_data);
            }
        break;

    case sldr_bordertile:
        barwidth = SLDR_BORDERTILEMAX;
        offset = slidersettings[sldr_bordertile] += dir;
        
        if (TEST(item->flags, mf_disabled))
            break;
            
        offset = max(offset, 0);
        offset = min(offset, SLDR_BORDERTILEMAX - 1);
        slidersettings[sldr_bordertile] = offset;
        
        if (gs.BorderTile != offset)
            {
            extern short BorderTest[];
            
            gs.BorderTile = offset;

            SetRedrawScreen(&Player[myconnectindex]);
            }
        break;

    case sldr_gametype:
        barwidth = SLDR_GAMETYPEMAX;
        offset = slidersettings[sldr_gametype] += dir;
        
        if (TEST(item->flags, mf_disabled))
            break;
            
        offset = max(offset, 0);
        offset = min(offset, SLDR_GAMETYPEMAX - 1);
        slidersettings[sldr_gametype] = offset;
        
        extra_text = gametype[offset];
        MNU_DrawString(OPT_XSIDE, item->y, extra_text, 1, 16);
        gs.NetGameType = offset;
        // friendly fire menu
        MNU_ItemPreProcess(currentmenu);
        break;

    case sldr_netlevel:
        barwidth = SLDR_NETLEVELMAX;
        offset = slidersettings[sldr_netlevel] += dir;
        
        if (TEST(item->flags, mf_disabled))
            break;
            
        offset = max(offset, 0);
        offset = min(offset, SLDR_NETLEVELMAX - 1);
        slidersettings[sldr_netlevel] = offset;
        
        // Show the currently selected level on next line
        extra_text = MNU_LevelName[offset];
        MNU_DrawString(OPT_XS, item->y+10, extra_text, 1, 16);
        gs.NetLevel = offset;
        break;

    case sldr_monsters:
        barwidth = SLDR_MONSTERSMAX;
        offset = slidersettings[sldr_monsters] += dir;
        
        if (TEST(item->flags, mf_disabled))
            break;
            
        offset = max(offset, 0);
        offset = min(offset, SLDR_MONSTERSMAX - 1);
        slidersettings[sldr_monsters] = offset;
        
        extra_text = monsterskills[offset];
        MNU_DrawString(OPT_XSIDE+54, item->y, extra_text, 1, 16);
        gs.NetMonsters = offset;
        break;

    case sldr_killlimit:
        barwidth = SLDR_KILLLIMITMAX;
        offset = slidersettings[sldr_killlimit] += dir;
        
        if (TEST(item->flags, mf_disabled))
            break;
            
        offset = max(offset, 0);
        offset = min(offset, SLDR_KILLLIMITMAX - 1);
        slidersettings[sldr_killlimit] = offset;

        if(offset == 0)
            {
            strcpy(tmp_text,"Infinite\n");
            }
        else 
            {
            itoa(offset*10,tmp_text,10);        
            }
        MNU_DrawString(OPT_XSIDE+101, item->y, tmp_text, 1, 16);
        gs.NetKillLimit = offset;
        break;

    case sldr_timelimit:
        barwidth = SLDR_TIMELIMITMAX;
        offset = slidersettings[sldr_timelimit] += dir;
        
        if (TEST(item->flags, mf_disabled))
            break;
            
        offset = max(offset, 0);
        offset = min(offset, SLDR_TIMELIMITMAX - 1);
        slidersettings[sldr_timelimit] = offset;

        if(offset == 0)
            {
            strcpy(tmp_text,"Infinite\n");
            }
        else 
            {
            sprintf(tmp_text,"%d Minutes\n",TimeLimitTable[offset]);
            }
            
        MNU_DrawString(OPT_XSIDE+86, item->y, tmp_text, 1, 16);
        gs.NetTimeLimit = offset;
        break;

    case sldr_playercolor:
        barwidth = SLDR_PLAYERCOLORMAX;
        offset = slidersettings[sldr_playercolor] += dir;
        
        if (TEST(item->flags, mf_disabled))
            break;
            
        offset = max(offset, 0);
        offset = min(offset, SLDR_PLAYERCOLORMAX - 1);
        slidersettings[sldr_playercolor] = offset;
        
        extra_text = playercolors[offset];
        MNU_DrawString(OPT_XSIDE+78, item->y, extra_text, 1, PALETTE_PLAYER0+offset);
        gs.NetColor = offset;
        break;

    default:
        return;
        }
    
    if (!draw)    
        return;
    
    // Now draw it
    item++;
    x = item->x;
    y = item->y;

    // Draw the left end cap of the bar
    rotatesprite(x << 16, y << 16, MZ, 0, pic_slidelend, shade, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);

    x += tilesizx[pic_slidelend];
    knobx = x;

    // Draw the in between sections
    for (i = 0; i < barwidth; i++)
        {
        rotatesprite(x << 16, y << 16, MZ, 0, pic_slidebar, shade, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
        x += tilesizx[pic_slidebar];
        }

    // Draw the right end cap
    rotatesprite(x << 16, y << 16, MZ, 0, pic_sliderend, shade, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);

    // Draw the knob
    knobx += tilesizx[pic_sliderknob] * offset;
    rotatesprite(knobx << 16, (y + 2) << 16, MZ, 0, pic_sliderknob, shade, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
    }

////////////////////////////////////////////////
// Start up menu array
////////////////////////////////////////////////
static void
MNU_SetupMenu(void)
    {
    MenuGroup *rootmenu;

    static MenuGroup *rootmenulist[] = 
        {
        &maingroup,
        &SaveGameGroup,
        &LoadGameGroup,
        &soundgroup,
        &optiongroup,
        &quickloadgroup,
        &quitgroup,
        &ordergroup,
        &episodegroup,
        };
    
    rootmenu = rootmenulist[ControlPanelType];
    ASSERT(ControlPanelType < ct_max);

    menuarrayptr = 0;
    menuarray[0] = currentmenu = rootmenu;
    if(ControlPanelType == ct_mainmenu)
    
    mnu_input_buffered.button0 = mnu_input_buffered.button1 = FALSE;    
    mnu_input_buffered.dir = dir_None;    
    order_input_buffered.button0 = order_input_buffered.button1 = FALSE;    
    order_input_buffered.dir = dir_None;    
    ResetKeys();

    // custom cust_callback starts out as null
    cust_callback = NULL;

    // for QuitCustom and QuickLoadCustom
    if (currentmenu->items == NULL)
        {
        if (currentmenu->draw_custom)
            currentmenu->draw_custom(uc_setup, NULL);
        }

    if(ControlPanelType == ct_mainmenu)
        currentmenu->cursor = 0;
            
    // disable any items necessary    
    MNU_ItemPreProcess(currentmenu);    
    }

////////////////////////////////////////////////
// Draw an item
////////////////////////////////////////////////
static void
MNU_ClearFlags(MenuGroup * node)
    {
    MenuItem *i;

    if (!node->items)
        return;

    for (i = node->items; i->type != mt_none; i++)
        {
        i->flags &= ~MenuSelectFlags;
        if (i->child)
            MNU_ClearFlags((MenuGroup *) i->child);
        }
    }

////////////////////////////////////////////////
// Pop a group off the menu stack
////////////////////////////////////////////////
static void
MNU_PopGroup(void)
    {
    if (!menuarrayptr)
        return;

    currentmenu = menuarray[--menuarrayptr];
    
    SetFragBar(Player + myconnectindex);    
    //PanelRefresh(Player + myconnectindex);    
    }

////////////////////////////////////////////////
// Push a group on to the menu stack
////////////////////////////////////////////////
static void
MNU_PushGroup(MenuGroup * node)
    {
    if (menuarrayptr == MaxLayers - 1)
        return;

    currentmenu = menuarray[++menuarrayptr] = node;
    
    SetFragBar(Player + myconnectindex);    
    }

////////////////////////////////////////////////
// Setup a new menu subgroup
////////////////////////////////////////////////
static void
MNU_SetupGroup(void)
    {
    MNU_SelectItem(currentmenu, currentmenu->cursor, FALSE);
    MNU_DrawMenu();
    }

VOID    
MNU_ItemPreProcess(MenuGroup * group)
    {
    MenuItem *item;
    
    if (!group->items)
        return;
    
    // process all items when going down a level
    // to see if anything is disabled    
    for (item = group->items; item->type != mt_none; item++)
        {
        if (item->preprocess)
            item->preprocess(item);
        }
    }

VOID    
MNU_ItemPostProcess(MenuGroup * group)
    {
    MenuItem *item;
    long zero = 0;

    if (!group->items)
        return;
    
    item = &currentmenu->items[currentmenu->cursor];

    if (item->postprocess)
        {
        item->postprocess(item);
        }
    }
    
////////////////////////////////////////////////
// Go to next menu subgroup
////////////////////////////////////////////////
static void
MNU_DownLevel(MenuGroup * group)
    {
    
    if (!group)
        {
        TerminateGame();
        printf("MNU_DownLevel() - NULL card\n");
        exit(0);
        }

    MNU_PushGroup(group);

    if (group->items == NULL)
        {
        if (group->draw_custom && group->draw_custom(uc_setup, NULL))
            MNU_PopGroup();
        }

    MNU_ItemPreProcess(currentmenu);    
        
    MNU_SetupGroup();
    
    SetRedrawScreen(&Player[myconnectindex]);
    }

////////////////////////////////////////////////
// Go to previous menu subgroup
////////////////////////////////////////////////
static void
MNU_UpLevel(void)
    {
    long zero = 0;
    static int handle1=0;
    // if run out of menus then EXIT
    if (!menuarrayptr)
        {
        if(!FX_SoundActive(handle1))
            handle1 = PlaySound(DIGI_STARCLINK,&zero,&zero,&zero,v3df_dontpan);
        ExitMenus();
        return;
        }

    if (currentmenu->items)
        currentmenu->items[currentmenu->cursor].flags &= ~mf_selected;
    MNU_PopGroup();
    MNU_SetupGroup();
    
    SetRedrawScreen(&Player[myconnectindex]);
    }

////////////////////////////////////////////////
// Do a menu item action
////////////////////////////////////////////////
static void
MNU_DoItem(void)
    {
    MenuItem *item;

    item = &currentmenu->items[currentmenu->cursor];
    
    if (TEST(item->flags, mf_disabled))
        {
        // Try to process again
        if (item->preprocess)
            item->preprocess(item);
    
        // Check once more
        if (TEST(item->flags, mf_disabled))
            return;
        }
        
    switch (item->type)
        {
        case mt_option:
            if (item->custom != NULL)
                item->custom();
            break;
        case mt_button:
            MNU_PushItem(item, FALSE);
            if (item->custom != NULL)
                item->custom();
            break;
        case mt_layer:
            if (item->custom != NULL)
                item->custom();
            MNU_DownLevel(item->child);
            break;
        }
    }

////////////////////////////////////////////////
// Draw an item icon or cursor
////////////////////////////////////////////////
static void
MNU_DrawItemIcon(MenuItem * item)
    {
    //void BorderRefreshClip(PLAYERp pp, short x, short y, short x2, short y2);
    long x = item->x, y = item->y;
    long scale = MZ;
    short w,h;
    
    if (item->text)
        {
        scale /= 2;
        x -= mulscale17(tilesizx[pic_yinyang],scale) + 2;
        y += 4;
        }
    else
        {
        scale -= (1<<13);
        x -= ((tilesizx[pic_yinyang]) / 2) - 3;
        y += 8;
        }    
        
    rotatesprite(x << 16, y << 16,
        scale, 0, pic_yinyang, item->shade, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
        
    SetRedrawScreen(&Player[myconnectindex]);
    //BorderRefreshClip(&Player[myconnectindex], x - 24, y - 24, x + 24, y + 24);
    }

////////////////////////////////////////////////
// Draw an item
////////////////////////////////////////////////
static void
MNU_DrawItem(MenuItem * item)
    {
    char *ptr;
    short px, py;

    MNU_ItemPostProcess(currentmenu);  // Put this in so things can be drawn on item select

    if (item->pic == NULL)
        return;
    
    MNU_DrawItemIcon(item);

    // if text string skip this part
    if (item->text)
        return;
        
    if (TEST(item->flags, mf_selected) && !TEST(item->flags, mf_disabled))
        {
        // Highlighted
        if (item->type != mt_button)
            rotatesprite(item->x << 16, item->y << 16, MZ, 0, item->pic,
                -30 + STD_RANDOM_RANGE(50), PALETTE_MENU_HIGHLIGHT, MenuDrawFlags,
                 0, 0, xdim - 1, ydim - 1);
        else
            rotatesprite((item->x + tilesizx[pic_radiobuttn1] + 4) << 16, item->y << 16,
                MZ, 0, item->pic, item->shade, PALETTE_MENU_HIGHLIGHT, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
        }
    else
        {
        // Un highlighted
        if (item->type != mt_button)
            rotatesprite(item->x << 16, item->y << 16, MZ, 0, item->pic,
                item->shade, 0, MenuDrawFlags, 0, 319, 199, 0);
        else
            rotatesprite((item->x + tilesizx[pic_radiobuttn1] + 4) << 16, item->y << 16,
                MZ, 0, item->pic, item->shade, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
        }
    }

////////////////////////////////////////////////
// Draw the menu contents
////////////////////////////////////////////////
static void
MNU_DrawMenuContents(void)
    {
    MenuItem *item;
    short w,h;

    ASSERT(currentmenu != NULL);

    if (currentmenu->text)
        {
        // Draw the backdrop bar
        rotatesprite(10 << 16, (currentmenu->y-3) << 16, MZ, 0, 2427,
            currentmenu->shade, 0, MenuDrawFlags|ROTATE_SPRITE_CORNER, 0, 0, xdim - 1, ydim - 1);
        MNU_MeasureStringLarge(currentmenu->text, &w, &h);
        MNU_DrawString(TEXT_XCENTER(w), currentmenu->y, currentmenu->text, 1, 16);
        }
    else    
    if (currentmenu->titlepic)
        {
        rotatesprite(currentmenu->x << 16, currentmenu->y << 16, MZ, 0, currentmenu->titlepic,
            currentmenu->shade, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
        }

    if (!currentmenu->items)
        return;

    for (item = currentmenu->items; item->type != mt_none; item++)
        {
        if (item->pic)
            {
            if (item->type == mt_button)
                {
                // all drawing done here also
                MNU_DoButton(item, TRUE);
                }
            else    
                {
                if (item->text)
                    {
                    if (TEST(item->flags, mf_disabled))
                        MenuTextShade = MENU_SHADE_INACTIVE;
                    MNU_DrawString(item->x, item->y, item->text, MenuTextShade, 16);
                    MenuTextShade = MENU_SHADE_DEFAULT;
                    }
                else
                    {
                    rotatesprite(item->x << 16, item->y << 16, MZ, 0, item->pic,
                        item->shade, 0, MenuDrawFlags, 0, 0, xdim - 1, ydim - 1);
                    }        
                }        
            }

        // Is there a slider attached to this item?  Draw it.
        if (item->type == mt_slider)
            MNU_DoSlider(0, item, TRUE);
        }

    MNU_SelectItem(currentmenu, currentmenu->cursor, TRUE);

    if (currentmenu->draw_custom)
        currentmenu->draw_custom(uc_touchup, NULL);
    }

////////////////////////////////////////////////
// Draw the menu
////////////////////////////////////////////////
void
MNU_DrawMenu(void)
    {
    if (cust_callback != NULL)
        {
        cust_callback(cust_callback_call, cust_callback_item);
        return;
        }

    if (currentmenu->items || currentmenu->titlepic)
        {
        MNU_DrawMenuContents();
        }
    }

////////////////////////////////////////////////
// Select a menu item
////////////////////////////////////////////////
static void
MNU_SelectItem(MenuGroup * group, short index, BOOL draw)
    {
    MenuItem *item;

    if (index != group->cursor)
        {
        item = &group->items[group->cursor];
        item->flags &= ~mf_selected;
        if (draw)
            MNU_DrawItem(item);
        }

    group->cursor = index;
    item = &group->items[group->cursor];
    item->flags |= mf_selected;
    if (draw)
        MNU_DrawItem(item);
    }

////////////////////////////////////////////////
// Toggle a menu radio button on/off
////////////////////////////////////////////////
static void
MNU_PushItem(MenuItem * item, BOOL draw)
    {
    if (item->type != mt_button)
        return;

    buttonsettings[item->button] ^= 1;
    
//    if (draw)
        MNU_DoButton(item, draw);
    }

////////////////////////////////////////////////
// Go to next item on menu
////////////////////////////////////////////////
static void
MNU_NextItem(void)
    {
    MenuTag type;
    MenuFlags flag;

    type = currentmenu->items[currentmenu->cursor + 1].type;
    flag = currentmenu->items[currentmenu->cursor + 1].flags;

    if (type == mt_none)
        MNU_SelectItem(currentmenu, 0, FALSE);
    else
        MNU_SelectItem(currentmenu, currentmenu->cursor + 1, FALSE);

    type = currentmenu->items[currentmenu->cursor].type;
    flag = currentmenu->items[currentmenu->cursor].flags;

    if (type == mt_inert || flag == mf_disabled)
        MNU_NextItem();
    }

////////////////////////////////////////////////
// Go to previous item on menu
////////////////////////////////////////////////
static void
MNU_PrevItem(void)
    {
    MenuTag type;
    MenuFlags flag;

    if (!currentmenu->cursor)
        while (currentmenu->items[++currentmenu->cursor].type != mt_none);

    MNU_SelectItem(currentmenu, currentmenu->cursor - 1, FALSE);

    type = currentmenu->items[currentmenu->cursor].type;
    flag = currentmenu->items[currentmenu->cursor].flags;
    if (type == mt_inert || flag == mf_disabled)
        MNU_PrevItem();
    }

////////////////////////////////////////////////
// Find hotkey press on current menu, if any.
////////////////////////////////////////////////
static BOOL
MNU_DoHotkey(void)
    {
    MenuItem_p item;
    short index;

    index = 0;
    for (item = currentmenu->items; item->type != mt_none; item++)
        {
        if (KEY_PRESSED(item->hotkey) && item->hotkey != NULL)
            {
            MNU_SelectItem(currentmenu, index, FALSE);
            return (TRUE);
            }
        index++;
        }

    return (FALSE);
    }

////////////////////////////////////////////////
// Setup Menus
////////////////////////////////////////////////
void 
SetupMenu(void)
    {
    if (!UsingMenus && !ConPanel)       // Doing this check for multiplay
                                        // menus
        {
        MNU_SetupMenu();

        // Clear the previous ESC key press
        KEY_PRESSED(KEYSC_ESC) = FALSE;
        UsingMenus = TRUE;
        }
    }

////////////////////////////////////////////////
// Setup the main menu
// This function will not loop if in modem
// or network game, otherwise it stops the
// game play until user finished in menus.
////////////////////////////////////////////////
#define MNU_SENSITIVITY 10              // The menu's mouse sensitivity, should be real low

void MNU_DoMenu( CTLType type, PLAYERp pp )
    {
    BOOL resetitem;
    UCHAR key;
    long zero = 0;
    static int handle2 = 0;
    static long limitmove=0;
    static BOOL select_held=FALSE;

    resetitem = TRUE;
    
    if (cust_callback != NULL)
        {
        cust_callback(cust_callback_call, cust_callback_item);
        return;
        }

    //ControlPanelType = type;
    SetupMenu();

    // Zero out the input structure
    mnu_input.button0 = mnu_input.button1 = FALSE;
    mnu_input.dir = dir_None;

    // should not get input if you are editing a save game slot
    if(totalclock < limitmove) limitmove = totalclock;
    if (!MenuInputMode)
        {
        UserInput tst_input;
        BOOL select_held = FALSE;
        
        
        // Zero out the input structure
        tst_input.button0 = tst_input.button1 = FALSE;
        tst_input.dir = dir_None;
        
        if(!select_held)
            {
            CONTROL_GetUserInput(&tst_input);
            mnu_input_buffered.dir = tst_input.dir;
            }
    
        if(mnu_input_buffered.button0 || mnu_input_buffered.button1)    
            {
            if(tst_input.button0 == mnu_input_buffered.button0 && 
               tst_input.button1 == mnu_input_buffered.button1) 
                {
                select_held = TRUE;
                } 
                else
                if(totalclock - limitmove > 7)
                {
                mnu_input.button0 = mnu_input_buffered.button0;
                mnu_input.button1 = mnu_input_buffered.button1;

                mnu_input_buffered.button0 = tst_input.button0;
                mnu_input_buffered.button1 = tst_input.button1;
                }
            } else
            {
            select_held = FALSE;    
            mnu_input_buffered.button0 = tst_input.button0;
            mnu_input_buffered.button1 = tst_input.button1;
            }

        if(totalclock - limitmove > 7 && !select_held)
            {
            mnu_input.dir = mnu_input_buffered.dir;

            if (mnu_input.dir != dir_None)
                if(!FX_SoundActive(handle2))
                    handle2 = PlaySound(DIGI_STAR,&zero,&zero,&zero,v3df_dontpan);

            limitmove = totalclock;
            mnu_input_buffered.dir = dir_None;
            }
        }

    if (mnu_input.dir == dir_North)
        {
        MNU_PrevItem();
        resetitem = TRUE;
        }
    else 
    if (mnu_input.dir == dir_South)
        {
        MNU_NextItem();
        resetitem = TRUE;
        }
    else 
    if (mnu_input.button0)
        {
        static int handle5=0;
        if(!FX_SoundActive(handle5))
            handle5 = PlaySound(DIGI_SWORDSWOOSH,&zero,&zero,&zero,v3df_dontpan);
        KB_ClearKeysDown();
        MNU_DoItem();
        resetitem = TRUE;
        }
    else 
    if (mnu_input.dir == dir_West
        && currentmenu->items[currentmenu->cursor].type == mt_slider)
        {
        MNU_DoSlider(-1, &currentmenu->items[currentmenu->cursor], FALSE);
        resetitem = TRUE;
        }
    else 
    if (mnu_input.dir == dir_East
        && currentmenu->items[currentmenu->cursor].type == mt_slider)
        {
        MNU_DoSlider(1, &currentmenu->items[currentmenu->cursor], FALSE);
        resetitem = TRUE;
        }
    else 
    if (mnu_input.button1)
        {
        static int handle3=0;
        if(!FX_SoundActive(handle3))
            handle3 = PlaySound(DIGI_SWORDSWOOSH,&zero,&zero,&zero,v3df_dontpan);
        MNU_UpLevel();
        resetitem = TRUE;
        }
    else 
    if (MNU_DoHotkey())
        {
        static int handle4=0;
        if(!FX_SoundActive(handle4))
            handle4 = PlaySound(DIGI_STAR,&zero,&zero,&zero,v3df_dontpan);
        resetitem = TRUE;
        mnu_input_buffered.button0 = mnu_input_buffered.button1 = FALSE;
        }
    else
        resetitem = FALSE;
    
    // !FRANK! I added this because the old custom was only called for drawing
    // Needed one for drawing and moving.
    if (currentmenu->move_custom)
        currentmenu->move_custom(uc_setup, NULL);

    if (resetitem)
        {
        KB_ClearKeysDown();
        ResetKeys();
        } 
    }

////////////////////////////////////////////////
//  Checks to see if we should be in menus
////////////////////////////////////////////////
void 
MNU_CheckForMenus(void)
    {
    extern BOOL GamePaused;

    if (UsingMenus)
        {
        //if (MoveSkip2 == 0)
            MNU_DoMenu(ct_mainmenu, Player + myconnectindex);
        }
    else
        {
        if (KEY_PRESSED(KEYSC_ESC) && dimensionmode == 3 && !ConPanel)
            {
            KEY_PRESSED(KEYSC_ESC) = 0;
            KB_ClearKeysDown();
            // setup sliders/buttons
            MNU_InitMenus();
            MNU_DoMenu(ct_mainmenu, Player + myconnectindex);
            pMenuClearTextLine(Player + myconnectindex);
            PauseGame();
            }
        }
    }

void 
MNU_CheckForMenusAnyKey(void)
    {
    if (UsingMenus)
        {
        //if (MoveSkip2 == 0)
            MNU_DoMenu(ct_mainmenu, Player + myconnectindex);
        }
    else
        {
        if (KeyPressed())
            {
            ResetKeys();
            KB_ClearKeysDown();
            MNU_InitMenus();
            MNU_DoMenu(ct_mainmenu, Player + myconnectindex);
            pMenuClearTextLine(Player + myconnectindex);
            }
        }
    }



///////////////////////////////////////////////////////////////////////////////////////////////////
// Miscellaneous Routines
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct RGB_color_typ
    {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    } RGB_color, *RGB_color_ptr;

#define PALETTE_MASK            0x3c6
#define PALETTE_READ            0x3c7
#define PALETTE_WRITE           0x3c8
#define PALETTE_DATA            0x3c9

unsigned char palette_data[256][3];     // Global palette array
unsigned char opalette_data[256][3];    // Global palette array

// V E R T I C A L   R E T R A C E   V A R I A B L E S //////////////////////////////////////////

#define VGA_INPUT_STATUS_1      0x3DA   // VGA status register 1, bit 3 is the vsync
                    // 1 = retrace in progress
                    // 0 = no retrace
#define VGA_VSYNC_MASK          0x08    // Masks off unwanted bits of status register.


// These routines are not used and should not be used.  Would interfere with VESA palette
// cards.    
#if 0
/////////////////////////////////////////////////
// WaitForVsync
// Waits for a vertical retrace to occur.  If one is in progress, it waits for the next one.
/////////////////////////////////////////////////
void 
WaitForVsync(void)
    {
    while (inp(VGA_INPUT_STATUS_1) & VGA_VSYNC_MASK);
    // Retrace in progress, wait.

    // Wait for vsync, and exit.
    while (!inp(VGA_INPUT_STATUS_1) & VGA_VSYNC_MASK);
    }

void 
Get_Palette(unsigned char *pal)
    {
    int i;

    outp(PALETTE_READ, 0);
    for (i = 0; i < 768; i++)
        pal[i] = inp(PALETTE_DATA);
    }
void 
Set_Palette(unsigned char *buff)
    {
    int i;

    outp(PALETTE_WRITE, 0);             // Resets color ram pointer to 1st
                                        // color
    for (i = 0; i < 768; i++)
        outp(PALETTE_DATA, buff[i]);
    }

#endif


/*
=================================================================================================
=
=       FadeOut - Fades the palette to color at assigned click rate
=
=================================================================================================
*/
// Heres some temp timer junk for this routine.  Replace it with game timer stuff later.
//unsigned int *clock  = (unsigned int *)0x046C;

void 
Fade_Timer(int clicks)
    {
//        unsigned int now;
    long now;

    now = totalclock;

    while (abs(totalclock - now) < clicks);
    }

void 
FadeIn(unsigned char startcolor, unsigned int clicks)
    {
    int i, palreg, usereg, tmpreg1 = 0, tmpreg2 = 0;
    RGB_color color;
    unsigned char temp_pal[768], *palette;


    palette = &palette_data[0][0];

    color.red = palette_data[startcolor][0];
    color.green = palette_data[startcolor][1];
    color.blue = palette_data[startcolor][2];

    usereg = 0;
    for (i = 0; i < 768; i++)
        {
        if (usereg == 0)
            temp_pal[i] = color.red;
        else 
        if (usereg == 1)
            temp_pal[i] = color.green;
        else
            temp_pal[i] = color.blue;

        if (++usereg > 2)
            usereg = 0;
        }

    for (i = 0; i < 32; i++)
        {
        for (palreg = 0; palreg < 768; palreg++)
            {
            tmpreg1 = (int) (temp_pal[palreg]) + 2;
            tmpreg2 = (int) (temp_pal[palreg]) - 2;
            if (tmpreg1 > 255)
                tmpreg1 = 255;
            if (tmpreg2 < 0)
                tmpreg2 = 0;

            if (temp_pal[palreg] < palette[palreg])
                {
                if ((temp_pal[palreg] = tmpreg1) > palette[palreg])
                    temp_pal[palreg] = palette[palreg];
                }
            else 
            if (temp_pal[palreg] > palette[palreg])
                if ((temp_pal[palreg] = tmpreg2) < palette[palreg])
                    temp_pal[palreg] = palette[palreg];

            }

        set_pal(&temp_pal[0]);

        // Delay clicks
        Fade_Timer(clicks);
        }
    }

void 
FadeOut(unsigned char targetcolor, unsigned int clicks)
    {
    int i, palreg, usereg = 0, tmpreg1 = 0, tmpreg2 = 0;
    RGB_color color;
    unsigned char temp_pal[768];


    color.red = palette_data[targetcolor][0];
    color.green = palette_data[targetcolor][1];
    color.blue = palette_data[targetcolor][2];

    memcpy(&temp_pal[0], &palette_data[0][0], 768);

    for (i = 0; i < 32; i++)
        {
        for (palreg = 0; palreg < 768; palreg++)
            {
            tmpreg1 = (int) (temp_pal[palreg]) + 2;
            tmpreg2 = (int) (temp_pal[palreg]) - 2;
            if (tmpreg1 > 255)
                tmpreg1 = 255;
            if (tmpreg2 < 0)
                tmpreg2 = 0;

            if (usereg == 0)
                {
                if (temp_pal[palreg] < color.red)
                    {
                    if ((temp_pal[palreg] = tmpreg1) > color.red)
                        temp_pal[palreg] = color.red;
                    }
                else 
                if (temp_pal[palreg] > color.red)
                    if ((temp_pal[palreg] = tmpreg2) < color.red)
                        temp_pal[palreg] = color.red;
                }
            else 
            if (usereg == 1)
                {
                if (temp_pal[palreg] < color.green)
                    {
                    if ((temp_pal[palreg] = tmpreg1) > color.green)
                        temp_pal[palreg] = color.green;
                    }
                else 
                if (temp_pal[palreg] > color.green)
                    if ((temp_pal[palreg] = tmpreg2) < color.green)
                        temp_pal[palreg] = color.green;
                }
            else 
            if (usereg == 2)
                {
                if (temp_pal[palreg] < color.blue)
                    {
                    if ((temp_pal[palreg] = tmpreg1) > color.blue)
                        temp_pal[palreg] = color.blue;
                    }
                else 
                if (temp_pal[palreg] > color.blue)
                    if ((temp_pal[palreg] = tmpreg2) < color.blue)
                        temp_pal[palreg] = color.blue;
                }

            if (++usereg > 2)
                usereg = 0;
            }


        set_pal(&temp_pal[0]);

        // Delay clicks
        Fade_Timer(clicks);
        }
    }

//////////////////////////////////////////////////////////////////////////////

#define FADE_DAMAGE_FACTOR  3   // 100 health / 32 shade cycles = 3.125

unsigned char ppalette[MAX_SW_PLAYERS][768];

//////////////////////////////////////////
// Set the amount of redness for damage
// the player just took
//////////////////////////////////////////
void 
SetFadeAmt(PLAYERp pp, short damage, unsigned char startcolor)
    {
    int palreg, usereg = 0, tmpreg1 = 0, tmpreg2 = 0;
    short fadedamage=0;
    RGB_color color;

    //CON_ConMessage("SetAmt: fadeamt = %d, startcolor = %d, pp = %d",pp->FadeAmt,startcolor,pp->StartColor);

    if (abs(pp->FadeAmt) > 0 && startcolor == pp->StartColor)
        return;

    // Don't ever over ride flash bomb
    if (pp->StartColor == 1 && abs(pp->FadeAmt) > 0)    
        return;
        
    // Reset the palette
    if(pp == Player + screenpeek)
        {
        COVERsetbrightness(gs.Brightness,(char *)palette);
        COVERsetbrightness(gs.Brightness,(char *)palette_data);
        if (pp->FadeAmt <= 0)
            GetPaletteFromVESA(&ppalette[screenpeek][0]);
        }

    if(damage < -150 && damage > -1000) fadedamage = 150;
    else
    if(damage < -1000)  // Underwater
        fadedamage = abs(damage+1000);
    else
        fadedamage = abs(damage);

    if(damage >= -5 && damage < 0)
        fadedamage += 10;

    // Don't let red to TOO red
    if(startcolor == COLOR_PAIN && fadedamage > 100) fadedamage = 100;

    pp->FadeAmt = fadedamage / FADE_DAMAGE_FACTOR;

    if (pp->FadeAmt <= 0)
        {
        pp->FadeAmt = 0;
        return;
        }

    // It's a health item, just do a preset flash amount
    if (damage > 0)
        pp->FadeAmt = 3;

    pp->StartColor = startcolor;

    pp->FadeTics = 0;

    // Set player's palette to current game palette
    GetPaletteFromVESA(pp->temp_pal);

    color.red = palette_data[pp->StartColor][0];
    color.green = palette_data[pp->StartColor][1];
    color.blue = palette_data[pp->StartColor][2];

    for (palreg = 0; palreg < 768; palreg++)
        {
        tmpreg1 = (int) (pp->temp_pal[palreg]) + ((2 * pp->FadeAmt) + 4);
        tmpreg2 = (int) (pp->temp_pal[palreg]) - ((2 * pp->FadeAmt) + 4);
        if (tmpreg1 > 255)
            tmpreg1 = 255;
        if (tmpreg2 < 0)
            tmpreg2 = 0;

        if (usereg == 0)
            {
            if (pp->temp_pal[palreg] < color.red)
                {
                if ((pp->temp_pal[palreg] = tmpreg1) > color.red)
                    pp->temp_pal[palreg] = color.red;
                }
            else 
            if (pp->temp_pal[palreg] > color.red)
                if ((pp->temp_pal[palreg] = tmpreg2) < color.red)
                    pp->temp_pal[palreg] = color.red;
            }
        else 
        if (usereg == 1)
            {
            if (pp->temp_pal[palreg] < color.green)
                {
                if ((pp->temp_pal[palreg] = tmpreg1) > color.green)
                    pp->temp_pal[palreg] = color.green;
                }
            else 
            if (pp->temp_pal[palreg] > color.green)
                if ((pp->temp_pal[palreg] = tmpreg2) < color.green)
                    pp->temp_pal[palreg] = color.green;
            }
        else 
        if (usereg == 2)
            {
            if (pp->temp_pal[palreg] < color.blue)
                {
                if ((pp->temp_pal[palreg] = tmpreg1) > color.blue)
                    pp->temp_pal[palreg] = color.blue;
                }
            else 
            if (pp->temp_pal[palreg] > color.blue)
                if ((pp->temp_pal[palreg] = tmpreg2) < color.blue)
                    pp->temp_pal[palreg] = color.blue;
            }

        if (++usereg > 2)
            usereg = 0;
        }

    // Do initial palette set
    if(pp == Player + screenpeek)
        {
        set_pal(pp->temp_pal);
        if (damage < -1000)
            pp->FadeAmt = 1000;  // Don't call DoPaletteFlash for underwater stuff
        }
    }

//////////////////////////////////////////
// Do the screen reddness based on damage
//////////////////////////////////////////
#define MAXFADETICS     5
void 
DoPaletteFlash(PLAYERp pp)
    {
    int i, palreg, tmpreg1 = 0, tmpreg2 = 0;
    unsigned char *pal_ptr = &ppalette[screenpeek][0];


    if (pp->FadeAmt <= 1)
        {
        pp->FadeAmt = 0;
        pp->StartColor = 0;
        if(pp == Player + screenpeek)
            {
            set_pal(palette_data);
            COVERsetbrightness(gs.Brightness,(char *)palette);
            COVERsetbrightness(gs.Brightness,(char *)palette_data);
            memcpy(pp->temp_pal, palette_data, sizeof(palette_data));
            DoPlayerDivePalette(pp);  // Check Dive again
            DoPlayerNightVisionPalette(pp);  // Check Night Vision again
            }

        return;
        }

        
    pp->FadeTics += synctics;           // Add this frame's tic amount to
                                        // counter

    if (pp->FadeTics >= MAXFADETICS)
        {
        while (pp->FadeTics >= MAXFADETICS)
            {
            pp->FadeTics -= MAXFADETICS;

            pp->FadeAmt--;              // Decrement FadeAmt till it gets to
                                        // 0 again.
            }
        }
    else
        return;                         // Return if they were not >
                                        // MAXFADETICS

    if (pp->FadeAmt > 32)
        return;

    if (pp->FadeAmt <= 1)
        {
        pp->FadeAmt = 0;
        pp->StartColor = 0;   
        if(pp == Player + screenpeek)
            {
            set_pal(palette_data);
            COVERsetbrightness(gs.Brightness,(char *)palette);
            COVERsetbrightness(gs.Brightness,(char *)palette_data);
            memcpy(pp->temp_pal, palette_data, sizeof(palette_data)); 
            DoPlayerDivePalette(pp);  // Check Dive again
            DoPlayerNightVisionPalette(pp);  // Check Night Vision again
            }
        return;
        }
    else
        {
        //CON_Message("gamavalues = %d, %d, %d",pp->temp_pal[pp->StartColor],pp->temp_pal[pp->StartColor+1],pp->temp_pal[pp->StartColor+2]);
        for (palreg = 0; palreg < 768; palreg++)
            {
            tmpreg1 = (int) (pp->temp_pal[palreg]) + 2;
            tmpreg2 = (int) (pp->temp_pal[palreg]) - 2;
            if (tmpreg1 > 255)
                tmpreg1 = 255;
            if (tmpreg2 < 0)
                tmpreg2 = 0;

            if (pp->temp_pal[palreg] < pal_ptr[palreg])
                {
                if ((pp->temp_pal[palreg] = tmpreg1) > pal_ptr[palreg])
                    pp->temp_pal[palreg] = pal_ptr[palreg];
                }
            else 
            if (pp->temp_pal[palreg] > pal_ptr[palreg])
                if ((pp->temp_pal[palreg] = tmpreg2) < pal_ptr[palreg])
                    pp->temp_pal[palreg] = pal_ptr[palreg];

            }

        // Only hard set the palette if this is currently the player's view
        if(pp == Player + screenpeek)
            {
            set_pal(pp->temp_pal);
            }

        }

    }

VOID ResetPalette(PLAYERp pp)
    {
    set_pal(palette_data);
    COVERsetbrightness(gs.Brightness,(char *)palette);
    COVERsetbrightness(gs.Brightness,(char *)palette_data);
    memcpy(pp->temp_pal, palette_data, sizeof(palette_data));
    //DoPlayerDivePalette(pp);  // Check Dive again
    //DoPlayerNightVisionPalette(pp);  // Check Night Vision again
    pp->FadeAmt = 0;
    pp->StartColor = 0;
    pp->FadeTics = 0;
    }
    
