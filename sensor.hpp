#ifndef SENSOR3D_H
#define SENSOR3D_H

#include <OpenNI.h>

namespace wezside 
{
    class Sensor
    {
    private:
    	openni::Status rc;
    	openni::Device device;
        openni::VideoStream depth, color;
    	const char* deviceURI;

	public:    	
		Sensor() : deviceURI(openni::ANY_DEVICE){};
		~Sensor();

		int init(int argc, char** argv);
    };
};


#endif // SENSOR3D_H