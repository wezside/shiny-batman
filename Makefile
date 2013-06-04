#
# Makefile for Glut App Wrapper class
# Requires NI shared lib file in same folder otherwise update the '-rpath' to 
# point to the location of the installed lib
#

CC = g++
CFLAGS  = -Wall 
CPPFLAGS = -I/home/wezside/Downloads/OpenNI-2.1.0/Include/ -ggdb
LDLIBS=-L./ -Wl,-rpath,./ -lGLEW -lglut -lGL -lGLU -lOpenNI2 -lFreenectDriver

default: app

app: glutapp.o globject.o sensor.o glsensorviewer.o glutils.o main.o 
	$(CC) $(LDLIBS) $(CPPFLAGS) $(CFLAGS) -o app glutapp.o globject.o sensor.o glsensorviewer.o glutils.o main.o 

clean: 
	$(RM) count *.o *~
