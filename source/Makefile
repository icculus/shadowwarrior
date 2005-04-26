#-----------------------------------------------------------------------------#
# Duke3D makefile.
#-----------------------------------------------------------------------------#

linux64 := true
linux_ppc := false
beos := false
macosx := false
solaris := false
shareware := false
controls_menu := true
#use_asm := true

#-----------------------------------------------------------------------------#
# If this makefile fails to detect Cygwin correctly, or you want to force
#  the build process's behaviour, set it to "true" or "false" (w/o quotes).
#-----------------------------------------------------------------------------#
#cygwin := true
#cygwin := false
cygwin := autodetect

# you only need to set these for Cygwin at the moment.
SDL_INC_DIR = /cygdrive/c/SDL/include
SDL_LIB_DIR = /cygdrive/c/SDL/lib

# need this for now.
ifeq ($(strip $(beos)),true)
use_asm := false
endif

# Don't touch anything below this line unless you know what you're doing.

ifeq ($(strip $(cygwin)),autodetect)
ifneq ($(strip $(shell gcc -v 2>&1 |grep "cygwin")),)
cygwin := true
else
cygwin := false
endif
endif


ifeq ($(strip $(cygwin)),true)
ifeq ($(strip $(SDL_INC_DIR)),please_set_me_cygwin_users)
$(error Cygwin users need to set the SDL_INC_DIR envr var.)
else
SDL_CFLAGS := -I$(SDL_INC_DIR)
endif

ifeq ($(strip $(SDL_LIB_DIR)),please_set_me_cygwin_users)
$(error Cygwin users need to set the SDL_LIB_DIR envr var.)
else
SDL_LDFLAGS := -L$(SDL_LIB_DIR) -lSDL
endif
else
SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LDFLAGS := $(shell sdl-config --libs) -L.
endif

ifeq ($(strip $(macosx)),true)
EXTRACFLAGS += -DPLATFORM_MACOSX=1
EXTRALDFLAGS += -lSDLmain
endif

ifeq ($(strip $(linux_ppc)),true)
EXTRACFLAGS += -DPLATFORM_LINUXPPC=1
endif

ifneq ($(strip $(cygwin)),true)
ifneq ($(strip $(macosx)),true)
ifneq ($(strip $(beos)),true)
EXTRACFLAGS += -DUSE_EXECINFO=1
endif
endif
endif

ifeq ($(strip $(solaris)),true)
EXTRALDFLAGS += -lsocket -lnsl
endif

ifeq ($(strip $(shareware)),true)
EXTRACFLAGS += -DVOLUMEONE
else
EXTRACFLAGS += -DVOLUMEALL
endif

ifeq ($(strip $(controls_menu)),true)
EXTRACFLAGS += -DCONTROLS_CONFIG_MENU=1
endif

# check out buildengine in this directory:
#  cvs -z3 -d:pserver:anonymous@cvs.icculus.org:/cvs/cvsroot login
#   (password is "anonymous", without the quotes.)
#  cvs -z3 -d:pserver:anonymous@cvs.icculus.org:/cvs/cvsroot co buildengine
#  cd buildengine
#  make
#  cd ..
#  This sucks. We'll figure out something better.
BUILDOBJS := \
buildengine/cache1d.o \
buildengine/engine.o \
buildengine/sdl_driver.o \
buildengine/mmulti.o \
buildengine/pragmas.o \
buildengine/unix_compat.o

ifeq ($(strip $(use_asm)),true)
BUILDOBJS += buildengine/a_gnu.o buildengine/a_nasm.o
else
BUILDOBJS += buildengine/a.o
endif

CC = gcc
CFLAGS = -c -g $(SDL_CFLAGS) -DUSE_SDL=1 -DPLATFORM_UNIX=1 -Wno-unused $(EXTRACFLAGS) -funsigned-char

ifeq ($(strip $(linux64)),true)
  CFLAGS += -m32
  EXTRALDFLAGS +=-m32 -L/emul/linux/x86/usr/lib
endif

#CFLAGS += -Wall
CFLAGS += -w

ifeq ($(strip $(macosx)),true)
OPTIMIZE = -O3 -mdynamic-no-pic -falign-loops=16
else
#OPTIMIZE = -O3
endif

OPTIMIZE = -O0

# Always turn OFF strict aliasing, even when optimizing. Otherwise, this is
#  just an accident waiting to happen...  --ryan.
CFLAGS += -fno-strict-aliasing

# Uncomment this to compile with the Intel compiler (v6.0)
#CC = icc
#CFLAGS = -g $(SDL_CFLAGS) -DUSE_SDL=1 -DPLATFORM_UNIX=1 -DUSE_I386_ASM=1 $(EXTRACFLAGS) -O2

LDLIBS = $(SDL_LDFLAGS) -lSDL -lSDL_mixer $(EXTRALDFLAGS)

# !!! FIXME: Do we even need this? It doesn't fly on MacOS X.  --ryan.
#LDLIBS += -Wl,-E

#all: shadow build
all: shadow


%.o : %.c _%.h
	$(CC) $(CFLAGS) $(OPTIMIZE) -o $@ $<

# Animation playback crashes due to optimization error on MacOS X. --ryan.
ifeq ($(strip $(macosx)),true)
animlib.o : animlib.c
	$(CC) $(CFLAGS) -o $@ $<
endif

# Animation playback crashes due to optimization error on Linux PPC. --Felipe Barriga.
ifeq ($(strip $(linux_ppc)),true)
animlib.o: animlib.c
	$(CC) $(CFLAGS) -o $@ $<
endif

audiolib/audiolib.a:
	$(MAKE) -C audiolib CC="$(CC)" CFLAGS="$(CFLAGS)" LDLIBS="$(LDLIBS)"

shadow: 	\
actor.o \
ai.o \
anim.o \
border.o \
break.o \
bunny.o \
cache.o \
cd.o \
cheats.o \
colormap.o \
config.o \
console.o \
coolg.o \
coolie.o \
copysect.o \
demo.o \
draw.o \
eel.o \
game.o \
girlninj.o \
goro.o \
hornet.o \
interp.o \
interpsh.o \
inv.o \
jplayer.o \
jsector.o \
jweapon.o \
kbd.o \
lava.o \
light.o \
mclip.o \
mdastr.o \
menus.o \
miscactr.o \
morph.o \
net.o \
ninja.o \
panel.o \
pcx.o \
player.o \
predict.o \
quake.o \
ripper.o \
ripper2.o \
rooms.o \
rotator.o \
rts.o \
save.o \
scrip2.o \
sector.o \
serp.o \
setup.o \
skel.o \
skull.o \
slidor.o \
sounds.o \
spike.o \
sprite.o \
sumo.o \
swconfig.o \
symutil.o \
sync.o \
text.o \
track.o \
usrhooks.o \
vator.o \
vis.o \
wallmove.o \
warp.o \
weapon.o \
zilla.o \
zombie.o \
timer.o \
animlib.o \
dukemusc.o \
control.o \
keyboard.o \
global.o \
scriplib.o \
audiolib/audiolib.a
	$(CC) $^ $(BUILDOBJS) $(LDLIBS) -o $@

#build: astub.o
#	$(CC) $^ $(BUILDOBJS) buildengine/build.o $(LDLIBS) -o $@

clean:
	$(MAKE) -C audiolib clean
	rm -rf shadow build *.o

distclean: clean
	$(MAKE) -C audiolib distclean
	rm -rf *~
