/**
 ** LEGOIDS - init.c
 **
 ** main program initialization routines
 **
 *************************************/

#include <stdio.h>
#include <stdlib.h>  // random numbers
#include <time.h>    // random number seeding
#include <allegro.h>

#include "globals.h"
#include "structs.h"
#include "legdat.h"

/* init() - main game initialization
   sets up some variables, initializes Allegro, enters graphics mode, etc.
*/
int init(void)
{
     // seed random number generator
     srandom(time(NULL));

     allegro_init();
     install_timer();
     install_keyboard();
     install_mouse();

     legdat = load_datafile("legoids.dat");
     if(legdat == (DATAFILE *) NULL)
     {
          allegro_exit();
          printf("Error loading LEGIODS.DAT");
          return(2);
     }

     /* load main tile map */
     all=(BITMAP *) legdat[ASTER1].dat;

     if( install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,(char *)NULL) )
     {
          allegro_exit();
          printf("Error installing sound/music handler! (Allegro error follows)\n");
          puts(allegro_error);
          return(1);
     }

     if(set_gfx_mode(GFX_AUTODETECT,320,200,0,0) < 0)
     {
          allegro_exit();
          printf("Error setting graphics mode! (Allegro error follows)\n");
          puts(allegro_error);
          return(1);
     }
     clear(screen);

     set_palette(legdat[gamepal].dat);
     get_palette(pal);

     font=(FONT *)legdat[robofnt1].dat;

     buffer=create_bitmap(320,200);
     if(buffer==(BITMAP *)NULL)
     {
          printf("Error creating buffer bitmap\n");
          return(3);
     }
     clear(buffer);

     ship=create_bitmap(16,16);
     if(ship==(BITMAP *)NULL)
     {
          allegro_exit();
          printf("Error creating ship bitmap\n");
          return(3);
     }
     blit(all,ship,0,0,0,0,16,16);

     /* initialize player's ship */
     player.xpos = itofix(159);
     player.ypos = itofix(99);
     player.xspeed = 0;
     player.yspeed = 0;
     player.angle = itofix(128);
     player.pic=create_bitmap(16,16);
     if(player.pic==(BITMAP *)NULL)
     {
          printf("Error creating player bitmap\n");
          return(3);
     }
     clear(player.pic);

     return(0);
}
