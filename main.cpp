/**
 * @author wezside
 * @date 2013-05-30
 */

#include <iostream>
#include <stdlib.h>
#include "glutapp.hpp"
#include "sensor.hpp"
#include "glsensorviewer.hpp"

using namespace wezside;

int main(int argc, char **argv)
{
    // Set up new 3D Sensor
    Sensor sensor;
    int rc = sensor.init(argc, argv);
    if (rc != openni::STATUS_OK)
    {
        return EXIT_FAILURE;
    }

    // Set up new GL Object - TODO: Needs to be abstract class
    GLSensorViewer glo("Viewer", sensor.device, sensor.depth, sensor.color);
    glo.init();

    // Set up our Glut Application 
    GlutApp app;
    app.glo = (GLObject*)&glo;
    app.init(argc, argv, "demo-rotate-shader.vert.glsl", "demo-rotate-shader.frag.glsl"); 
    app.render();

	return EXIT_SUCCESS;
}
