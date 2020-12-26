/**
 ** LEGOIDS - lists.c
 **
 ** linked list routines, etc. (include lists.h for function decl'ns)
 **
 *************************************/

#include <malloc.h>
#include "structs.h"

#include <allegro.h> // needed for putpixel call made in debug mode and asteroid init

/* BULLET LIST ROUTINES:

   newbulletlist() - create and initialize a new bullet list
   addbullet()     - add a new bullet to the end of a bullet list
   delbullet()     - remove an arbitrary bullet from a bullet list
*/

/* create a new bullet list (pointer to list optional)
   returns a pointer to the new list */
BULLETLIST *newbulletlist(BULLETLIST *list)
{
     if(!list) // bad/no ptr passed, make new list
     {
          list = malloc(sizeof(BULLETLIST)); // grab some memory
          if(!list)
               return((BULLETLIST *)NULL); // malloc failed, return NULL pointer
     }
     // init the list
     list->bullets = 0;
     list->first = (BULLET *) NULL;
     return(list);
}

/* add bullet to the end of bullet list using passed values
   returns a pointer to the new bullet, or a pointer to NULL on failure */
inline BULLET *addbullet(BULLETLIST *list, fixed xpos, fixed ypos, fixed xspeed, fixed yspeed, int time_to_live)
{
     BULLET *nodeptr=list->first;

     if(nodeptr) // list not empty
     {
          while(nodeptr->next) // advance to last node in list, but no further
               nodeptr=nodeptr->next;

          // grab some memory and set pointers
          nodeptr->next = malloc(sizeof(BULLET));
          if(nodeptr->next == (BULLET *) NULL) // bad malloc, return NULL
               return((BULLET *)NULL);
          nodeptr->next->last = nodeptr; // point new node's last pointer to previous node on list
          nodeptr=nodeptr->next;
     }
     else // empty list; make an initial entry
     {
          list->first = malloc(sizeof(BULLET));
          if(list->first == (BULLET *) NULL) // bad malloc, return NULL
               return((BULLET *)NULL);
          list->first->last = (BULLET *) NULL; // make new first entry point backwards to NULL
          nodeptr = list->first;
     }

     // init new node with passed values
     nodeptr->xpos=xpos;
     nodeptr->ypos=ypos;
     nodeptr->xspeed=xspeed;
     nodeptr->yspeed=yspeed;
     nodeptr->time_to_live=time_to_live;

     nodeptr->next = (BULLET *) NULL; // nothing comes after the new node
     list->bullets++; // we have another bullet on the list now

     return(nodeptr);
}

/* remove bullet indicated by passed pointer from bullet list
   returns remaining number of bullets on list, or -1 on error */
inline int delbullet(BULLETLIST *list, BULLET *bullet)
{
     BULLET *temp;

     // check for no bullet passed, or bullet that isn't in a list! May be unnecessary (read: optimization food)
     if(bullet == (BULLET *) NULL)
          return(-1);

     if(bullet->last) // not first entry on the list, alter previous node's pointer
          bullet->last->next = bullet->next;
     else
          list->first = bullet->next; // make second node into first node

     if(bullet->next) // not last on list
     {
          bullet->next->last = bullet->last;

          temp=bullet;
          bullet=bullet->next; // advance to node after break
          free(temp); // trash old node
     }
     else
     {
          free(bullet);
          bullet = (BULLET *) NULL;
     }

     return(--(list->bullets)); // reduce bullet count by 1 and return new count
}

/* asteroid LIST ROUTINES:

   newasteroidlist() - create and initialize a new asteroid list
   addasteroid()     - add a new asteroid to the end of a asteroid list
   delasteroid()     - remove an arbitrary asteroid from a asteroid list
*/

/* create a new asteroid list (pointer to list optional)
   returns a pointer to the new list */
ASTEROIDLIST *newasteroidlist(ASTEROIDLIST *list)
{
     if(!list) // bad/no ptr passed, make new list
     {
          list = malloc(sizeof(ASTEROIDLIST)); // grab some memory
          if(!list)
               return((ASTEROIDLIST *)NULL); // malloc failed, return NULL pointer
     }
     // init the list
     list->asteroids = 0;
     list->first = (ASTEROID *) NULL;
     return(list);
}

/* add asteroid to the end of asteroid list using passed values
   returns a pointer to the new asteroid, or a pointer to NULL on failure */
inline ASTEROID *addasteroid(ASTEROIDLIST *list, fixed xpos, fixed ypos, fixed xspeed, fixed yspeed, int size)
{
     ASTEROID *nodeptr=list->first;

     if(nodeptr) // list not empty
     {
          while(nodeptr->next) // advance to last node in list, but no further
               nodeptr=nodeptr->next;

          // grab some memory and set pointers
          nodeptr->next = malloc(sizeof(ASTEROID));
          if(nodeptr->next == (ASTEROID *) NULL) // bad malloc, return NULL
          {
               return((ASTEROID *)NULL);
          }
          nodeptr->next->last = nodeptr; // point new node's last pointer to previous node on list
          nodeptr=nodeptr->next;
     }
     else // empty list; make an initial entry
     {
          list->first = malloc(sizeof(ASTEROID));
          if(list->first == (ASTEROID *) NULL) // bad malloc, return NULL
          {
               return((ASTEROID *)NULL);
          }
          list->first->last = (ASTEROID *) NULL; // make new first entry point backwards to NULL
          nodeptr = list->first;
     }

     // init new node with passed values
     nodeptr->xpos=xpos;
     nodeptr->ypos=ypos;
     nodeptr->xspeed=xspeed;
     nodeptr->yspeed=yspeed;
     nodeptr->size=size;

     nodeptr->pic = create_bitmap(16*nodeptr->size,16*nodeptr->size);
     if(nodeptr->pic == (BITMAP *) NULL) // danger will robinson! danger!
     {
          free(nodeptr);
          return((ASTEROID *)NULL);
     }

     nodeptr->next = (ASTEROID *) NULL; // nothing comes after the new node
     list->asteroids++; // we have another asteroid on the list now

     return(nodeptr);
}

/* remove asteroid indicated by passed pointer from asteroid list
   returns remaining number of asteroids on list, or -1 on error */
inline int delasteroid(ASTEROIDLIST *list, ASTEROID *asteroid)
{
     ASTEROID *temp;

     // check for no asteroid passed, or asteroid that isn't in a list! May be unnecessary (read: optimization food)
     if(asteroid == (ASTEROID *) NULL)
          return(-1);

     if(asteroid->last) // not first entry on the list, alter previous node's pointer
          asteroid->last->next = asteroid->next;
     else
          list->first = asteroid->next; // make second node into first node

     if(asteroid->next) // not last on list
     {
          asteroid->next->last = asteroid->last;

          temp=asteroid;
          asteroid=asteroid->next; // advance to node after break
          free(temp); // trash old node
     }
     else
     {
          free(asteroid);
          asteroid = (ASTEROID *) NULL;
     }

     return(--(list->asteroids)); // reduce asteroid count by 1 and return new count
}
