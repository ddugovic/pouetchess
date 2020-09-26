What is pouetChess ?
-------------------- 

pouetChess is a graphic chess game created for Linux and
ported to Windows (thanks to SDL and openGL)

It uses FAILE [http://faile.sourceforge.net] as an embedded engine 
for AI (up to 2250 ELO, even more, depending on your computer)



Dependencies :
-------------------- 

SDL

SDL-image

OpenGL (ie: latest drivers from your graphic card manufacturer)



In-game controls :
-------------------- 

Left mouse click : Select a case on the chessboard

Right mouse click : move the camera around the chessboard

Escape : return to main menu


Configuration file:
-------------------- 

Note : you should not have to modify the configuration file manually
since you have an in-game option menu to do that !

Linux/*BSD : See `.pouetChess` in the user's home directory

Windows : `Config.ini` in the same directory as the executable


How to Compile pouetChess:
-------------------- 

pouetChess uses the Scons build system. So if you want to compile from
source you will need it.

To build pouetChess you have to configure first : `pip install semver && scons configure`

To know all the options, you can do `scons --help`

Once configure is done, you can run `scons` to build pouetChess.

Then you can install pouetChess by `scons install`

IMPORTANT: if you want to run pouetChess without installing it,
you will have to add `noinstall=yes` to the configure option !
(eg: `scons configure noinstall=yes`)

Then you'll be able to launch pouetChess directly from the bin folder.

NOTE: In the 0.2.0 version, it seems that using `-O2` option with 
gcc 4.0.2 create a broken executable (because AI segFault when thinking)

This doesn't seem to appear when compiling with `-O1`.
So default optimization level is set to 1.

If you have an other version of GCC and want to see if this
bug occurs with `-O2` you can change optimization level 
to 2 with `scons configure optimize=2`

And If you don't want to optimize at all:
`scons configure optimize=no`

