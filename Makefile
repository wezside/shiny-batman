# Makefile for Glut App Wrapper class

CC = g++
CFLAGS  = -g -Wall -ggdb 
LDLIBS=-lGLEW -lglut -lGL -lGLU

default: app

app: glutapp.o globject.o sensor3d.o main.o 
	$(CC) $(LDLIBS) $(CFLAGS) -o glutapp.o globject.o sensor3d.o main.o 

clean: 
	$(RM) count *.o *~