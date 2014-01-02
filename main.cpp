/**
 * @author wezside
 * @date 2013-05-30
 */

#include <iostream>
#include <stdlib.h>
#include "glutapp.hpp"
#include "sensor.hpp"
#include "signalhandler.hpp"
#include "glparticlesystem.hpp"
 
#ifdef WITH_SENSOR
	#include "glsensorviewer.hpp"
#endif

using namespace wezside;

int main(int argc, char **argv)
{
	// Set up Glut Application 
	GlutApp app;
	app.init(argc, argv); 
	app.loadShader("particle.vert.glsl", GL_VERTEX_SHADER);
	app.loadShader("particle.frag.glsl", GL_FRAGMENT_SHADER);

#ifdef WITH_SENSOR	
	// Set up new 3D Sensor
	Sensor sensor;
	int rc = sensor.init(argc, argv);
	if (rc != openni::STATUS_OK) return EXIT_FAILURE;

	// Set up new GL Object
	GLSensorViewer glo("Viewer", sensor.device, sensor.depth, sensor.color);
	glo.init();
	GLuint pid = app.glslProgram();
	app.v.push_back(&glo);
#else
	// GLParticleSystem glo;
	GLuint pid = app.glslProgram();
	app.v.push_back(new GLParticleSystem(pid));
#endif


	// Register signal handler to handle kill signal
	SignalHandler signalHandler;
	signalHandler.setupSignalHandlers();
	while (!signalHandler.gotExitSignal())
	{
		app.render();
	}	

	return EXIT_SUCCESS;
}
