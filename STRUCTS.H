/**
 ** LEGOIDS - structs.h
 **
 ** structures, typedefs, etc.
 **
 *************************************/

#ifndef structs_h
#define structs_h

#include <allegro.h>

/* structures:
   GAME_STATUS  - general game status variables
   SHIP         - ship object
   BULLET       - bullet object (list node)
   BULLETLIST   - bullet linked list for bullet objects
   ASTEROID     - asteroid object (list node)
   ASTEROIDLIST - asteroid linked list for asteroid objects
*/

typedef struct GAME_STATUS_STRUCT
{
     int counter;   // game cycle counter
     int drawflag;  // buffer changed since last screen write flag
     int frames;    // actual frames drawn to screen
     int gameover;  // is the game over?
     int playerhit; // player is on top of an asteroid waiting for placement
     int level;     // current player level
     int lives;     // player lives in reserve
     int saveflag;  // screenshot key pressed flag
     int score;     // current score
     int shotflag;  // cycles until player can fire again / fire key pressed flag
     int showflag;  // FPS key pressed flag
     int showfps;   // FPS display toggle status
     int waitplace; // wait for new player placement flag
     int lastfps; // results of last frames-per-second calculation
} GAME_STATUS;

typedef struct SHIP_STRUCT
{
     fixed xpos, ypos;
     fixed xspeed, yspeed;
     fixed angle;
     BITMAP *pic;
} SHIP;

typedef struct BULLET_STRUCT
{
     fixed xpos, ypos;
     fixed xspeed, yspeed;
     int time_to_live;
     struct BULLET_STRUCT *last, *next;
} BULLET;

typedef struct BULLETLIST_STRUCT
{
     int bullets;
     BULLET *first;
} BULLETLIST;

typedef struct ASTEROID_STRUCT
{
     int size;
     fixed xpos, ypos;
     fixed xspeed, yspeed;
     BITMAP *pic;
     struct ASTEROID_STRUCT *last, *next;
} ASTEROID;

typedef struct ASTEROIDLIST_STRUCT
{
     int asteroids;
     ASTEROID *first;
} ASTEROIDLIST;

#endif
