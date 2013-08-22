/**
 * @author wezside
 * @date 2013-05-30
 */

#include <iostream>
#include <stdlib.h>
#include "glutapp.hpp"
#include "sensor.hpp"
#include "signalhandler.hpp"
 
#ifdef WITH_SENSOR
	#include "glsensorviewer.hpp"
#endif

using namespace wezside;

int main(int argc, char **argv)
{
#ifdef WITH_SENSOR	
	// Set up new 3D Sensor
	Sensor sensor;
	int rc = sensor.init(argc, argv);
	if (rc != openni::STATUS_OK)
	{
		return EXIT_FAILURE;
	}

	// Set up new GL Object
	GLSensorViewer glo("Viewer", sensor.device, sensor.depth, sensor.color);
	glo.init();
#else
	GLObject glo;
#endif

	// Set up Glut Application 
	GlutApp app;
	app.v.push_back(&glo);
	app.init(argc, argv); 

	// Register signal handler to handle kill signal
	SignalHandler signalHandler;
	signalHandler.setupSignalHandlers();
	while (!signalHandler.gotExitSignal())
	{
		app.render();
	}	

	return EXIT_SUCCESS;
}
