/**
 * @author wezside
 * @date 2013-05-30
 */

#include <iostream>
#include <stdlib.h>
#include "glutapp.h"
#include "globject.h"
#include "sensor3d.h"

int main(int argc, char **argv)
{
    // Set up new 3D Sensor
    wezside::Sensor3D s3d;
    s3d.init(argc, argv);

    // Set up new GL Object 
    wezside::GLObject glShape;
    glShape.loadShader("", GLVertexShape::VERTEX);
    glShape.loadShader("", GLVertexShape::FRAGMENT);

    // Set up our Glut Application 
    GlutApp app;
    app.init(argc, argv, &glshape, &s3d);
    app.render();

	exit( EXIT_SUCCESS );
}
