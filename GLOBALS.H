/**
 ** LEGOIDS - globals.h
 **
 ** global variable declarations
 **
 *************************************/

#ifndef globals_h
#define globals_h

#include <allegro.h>
#include "structs.h"

extern PALETTE pal;
extern SHIP player; // player's ship info
extern GAME_STATUS game_status; // general game status
extern BITMAP *buffer, *all, *ship, *robfont; /* double buffer, main tilemap,
                                                 unrotated player bitmap, and
                                                 Robotech font */
extern DATAFILE *legdat; // legoids.dat datafile
#endif
