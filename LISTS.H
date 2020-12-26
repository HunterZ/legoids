/**
 ** LEGOIDS - lists.h
 **
 ** lists.c linked list routine function declarations, etc.
 **
 *************************************/

#ifndef lists_h
#define lists_h

/* create a new bullet list (pointer to list optional)
   returns a pointer to the new list */
BULLETLIST *newbulletlist(BULLETLIST *list);

/* add bullet to the end of bullet list using passed values
   returns a pointer to the new bullet, or a pointer to NULL on failure */
BULLET *addbullet(BULLETLIST *list, fixed xpos, fixed ypos, fixed xspeed, fixed yspeed, int time_to_live);

/* remove bullet indicated by passed pointer from bullet list
   returns remaining number of bullets on list, or -1 on error */
int delbullet(BULLETLIST *list, BULLET *bullet);

/* create a new asteroid list (pointer to list optional)
   returns a pointer to the new list */
ASTEROIDLIST *newasteroidlist(ASTEROIDLIST *list);

/* add asteroid to the end of asteroid list using passed values
   returns a pointer to the new asteroid, or a pointer to NULL on failure */
ASTEROID *addasteroid(ASTEROIDLIST *list, fixed xpos, fixed ypos, fixed xspeed, fixed yspeed, int size);

/* remove asteroid indicated by passed pointer from asteroid list
   returns remaining number of asteroids on list, or -1 on error */
int delasteroid(ASTEROIDLIST *list, ASTEROID *asteroid);

#endif
