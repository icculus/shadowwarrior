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

#if 1 

//#include <dos.h>
#include <stdio.h>
#include <stdarg.h>
//#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#if PLATFORM_DOS
#include <io.h>
#include <graph.h>
#endif

#include "proto.h"
#include "names2.h"
#include "panel.h"
#include "game.h"
#include "tags.h"
#include "sector.h"
#include "sprite.h"
#include "weapon.h"
#include "player.h"
#include "lists.h"
#include "pal.h"
#include "types.h"
#include "break.h"
#include "ai.h"
#include "jsector.h"
#include "parent.h"
#include "menus.h"
#include "mfile.h"
#include "slidor.h"
#include "task_man.h"
#include "buildengine/pragmas.h"


#include "actor.def"
//#include "ai.def"
#include "border.def"
#include "break.def"
#include "cd.def"
#include "cheats.def"
#include "colormap.def"
#include "console.def"
#include "coolg.def"
#include "coolie.def"
#include "copysect.def"
#include "demo.def"
#include "draw.def"
#include "eel.def"
#include "game.def"
#include "goro.def"
#include "hornet.def"
#include "inv.def"
#include "jplayer.def"
#include "jsector.def"
#include "jweapon.def"
#include "kbd.def"
#include "lava.def"
#include "light.def"
#include "mclip.def"
#include "mdastr.def"
#include "menus.def"
#include "midi.def"
#include "miscactr.def"
#include "morph.def"
#include "net.def"
#include "ninja.def"
#include "panel.def"
#include "player.def"
#include "predict.def"
#include "quake.def"
#include "ripper.def"
#include "ripper2.def"
#include "rooms.def"
#include "rotator.def"
#include "save.def"
#include "scrip2.def"
#include "sector.def"
#include "serp.def"
#include "setup.def"
#include "skel.def"
#include "skull.def"
#include "slidor.def"
#include "sounds.def"
#include "spike.def"
#include "sprite.def"
#include "sumo.def"
#include "sync.def"
#include "text.def"
#include "timer.def"
#include "track.def"
#include "usrhooks.def"
#include "vator.def"
#include "wallmove.def"
#include "warp.def"
#include "weapon.def"

// Build              
#include "mmulti.def" 
#include "build.def"  
#include "engine.def" 
#include "cache1d.def"
                      
#endif                
