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
    // glo.loadShader();
    glo.init();
    // glo.loadShader("demo-shader.vert", GL_VERTEX_SHADER); // custom shader
    // glo.loadShader("demo-shader.frag", GL_FRAGMENT_SHADER); // custom shader

    // Set up our Glut Application 
    GlutApp app;
    app.glo = (GLObject*)&glo;
    std::cout << app.glo->getName() << std::endl;
    app.init(argc, argv); 
    app.render();

	return EXIT_SUCCESS;
}
