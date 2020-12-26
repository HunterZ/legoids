LEGOIDS v0.21a (9/23/1998) - an Asteroids arcade game clone by Ben Shadwick

What is Legoids?

Legoids is just a little Asteroids look-alike that I decided to throw together
as proof to myself that I can actually do it =) Right now I guess it's still
in the alpha stage, meaning that it isn't really a game yet - just a "work in
progress". It's really moving along now though; I think the hardest part is
over now (now I mostly just need to make things look and feel pretty =)

Running Legoids:

Just run LEGOIDS.EXE from a DOS prompt (works fine in a Win95 DOS box too)
Legoids uses the standard 320x200 256-color VGA mode, so it should work as
long as you have some sort of VGA card.

Compiling/building/tweaking Legoids:

If you are into C programming, I have included the source code for Legoids so
that you can see how it works. (if you have any suggestions or ideas, be sure
to let me know!) If you want to compile it, you will need DJGPP, Allegro, and
PPCol. (see http://www.delorie.com/djgpp/ and
http://www.talula.demon.co.uk/allegro/) I have included both a RHIDE project
file (outdated probably) and a makefile, so you can either load up the project
file in RHIDE and compile, or just type 'make' at a DOS prompt and hit enter.

Controls:

        up arrow - thrust
left/right arrow - turn counterclockwise/clockwise
        spacebar - fire a "bullet"
           pause - pause/unpause the game
          escape - end the game
    ctrl+alt+end - emergency exit (courtesy of Allegro)
         'f' key - toggle frames per second (FPS) display
         's' key - save 320x200x256 screenshot to leg#####.pcx, where ##### is
                   the next available number (starting with 00000)
             ??? - secret weapon (I'm not telling!)

What's new in this version:

[gameplay]
- nothing

[code]
- added structs.h to the ZIP file like I was supposed to in the first place!
- added George Foot's (much better) makefile

[to do]
- give the player an extra life every so many points
- add a title screen
- add a high score list
- add sound and music
- optimize, organize, & modularize!
- make and implement multiple rotating asteroid sprites =)

Files in this version of Legoids:

init.c         - misc. init. routines
lists.c        - linked-list routines
main.c         - main program loop
game.c         - game-related functions
functs.h       - misc. function definitions
globals.h      - global variable declarations
legdat.h       - Allegro datafile header for legoids.dat
lists.h        - linked-list function declaration header file
structs.h      - struct definition header file
makefile       - makefile
legoids.gpr    - RHIDE project file
legoids.exe    - precompiled binary (just run it to play Legoids!)
legoids.dat    - Allegro datafile
cwsdpmi.exe    - DPMI server for DJGPP (if you're not running Windoze)
cwsdpmi.doc    - CWSDPMI docs
readme.txt     - this file!

Contacting the author:

Legoids was made by me, Ben Shadwick. I can be reached by Internet email at
bshadwick@juno.com or on my BBS (Mars Base) whose number is (360)882-0773
(in Vancouver, WA, USA). I also have a web page at
http://marsbase.home.ml.org/ (or http://surf.to/marsbase if ml.org is down)

If I am online, you can try to reach me via ICQ #18537282 (HunterZ)

Special thanks to:

- DJ Delorie (DJGPP)
- Shawn Hargreaves (Allegro!)
- Tom Grandgent (Allegro mailing lists!)
- Ivan Baldo, Neil Chinnery, and Michael de la Pena (PPCol addon for Allegro)
- George Foot (Allegro Vivace tutorial)
- Kevin Dickerson and Joshua Haberman (Allegro FAQ and Programming Techniques)
- Joe Shadwick, Eugene Teo, Aaron Nonis, Tom Brizendine, Tony Greenwood,
  Grzegorz Adam Hankiewicz, and George Foot (testing & feedback)
- The Allegro Mailing Lists crew (ideas & help)
