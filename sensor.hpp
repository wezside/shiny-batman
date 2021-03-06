#ifndef SENSOR3D_H
#define SENSOR3D_H

#include <OpenNI.h>
#include <string>
#include "NiTE.h"

namespace wezside 
{
    class Sensor
    {
    private:
        const char* deviceURI;

    public:     
        Sensor() : deviceURI(openni::ANY_DEVICE) {};
        Sensor(const char* value) : deviceURI(value) {};
        ~Sensor();

        int init(int, char**);
        std::string getPixelFormatString(int);
        void printSensorInfo(openni::VideoStream& vs);
        void printAvailSensorInfo(openni::Device& device, openni::SensorType st);
        
        openni::Status rc;
        openni::Device device;
        openni::VideoStream depth;
        openni::VideoStream color;
    };
};


#endif // SENSOR3D_H