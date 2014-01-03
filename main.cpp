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

#ifdef WITH_IMAGE
	#include "glimage.hpp"
#endif

#ifdef WITH_SENSOR
	#include "glsensorviewer.hpp"
#endif

using namespace wezside;

int main(int argc, char **argv)
{
	// Set up Glut Application 
	GlutApp app(640, 480);
	app.init(argc, argv); 
	app.loadShader("shader.vert.glsl", GL_VERTEX_SHADER);
	app.loadShader("shader.frag.glsl", GL_FRAGMENT_SHADER);
	GLuint pid = app.glslProgram();
	app.render();
	
#ifdef WITH_SENSOR	

	// Set up new 3D Sensor
	Sensor sensor;
	int rc = sensor.init(argc, argv);
	if (rc != openni::STATUS_OK) return EXIT_FAILURE;

	// Set up new GL Object
	GLSensorViewer glo("Viewer", sensor.device, sensor.depth, sensor.color);

	printf("[%s] %s %s\n", "main", "Depth Stream is valid", sensor.depth.isValid() == 1 ? "YES" : "NO");
	printf("[%s] %s %s\n", "main", "Colour Stream is valid", sensor.color.isValid() == 1 ? "YES" : "NO");

	if (glo.init() != openni::STATUS_OK) return EXIT_FAILURE;
	app.v.push_back(&glo);
#endif

#ifdef WITH_IMAGE

	GLImage glo();
	app.v.push_back(&glo);
#endif

#ifdef WITH_PARTICLE

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
