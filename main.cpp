/**
 * @author wezside
 * @date 2013-05-30
 */

#include <iostream>
#include <stdlib.h>
#include "glutapp.hpp"
#include "globject.hpp"
#include "sensor.hpp"

using namespace wezside;

int main(int argc, char **argv)
{
    // Set up new 3D Sensor
    Sensor sensor;
    sensor.init(argc, argv);

    // Set up new GL Object 
    GLObject glo;
    // glShape.loadShader("", GLVertexShape::VERTEX);
    // glShape.loadShader("", GLVertexShape::FRAGMENT);

    // Set up our Glut Application 
    GlutApp app;
    app.glo = &glo;
    app.init(argc, argv);
    // app.init(argc, argv, &glshape, &sensor);
    app.render();

	exit( EXIT_SUCCESS );
}
