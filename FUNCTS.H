/**
 ** LEGOIDS - functs.h
 **
 ** general function declarations (from init.c, etc.)
 **
 *************************************/

#ifndef functs_h
#define functs_h

/* init() (init.c) */
int init(void);

/* killplayer() (game.c) */
inline void killplayer(BULLETLIST *bulletlist);

/* newplayer() (game.c) */
inline void newplayer(void);

/* newgame() (game.c) */
void newgame(ASTEROIDLIST *asteroidlist, BULLETLIST *bulletlist);

/* newlevel() (game.c) */
inline int newlevel(ASTEROIDLIST *asteroidlist, int asteroids);

#endif
