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
    sensor.init(argc, argv);

    // Set up new GL Object - TODO: Needs to be abstract class
    GLSensorViewer glo;
    // glShape.loadShader("", GLVertexShape::VERTEX);
    // glShape.loadShader("", GLVertexShape::FRAGMENT);

    /*  
    SampleViewer sampleViewer("Simple Viewer", device, depth, color);
    rc = sampleViewer.init(argc, argv);
    if (rc != openni::STATUS_OK)
    {
        openni::OpenNI::shutdown();
        return 3;
    }
    */

    // Set up our Glut Application 
    GlutApp app;
    app.glo = &glo;
    app.init(argc, argv);
    // app.init(argc, argv, &glshape, &sensor);
     
    
    app.render();

	exit( EXIT_SUCCESS );
}
