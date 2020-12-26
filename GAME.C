/**
 ** LEGOIDS - game.c
 **
 ** Game-related functions (e.g. generating new levels)
 **
 *************************************/

#include <allegro.h>
#include <ppcol.h>
#include <stdio.h>
#include <stdlib.h>  // random numbers
#include "globals.h"
#include "lists.h"
#include "functs.h"

#define DEADWAIT 120

/* inline void checkbullet(BULLETLIST *bulletlist)
{
     int i;
     ASTEROID *asteroid, *asteroid2;
     BULLET *bullet;

     bullet=bulletlist->first;
     while(bullet!=(BULLET *)NULL)
     {
          if(check_pp_collision(bull,asteroid->pic,fixtoi(bullet->xpos),fixtoi(bullet->ypos), fixtoi(asteroid->xpos-(asteroid->size * itofix(8))),fixtoi(asteroid->ypos-(asteroid->size * itofix(8))) ))
          {
               delbullet(bulletlist,bullet);
               if((asteroid->size) > 1)
               {
                    for(i=0;i<(asteroid->size / 2);i++)
                    {
                         asteroid2=addasteroid(asteroidlist,(asteroid->xpos)+itofix( (random() % 6) - 3 ),(asteroid->ypos)+itofix( (random() % 6) - 3 ), (asteroid->xspeed)*( (double) (random()%251)/(100) ), (asteroid->yspeed)*( (double) (random()%251)/(100) ), (asteroid->size) / 2);
                         stretch_blit(all,asteroid2->pic,32,0,16,16,0,0,16*asteroid2->size,16*asteroid2->size);
                    }
               }
               game_status.score+=(asteroid->size)*25;
               delasteroid(asteroidlist,asteroid);
               break;
          }
          else
               bullet=bullet->next;
     }
}
*/

inline void newplayer(void)
{
     /* initialize player's ship */
     player.xpos = itofix(159);
     player.ypos = itofix(99);
     player.xspeed = 0;
     player.yspeed = 0;
     player.angle = itofix(128);
     clear(player.pic);
     rotate_sprite(player.pic,ship,0,0,itofix(128));

     /* set some game status variables */
     game_status.waitplace=DEADWAIT;
}

inline void killplayer(BULLETLIST *bulletlist)
{
     int i,k;

     game_status.lives--;

     for(i=0;i<64;i++)
     {
          k=random() & 255;
          addbullet(bulletlist, player.xpos, player.ypos, player.xspeed-((double)i/32)*(fsin(itofix(k))), player.yspeed+((double)i/32)*(fcos(itofix(k))), 30);
     }

     newplayer();
}

void newgame(ASTEROIDLIST *asteroidlist, BULLETLIST *bulletlist)
{
     /* empty asteroid and bullet lists */
     while(asteroidlist->asteroids)
          delasteroid(asteroidlist, asteroidlist->first);

     while(bulletlist->bullets)
          delbullet(bulletlist, bulletlist->first);

     /* initialize general game status flags & variables */
     game_status.gameover=FALSE;
     game_status.shotflag=0;
     game_status.counter=0;
     game_status.drawflag=FALSE;
     game_status.frames=0;
     game_status.playerhit=FALSE;
     game_status.showflag=FALSE;
     game_status.showfps=FALSE;
     game_status.saveflag=FALSE;
     game_status.lives=4;
     game_status.level=1;
     game_status.score=0;

     /* init player's ship and set waitplace to 2 seconds */
     newplayer();
}

/* Randomly build asteroid list for new level
   Returns zero on success or -1 on error
   Arguments:
   asteroidlist - pointer to main game asteroid list
      asteroids - number of asteroids to create
*/
inline int newlevel(ASTEROIDLIST *asteroidlist, int asteroids)
{
     int i,k=4;
     fixed temp1,temp2;
     ASTEROID *asteroid = (ASTEROID *) NULL;

     /* build asteroid list */
     for(i=0;i<asteroids;i++)
     {
          temp1=0;
          temp2=0;

          /* pick random x and y speeds, but make sure
             they don't result in a nonmoving asteroid! */
          do
          {
               temp1 = ftofix((((double) random() / RAND_MAX) * 1) - .5);
               temp2 = ftofix((((double) random() / RAND_MAX) * 1) - .5);
          }
          while(!temp1 && !temp2);

          asteroid = addasteroid(asteroidlist, itofix(random() % 304), itofix(random() % 184), temp1, temp2, k);

          if(asteroid == (ASTEROID *) NULL)
          {
               allegro_exit();
               printf("newlevel(): Error creating asteroid\n");
               return(-1);
          }

          stretch_blit(all,asteroid->pic,32,0,16,16,0,0,16*asteroid->size,16*asteroid->size);
     }
     return(0);
}
