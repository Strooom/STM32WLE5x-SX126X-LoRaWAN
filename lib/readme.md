This directory contains all source code which is common to all environments : 
* windows desktop
* STM32WLE5 and MuMo PCB

files are using lowercase only, as this the most portable solution towards Windows/MacOS/Linux

example :
* lib.h
* lib.cpp

then in libdesktop and libtarget there is a 
* lib2.cpp
containing the HW specific variants of member functions