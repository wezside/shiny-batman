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
	// GLParticleSystem glo;
#endif

	// Set up Glut Application 
	GlutApp app;
	app.init(argc, argv); 
	app.loadShader("strange.vert.glsl", GL_VERTEX_SHADER);
    app.loadShader("particle.frag.glsl", GL_FRAGMENT_SHADER);
	GLuint pid = app.glslProgram();
	app.v.push_back(new GLParticleSystem(pid));

	// Navier Stokes
	// get_from_UI ( dens_prev, u_prev, v_prev );
	// vel_step ( N, u, v, u_prev, v_prev, visc, dt );
	// dens_step ( N, dens, dens_prev, u, v, diff, dt );
	// draw_dens ( N, dens );

	// Register signal handler to handle kill signal
	SignalHandler signalHandler;
	signalHandler.setupSignalHandlers();
	while (!signalHandler.gotExitSignal())
	{
		app.render();
	}	

	return EXIT_SUCCESS;
}
