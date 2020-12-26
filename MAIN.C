/**
 ** LEGOIDS - main.c
 **
 ** main program control and loop
 **
 *************************************/

/* NOTES TO SELF:
   - think about adding a bullet->pic member to the BULLET type;
     this would allow different weapons to have different bullet gfx =)
   - modularize, modularize, modularize!
   - think about passing around a general game status struct to help manage
     all these stupid global and main() variables and stuff =)
   - move most of the code in the main game loop to a seperate function for
     readability and modularity
   - add sound, music, fonts, title screens, high scores, blah, blah, blah...
*/

/* library includes */
#include <stdio.h>   // text mode output and file access
#include <stdlib.h>  // random numbers
#include <string.h>  // string functions
#include <allegro.h> // allegro!
#include <ppcol.h>   // pixel-perfect collision detection

/* legoids includes */
#include "structs.h"
#include "lists.h"
#include "functs.h"
#include "legdat.h"

/* defines */
#define VERSION "LEGOIDS v0.21a (9/23/98)"
#define MAX_FPS 60
#define PLAYER_ACCEL 0.05
#define PLAYER_MAX ftofix(3)
#define TURN_SPEED 4
#define PLAYER_MIN 0
#define BULLET_MAX 256 // 12
#define BULLET_SPEED 3 // 4
#define BULLET_LIFE 32 // 18

/* global variables */
PALETTE pal;
BITMAP *buffer = (BITMAP *) NULL, *all = (BITMAP *) NULL, *ship = (BITMAP *) NULL;
DATAFILE *legdat = (DATAFILE *) NULL;
SHIP player;
volatile GAME_STATUS game_status;
volatile int game_time=0,fps_time=0;

/* the main game timer */
void timer()
{
     game_time++;
     fps_time++;
     if(fps_time >= MAX_FPS)
     {
          fps_time=0;
          game_status.lastfps=game_status.frames;
          game_status.frames=0;
     }
}

END_OF_FUNCTION(timer);

