#ifndef _INCL_SHADOW_H_
#define _INCL_SHADOW_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if !PLATFORM_MACOSX
#include <malloc.h>
#endif

#include <fcntl.h>
#include <time.h>
#include <ctype.h>

#if (!defined MAX_PATH)
  #if (defined MAXPATHLEN)
    #define MAX_PATH MAXPATHLEN
  #elif (defined PATH_MAX)
    #define MAX_PATH PATH_MAX
  #else
    #define MAX_PATH 256
  #endif
#endif

#if PLATFORM_DOS
#include <dos.h>
#include <bios.h>
#include <io.h>
#define PATH_SEP_CHAR '\\'
#define PATH_SEP_STR  "\\"
#endif

#if PLATFORM_UNIX
#include "swunix.h"
#endif

#if PLATFORM_WIN32
#include "swwin.h"
#endif

#include "mytypes.h"

#ifndef GCC_PACK1_EXT
#define GCC_PACK1_EXT
#endif

#if USE_SDL
#include "SDL.h"
#include "SDL_mixer.h"
#endif

#include "buildengine/pragmas.h"
//#undef getch	// quick hack
#include "buildengine/build.h"

#endif


