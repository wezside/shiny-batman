#
# Makefile for Glut App Wrapper class
# Requires NI shared lib file in same folder otherwise update the '-rpath' to 
# point to the location of the installed lib. Also update the CPPFLAGS to locate 
# the folder containing the OpenNI.h header file.
CC = g++
CFLAGS  = -Wall 
CPPFLAGS = -I/home/wezside/Packages/OpenNI2/Include/ -ggdb
LDLIBS=-L./ -Wl,-rpath,./ -lGLEW -lglut -lGL -lGLU -lOpenNI2 -lFreenectDriver

default: app

app: glutapp.o globject.o sensor.o glsensorviewer.o glutils.o main.o 
	$(CC) $(LDLIBS) $(CPPFLAGS) $(CFLAGS) -o app glutapp.o globject.o sensor.o glsensorviewer.o glutils.o main.o 

clean: 
	$(RM) count *.o *~
