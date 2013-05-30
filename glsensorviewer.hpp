#ifndef GLSENSOR_VIEWER_H
#define GLSENSOR_VIEWER_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "globject.hpp"

#define DEFAULT_DISPLAY_MODE    DISPLAY_MODE_DEPTH

namespace wezside
{
    enum DisplayModes
    {
        DISPLAY_MODE_OVERLAY,
        DISPLAY_MODE_DEPTH,
        DISPLAY_MODE_IMAGE
    };

    class GLSensorViewer : public GLObject
    {
        private:
            openni::VideoFrameRef m_depthFrame;
            openni::VideoFrameRef m_colorFrame;

            openni::Device&       m_device;
            openni::VideoStream&  m_depthStream;
            openni::VideoStream&  m_colorStream;
            openni::VideoStream** m_streams;
            

        public:
            GLSensorViewer(std::string value,
                           openni::Device& device, 
                           openni::VideoStream& depth, 
                           openni::VideoStream& color ) : GLObject(value),
                                                          m_device(device), 
                                                          m_depthStream(depth), 
                                                          m_colorStream(color), 
                                                          m_streams(NULL), 
                                                          m_eViewState(DEFAULT_DISPLAY_MODE), 
                                                          m_pTexMap(NULL)
                            {};

            DisplayModes m_eViewState;
            openni::RGB888Pixel* m_pTexMap;            
    };
};
#endif // GLSENSOR_VIEWER_H
