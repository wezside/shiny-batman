#include "sensor.hpp"


wezside::Sensor::~Sensor() 
{
	openni::OpenNI::shutdown();
}

int wezside::Sensor::init(int argc, char** argv)
{
	openni::Status rc = openni::STATUS_OK;
	if (argc > 1)
	{
		deviceURI = argv[1];
	}

	rc = openni::OpenNI::initialize();
	printf("After initialization:\n%s\n", openni::OpenNI::getExtendedError());

	rc = device.open(deviceURI);
	if (rc != openni::STATUS_OK)
	{
		printf("Device open failed:\n%s\n", openni::OpenNI::getExtendedError());
		openni::OpenNI::shutdown();
		return 1;
	}

	rc = depth.create(device, openni::SENSOR_DEPTH);
	if (rc == openni::STATUS_OK)
	{
		rc = depth.start();
		if (rc != openni::STATUS_OK)
		{
			printf("Couldn't start depth stream:\n%s\n", openni::OpenNI::getExtendedError());
			depth.destroy();
		}
	}
	else
	{
		printf("Couldn't find depth stream:\n%s\n", openni::OpenNI::getExtendedError());
	}

	rc = color.create(device, openni::SENSOR_COLOR);
	if (rc == openni::STATUS_OK)
	{
		rc = color.start();
		if (rc != openni::STATUS_OK)
		{
			printf("Couldn't start color stream:\n%s\n", openni::OpenNI::getExtendedError());
			color.destroy();
		}
	}
	else
	{
		printf("Couldn't find color stream:\n%s\n", openni::OpenNI::getExtendedError());
	}

	if (!depth.isValid() || !color.isValid())
	{
		printf("No valid streams. Exiting\n");
		openni::OpenNI::shutdown();
		return 2;
	}

	

	// Get current VideoMode
	openni::VideoMode currentVM = color.getVideoMode();
	printf("%s\n", "\n=================\nCurrent VideoMode\n=================");
	printf("FPS %d\n", currentVM.getFps());
	printf("PixelFormat (%d) %s\n", currentVM.getPixelFormat(),
								  	getPixelFormatString(
								  		currentVM.getPixelFormat()).c_str()
								  	);
	printf("Resolution X: %d\n", currentVM.getResolutionX());
	printf("Resolution y: %d\n\n", currentVM.getResolutionY());	
	return rc;
}

std::string wezside::Sensor::getPixelFormatString(int format)
{
	std::string result;
	switch(format)
	{
		case openni::PIXEL_FORMAT_DEPTH_1_MM: 	result = "PIXEL_FORMAT_DEPTH_1_MM"; break;
		case openni::PIXEL_FORMAT_DEPTH_100_UM: result = "PIXEL_FORMAT_DEPTH_100_UM"; break;
		case openni::PIXEL_FORMAT_SHIFT_9_2: 	result = "PIXEL_FORMAT_SHIFT_9_2"; break;
		case openni::PIXEL_FORMAT_SHIFT_9_3: 	result = "PIXEL_FORMAT_SHIFT_9_3"; break;
		case openni::PIXEL_FORMAT_RGB888: 		result = "PIXEL_FORMAT_RGB888"; break;
		case openni::PIXEL_FORMAT_YUV422: 		result = "PIXEL_FORMAT_YUV422"; break;
		case openni::PIXEL_FORMAT_GRAY8: 		result = "PIXEL_FORMAT_GRAY8"; break;
		case openni::PIXEL_FORMAT_GRAY16: 		result = "PIXEL_FORMAT_GRAY16"; break;
		case openni::PIXEL_FORMAT_JPEG: 		result = "PIXEL_FORMAT_JPEG"; break;
		default: result = "Unrecognized pixel format"; break;
	}	
	return result;
}

void wezside::Sensor::printAvailSensorInfo(openni::Device& device, openni::SensorType st)
{
	printf("%s\n", "Sensor Info\n============\n");
	const openni::SensorInfo* sensorInfo = device.getSensorInfo(st);
	const openni::Array<openni::VideoMode>& videoModes = sensorInfo->getSupportedVideoModes();
	for (int i = 0; i < videoModes.getSize(); ++i)
	{
		const openni::VideoMode vm(videoModes[i]);
		printf("FPS %d\n", vm.getFps());
		printf("PixelFormat (%d) %s", vm.getPixelFormat(),
									  getPixelFormatString(
									  	vm.getPixelFormat()).c_str()
									  );
		printf("Resolution X: %d\n", vm.getResolutionX());
		printf("Resolution y: %d\n\n", vm.getResolutionY());
	}	
}