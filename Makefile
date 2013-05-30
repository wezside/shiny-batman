# Makefile for Glut App Wrapper class

CC = g++
CFLAGS  = -g -Wall -ggdb 
CPPFLAGS = -I/home/wezside/Downloads/OpenNI-2.1.0/Include/
LDLIBS=-L./ -Wl,-rpath,./ -lGLEW -lglut -lGL -lGLU -lOpenNI2

default: app

app: glutapp.o globject.o sensor.o main.o 
	$(CC) $(LDLIBS) $(CPPFLAGS) $(CFLAGS) -o app glutapp.o globject.o sensor.o main.o 

clean: 
	$(RM) count *.o *~