#ifndef GLSENSOR_VIEWER_H
#define GLSENSOR_VIEWER_H


#include <OpenNI.h>
#include "globject.hpp"

#define TEXTURE_SIZE    512
#define DEFAULT_DISPLAY_MODE    DISPLAY_MODE_DEPTH
#define MIN_NUM_CHUNKS(data_size, chunk_size)   ((((data_size)-1) / (chunk_size) + 1))
#define MIN_CHUNKS_SIZE(data_size, chunk_size)  (MIN_NUM_CHUNKS(data_size, chunk_size) * (chunk_size))

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

            int m_width;
            int m_height;            
            unsigned int m_nTexMapX;
            unsigned int m_nTexMapY;            

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
            int init(void);
            
            DisplayModes m_eViewState;
            openni::RGB888Pixel* m_pTexMap;            
    };
};
#endif // GLSENSOR_VIEWER_H