/* the all-powerful, all-important main() ;)
   returns 0 on good exit, or different positive values corresponding to
   various errors:

     Exit code:     Error:
     0              none
     1              can't set gfx mode
     2              error accessing file
     3              error creating bitmap or allocating memory
*/
int main(int argc, char *argv[])
{
     BITMAP *bull,*screenshot;
     int i,k;
     char buf[256]; // screenshot filename buffer
     BULLETLIST *bulletlist = (BULLETLIST *) NULL;
     ASTEROIDLIST *asteroidlist = (ASTEROIDLIST *) NULL;
     BULLET *bullet = (BULLET *) NULL;
     ASTEROID *asteroid = (ASTEROID *) NULL, *asteroid2 = (ASTEROID *)NULL;

     printf("%s (FREEWARE, Ben Shadwick)\nThis program proudly uses:\n", VERSION);
     puts(allegro_id);

     i=init();
     if(i)
     {
          allegro_exit();
          printf("init() returned error %i, exiting...\n",i);
          exit(i);
     }

     LOCK_VARIABLE(game_time);
     LOCK_VARIABLE(fps_time);
     LOCK_VARIABLE(game_status);
     LOCK_FUNCTION(timer);

     /* this is simply for collision detection between bullets and asteroids */
     bull=create_bitmap(1,1);
     if(bull==(BITMAP *)NULL)
     {
          allegro_exit();
          printf("Error creating bullet bitmap\n");
          exit(3);
     }
     putpixel(bull, 0, 0, 15);

     set_palette(pal);
     clear(screen);
     text_mode(-1);

     bulletlist=newbulletlist((BULLETLIST *)NULL);
     if(!newbulletlist)
     {
          allegro_exit();
          printf("Error allocating memory for bullet list\n");
          exit(3);
     }

     asteroidlist=newasteroidlist((ASTEROIDLIST *)NULL);
     if(!newasteroidlist)
     {
          allegro_exit();
          printf("Error allocating memory for asteroid list\n");
          exit(3);
     }

     newgame(asteroidlist, bulletlist);

     install_int_ex(timer, BPS_TO_TIMER(MAX_FPS));
     game_time=0;
     game_status.counter=game_time;

     /* main loop */
     while(!game_status.gameover)
     {
          /* main game logic */
          while(game_status.counter < game_time)
          {
            // handle player input
            if(!game_status.waitplace)
            {
               if(key[KEY_SPACE])
               {
                    if(game_status.shotflag <= 0)
                    {
                         if(bulletlist->bullets < BULLET_MAX)
                         {
                              addbullet(bulletlist, player.xpos, player.ypos, player.xspeed-(BULLET_SPEED*fsin(player.angle)), player.yspeed+(BULLET_SPEED*fcos(player.angle)), BULLET_LIFE);
                         }
                         game_status.shotflag=15;
                    }
                    else
                         game_status.shotflag--;
               }
               else if(key[KEY_BACKSPACE])
               {
                    if(game_status.shotflag <= 0)
                    {
                         if(!bulletlist->bullets)
                         {
                              k = 256 / BULLET_MAX;
                              for(i=0;i<BULLET_MAX;i++)
                                   addbullet(bulletlist, player.xpos, player.ypos, player.xspeed-(BULLET_SPEED*fsin(itofix(i*k))), player.yspeed+(BULLET_SPEED*fcos(itofix(i*k))), BULLET_LIFE);
                         }
                         game_status.shotflag=15;
                    }
                    else
                         game_status.shotflag--;
               }
               else if(game_status.shotflag)
                    game_status.shotflag=0;

               if(key[KEY_LEFT])
               {
                    player.angle = (player.angle - ftofix(TURN_SPEED)) & ftofix(255);
                    clear(player.pic);
                    rotate_sprite(player.pic,ship,0,0,player.angle);
               }

               if(key[KEY_RIGHT])
               {
                    player.angle = (player.angle + ftofix(TURN_SPEED)) & ftofix(255);
                    clear(player.pic);
                    rotate_sprite(player.pic,ship,0,0,player.angle);
               }

               if(key[KEY_UP])
               {
                    player.xspeed-=PLAYER_ACCEL*fsin(player.angle);
                    player.yspeed+=PLAYER_ACCEL*fcos(player.angle);

                    if(player.xspeed < PLAYER_MIN && player.xspeed > -PLAYER_MIN)
                         player.xspeed = 0;
                    else if(player.xspeed > PLAYER_MAX)
                         player.xspeed = PLAYER_MAX;
                    else if(player.xspeed < -PLAYER_MAX)
                         player.xspeed = -PLAYER_MAX;

                    if(player.yspeed < PLAYER_MIN && player.yspeed > -PLAYER_MIN)
                         player.yspeed = 0;
                    else if(player.yspeed > PLAYER_MAX)
                         player.yspeed = PLAYER_MAX;
                    else if(player.yspeed < -PLAYER_MAX)
                         player.yspeed = -PLAYER_MAX;
               }
            }
               if(key[KEY_PAUSE])
               {
                    install_int(timer,0);

                    text_mode(5);
                    textout_centre(screen, font, "[PAUSED]", 159, 95, 12);
                    text_mode(-1);

                    while(key[KEY_PAUSE]);

                    install_int_ex(timer, BPS_TO_TIMER(MAX_FPS));
               }

               if(key[KEY_F])
               {
                    if(!game_status.showflag)
                    {
                         game_status.showfps=!game_status.showfps;
                         game_status.showflag=TRUE;
                    }
               }
               else if(game_status.showflag)
                    game_status.showflag=FALSE;

               if(key[KEY_S])
               {
                    if(!game_status.saveflag)
                    {
                         game_status.saveflag=TRUE;

                         get_palette(pal);
                         screenshot=create_sub_bitmap(screen,0,0,SCREEN_W,SCREEN_H);

                         i=0;
                         do // find the next usable screenshot filename
                         {
                              sprintf(buf,"leg%05d.pcx",i++);
                         }
                         while(file_exists(buf,63,NULL) && i<99999);

                         save_pcx(buf,screenshot,pal);
                    }
               }
               else if(game_status.saveflag)
                    game_status.saveflag=FALSE;

               if(key[KEY_ESC] && game_status.lives >= 0)
               {
                    game_status.lives=0;
                    killplayer(bulletlist);
               }


               // RENDERING CODE
               clear(buffer);

               // Generate new level if necessary
               if(!(asteroidlist->asteroids))
               {
                    game_status.level++;
                    i=newlevel(asteroidlist, 4 + (random() % game_status.level));
                    if(i==-1)
                    {
                         allegro_exit();
                         printf("newlevel(): (-1) Error creating new level\n");
                         exit(-1);
                    }
               }
               else
               {
                 // check if each asteroid has been hit by a bullet or the player

                 asteroid=asteroidlist->first;
                 while(asteroid!=(ASTEROID *)NULL)
                 {
                    if((game_status.waitplace == 1) && check_pp_collision(player.pic,asteroid->pic,fixtoi(player.xpos)-8,fixtoi(player.ypos)-8, fixtoi(asteroid->xpos-(asteroid->size * itofix(8))),fixtoi(asteroid->ypos-(asteroid->size * itofix(8))) ))
                         game_status.playerhit=TRUE;

                    // first, check to see if asteroid has hit player
                    if(!game_status.waitplace && check_pp_collision(player.pic,asteroid->pic,fixtoi(player.xpos)-8,fixtoi(player.ypos)-8, fixtoi(asteroid->xpos-(asteroid->size * itofix(8))),fixtoi(asteroid->ypos-(asteroid->size * itofix(8))) ))
                    {
                         if((asteroid->size) > 1)
                         {
                              for(i=0;i<(asteroid->size - 1);i++)
                              {
                                   asteroid2=addasteroid(asteroidlist,(asteroid->xpos)+itofix( (random() % 6) - 3 ),(asteroid->ypos)+itofix( (random() % 6) - 3 ), (asteroid->xspeed)*( (double) (random()%251)/(100) ), (asteroid->yspeed)*( (double) (random()%251)/(100) ), (asteroid->size) / 2);
                                   stretch_blit(all,asteroid2->pic,32,0,16,16,0,0,16*asteroid2->size,16*asteroid2->size);
                              }
                         }
                         game_status.score+=(asteroid->size)*25;
                         delasteroid(asteroidlist,asteroid);

                         killplayer(bulletlist);
                    }
                    else
                    {
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

                    asteroid->xpos = (asteroid->xpos + asteroid->xspeed);
                    asteroid->ypos = (asteroid->ypos + asteroid->yspeed);

                    if(asteroid->xpos < itofix(-16))
                         asteroid->xpos = itofix(336);
                    else if(asteroid->xpos > itofix(336))
                         asteroid->xpos = itofix(-16);

                    if(asteroid->ypos < itofix(-16))
                         asteroid->ypos = itofix(216);
                    else if(asteroid->ypos > itofix(216))
                         asteroid->ypos = itofix(-16);

                    draw_sprite(buffer,asteroid->pic,fixtoi(asteroid->xpos-(asteroid->size * itofix(8))), fixtoi(asteroid->ypos-(asteroid->size * itofix(8))) );

                    asteroid=asteroid->next;
                 }
               }

               // render asteroids
/*               asteroid=asteroidlist->first;
               while(asteroid)
               {
                    draw_sprite(buffer,asteroid->pic,fixtoi(asteroid->xpos-(asteroid->size * itofix(8))), fixtoi(asteroid->ypos-(asteroid->size * itofix(8))) );

                    asteroid->xpos = (asteroid->xpos + asteroid->xspeed);
                    asteroid->ypos = (asteroid->ypos + asteroid->yspeed);

                    if(asteroid->xpos < itofix(-16))
                         asteroid->xpos = itofix(336);
                    else if(asteroid->xpos > itofix(336))
                         asteroid->xpos = itofix(-16);

                    if(asteroid->ypos < itofix(-16))
                         asteroid->ypos = itofix(216);
                    else if(asteroid->ypos > itofix(216))
                         asteroid->ypos = itofix(-16);

                    asteroid=asteroid->next;
               }
*/

               // render bullets
               bullet=bulletlist->first;

               while(bullet)
               {
                    if(bullet && (bullet->time_to_live <= 0)) /* bullet's time has run out; kill it */
                    {
                         delbullet(bulletlist,bullet);
                    }
                    else
                    {
                         bullet->xpos+=bullet->xspeed;
                         bullet->ypos+=bullet->yspeed;

                         if(bullet->xpos < itofix(-16))
                              bullet->xpos = itofix(336);
                         else if(bullet->xpos > itofix(336))
                              bullet->xpos = itofix(-16);

                         if(bullet->ypos < itofix(-16))
                              bullet->ypos = itofix(216);
                         else if(bullet->ypos > itofix(216))
                              bullet->ypos = itofix(-16);

                         bullet->time_to_live--;
                         putpixel(buffer, fixtoi(bullet->xpos), fixtoi(bullet->ypos), 32-(bullet->time_to_live));

                    }                    
                    bullet=bullet->next;
               }

               // move and render player
               player.xpos+=player.xspeed;
               player.ypos+=player.yspeed;

               if(player.xpos < itofix(-16))
                    player.xpos = itofix(336);
               else if(player.xpos > itofix(336))
                    player.xpos = itofix(-16);

               if(player.ypos < itofix(-16))
                    player.ypos = itofix(216);
               else if(player.ypos > itofix(216))
                    player.ypos = itofix(-16);

               if(game_status.waitplace)
               {
                    if(game_status.lives >= 0)
                         textprintf_centre(buffer,font,160,100,(game_status.counter)%256,"READY!");
                    else
                         textprintf_centre(buffer,font,160,100,(game_status.counter)%256,"- G A M E   O V E R -");

                    if(!game_status.playerhit)
                         game_status.waitplace--;
                    else
                         game_status.playerhit=FALSE;
               }
               else if(game_status.lives < 0)
                    game_status.gameover=TRUE;
               else
                    draw_sprite(buffer,player.pic,fixtoi(player.xpos)-8,fixtoi(player.ypos)-8);

               game_status.counter++;

               textprintf(buffer,font,0,0,5,"lives:");
               textprintf(buffer,font,1,1,13,"lives:");
               for(i=0;(i<game_status.lives && i<40);i++)
                    rotate_scaled_sprite(buffer,ship,(i*8)-1,12,itofix(127),ftofix(.5));
               textprintf(buffer,font,200,0,5,"score:");
               textprintf(buffer,font,201,1,13,"score:");
               textprintf(buffer,font,200,12,5,"%i",game_status.score);
               textprintf(buffer,font,201,13,13,"%i",game_status.score);

               if(game_status.showfps)
                    textprintf(buffer, font, 0, 0, 14, "FPS: %i %i", game_status.lastfps,game_status.waitplace);

               game_status.drawflag=TRUE;
          }

          /* look, it's the drawing code! */
          if(game_status.drawflag)
          {
               game_status.frames++;

               blit(buffer,screen,0,0,0,0,320,200);

               game_status.drawflag=FALSE;
          }

     }

     exit(0);
}
