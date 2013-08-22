# Makefile for Glut App Wrapper class
# Requires NI shared lib file in same folder otherwise update the '-rpath' to 
# point to the location of the installed lib. Also update the CPPFLAGS to locate 
# the folder containing the OpenNI.h header file.
CC = g++
CFLAGS  = -Wall
CPPFLAGS = -ggdb -I./src  -I../3rdparty/Include
LDLIBS = -Wl,-rpath ../3rdparty -L../3rdparty -L/usr/local/lib -lserial -lOpenNI2 -lNiTE2  -lboost_thread -lcurl -ltbb  -lGLEW -lglut -lGL -lGLU
LDLIBS += -lopencv_core
LDLIBS += -lopencv_video 
LDLIBS += -lopencv_highgui
LDLIBS += -lopencv_calib3d 
LDLIBS += -lopencv_contrib 
LDLIBS += -lopencv_core 
LDLIBS += -lopencv_features2d 
LDLIBS += -lopencv_flann 
LDLIBS += -lopencv_gpu
LDLIBS += -lopencv_highgui
LDLIBS += -lopencv_imgproc
LDLIBS += -lopencv_legacy
LDLIBS += -lopencv_ml 
LDLIBS += -lopencv_nonfree 
LDLIBS += -lopencv_objdetect 
LDLIBS += -lopencv_photo 
LDLIBS += -lopencv_stitching 
LDLIBS += -lopencv_superres 
LDLIBS += -lopencv_ts 
LDLIBS += -lopencv_video 
LDLIBS += -lopencv_videostab 

default: app

app: glutapp.o globject.o sensor.o glsensorviewer.o glutils.o signalhandler.o main.o 
	$(CC) $(CPPFLAGS) $(CFLAGS) -o app glutapp.o globject.o sensor.o glsensorviewer.o glutils.o signalhandler.o main.o $(LDLIBS) 

clean: 
	$(RM) count *.o *~
