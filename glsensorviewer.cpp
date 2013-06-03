#include "glsensorviewer.hpp"

wezside::GLSensorViewer::~GLSensorViewer()
{
	delete[] m_streams;
	delete[] m_pTexMap;
}
int wezside::GLSensorViewer::init()
{
	openni::VideoMode depthVideoMode;
	openni::VideoMode colorVideoMode;

	if (m_depthStream.isValid() && m_colorStream.isValid())
	{
		depthVideoMode = m_depthStream.getVideoMode();
		colorVideoMode = m_colorStream.getVideoMode();

		int depthWidth = depthVideoMode.getResolutionX();
		int depthHeight = depthVideoMode.getResolutionY();
		int colorWidth = colorVideoMode.getResolutionX();
		int colorHeight = colorVideoMode.getResolutionY();

		if (depthWidth == colorWidth &&
			depthHeight == colorHeight)
		{
			m_width = depthWidth;
			m_height = depthHeight;
		}
		else
		{
			printf("Error - expect color and depth to be in same resolution: D: %dx%d, C: %dx%d\n",
				depthWidth, depthHeight,
				colorWidth, colorHeight);
			return openni::STATUS_ERROR;
		}
	}
	else if (m_depthStream.isValid())
	{
		depthVideoMode = m_depthStream.getVideoMode();
		m_width = depthVideoMode.getResolutionX();
		m_height = depthVideoMode.getResolutionY();
	}
	else if (m_colorStream.isValid())
	{
		colorVideoMode = m_colorStream.getVideoMode();
		m_width = colorVideoMode.getResolutionX();
		m_height = colorVideoMode.getResolutionY();
	}
	else
	{
		printf("Error - expects at least one of the streams to be valid...\n");
		return openni::STATUS_ERROR;
	}

	m_streams = new openni::VideoStream*[2];
	m_streams[0] = &m_depthStream;
	m_streams[1] = &m_colorStream;

	// Texture map init
	m_nTexMapX = MIN_CHUNKS_SIZE(m_width, TEXTURE_SIZE);
	m_nTexMapY = MIN_CHUNKS_SIZE(m_height, TEXTURE_SIZE);
	m_pTexMap = new openni::RGB888Pixel[m_nTexMapX * m_nTexMapY];
	return openni::STATUS_OK;
}

void wezside::GLSensorViewer::createVBO()
{
	std::cout << "GLSensorViewer::createVBO" << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUtil.exitOnGLError("ERROR: Could not set OpenGL depth testing options");
     
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    glUtil.exitOnGLError("ERROR: Could not set OpenGL culling options");
	
    modelMatrixUniformLocation = glGetUniformLocation(programID, "modelMatrix");
    viewMatrixUniformLocation = glGetUniformLocation(programID, "viewMatrix");
    projectionMatrixUniformLocation = glGetUniformLocation(programID, "projectionMatrix");	
 	/*
 	// For use with Perspective projection
 	Vertex vertices[] =
    {
        {{ -0.8f,  0.8f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
        {{  0.8f,  0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
        {{ -0.8f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }},
        {{  0.8f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}
    };*/

    // For use with Orthogonal projection
    float wwww = (float)screenWidth;
    float hhhh = (float)screenHeight;
	Vertex vertices[] =
    {
        {{  0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
        {{  wwww, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
        {{  0.0f, hhhh, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }},
        {{  wwww, hhhh, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}
    };        

    const size_t vertexSize = sizeof(vertices[0]);
    const size_t rgbOffset = sizeof(vertices[0].XYZW);    

	// Create VAO that describes how the vertex attributes are stored in a Vertex Buffer Object 
	// The VAO is not the actual object storing the vertex data but the descriptor
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // Generate a valid ID used for storage and bind to this new ID so data 
    // can be copied to GPU's memory
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);

    // Reserve appropriate data storage on the GPU for our vertices
    // Passsing NULL as "data" param will indicate that the reserved data store 
    // is uninitialized
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Describe vertex attributes stored in GPU's memory
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertexSize, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*) rgbOffset);

	// Enable the vertex attributes 
	// 0-Vertices 
	// 1-Colour
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
 
 	glUtil.exitOnGLError("ERROR: Could not create a VBO");
}

void wezside::GLSensorViewer::resize(int w, int h)
{
    screenWidth = w;
    screenHeight = h;
    printf("GLObject::resize(%d,%d)\n", w, h);

    // Orthogonal Projeciton
    projectionMatrix =
        glUtil.createOrthogonalMatrix(
           0.001, 100.0, 0.0, (float)w, (float)h, 0.0
        );

    glUtil.translateMatrix(&viewMatrix, 0, 0, -0.01);

    glUseProgram(programID);
    glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, projectionMatrix.m);
    glUseProgram(0);    
}

/**
 * This is the draw method where things happen on screen. The first part updates
 * values and read values from the sensor. The second tells OpenGL to draw the 
 * Vertex Buffer Objects to screen using the shader programs.
 */
void wezside::GLSensorViewer::display()
{
	int changedIndex;
	openni::Status rc = openni::OpenNI::waitForAnyStream(m_streams, 2, &changedIndex);
	if (rc != openni::STATUS_OK)
	{
		printf("Wait failed\n");
		return;
	}

	switch (changedIndex)
	{
		case 0:
			m_depthStream.readFrame(&m_depthFrame); break;		
		case 1:
			m_colorStream.readFrame(&m_colorFrame); break;
		default:
			printf("Error in wait\n");
	}	

	// Read depth frame
	if (m_depthFrame.isValid()) simpleRead(m_depthFrame);

	// Draw OpenGL
	angle += 1.0f;
	angle = fmod(angle, 360.0);

	modelMatrix = GLUtils::IDENTITY_MATRIX;
	// glUtil.rotateAboutY(&modelMatrix, glUtil.degreesToRadians(angle));
	// glUtil.rotateAboutX(&modelMatrix, glUtil.degreesToRadians(angle));

	// Make the shader program active
	glUseProgram(programID);
	glUtil.exitOnGLError("ERROR: Could not use the shader program");	

	// Update the shader Uniform variables
	glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, modelMatrix.m);
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, viewMatrix.m);
	glUtil.exitOnGLError("ERROR: Could not set the shader uniforms");

	glBindVertexArray(vaoID);
	glUtil.exitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glUtil.exitOnGLError("ERROR: Could not draw");

	// Cleanup
	glBindVertexArray(0);
	glUseProgram(0);
}

/**
 * Grab the pixel values from the color frame buffer and add it to the 
 * texture buffer. 
 */
void wezside::GLSensorViewer::drawColorFrame(openni::VideoFrameRef& frame)
{
	memset(m_pTexMap, 0, m_nTexMapX*m_nTexMapY*sizeof(openni::RGB888Pixel));

	// check if we need to draw image frame to texture
	if ((m_eViewState == DISPLAY_MODE_OVERLAY ||
		m_eViewState == DISPLAY_MODE_IMAGE) && frame.isValid())
	{
		const openni::RGB888Pixel* pImageRow = (const openni::RGB888Pixel*)frame.getData();
		openni::RGB888Pixel* pTexRow = m_pTexMap + frame.getCropOriginY() * m_nTexMapX;
		int rowSize = frame.getStrideInBytes() / sizeof(openni::RGB888Pixel);

		for (int y = 0; y < frame.getHeight(); ++y)
		{
			const openni::RGB888Pixel* pImage = pImageRow;
			openni::RGB888Pixel* pTex = pTexRow + frame.getCropOriginX();

			for (int x = 0; x < frame.getWidth(); ++x, ++pImage, ++pTex)
			{
				*pTex = *pImage;
			}

			pImageRow += rowSize;
			pTexRow += m_nTexMapX;
		}
	}	

	// Set up texture
	
}

/** 
 * This method will simply return the center point depth value.
 */
void wezside::GLSensorViewer::simpleRead(openni::VideoFrameRef& frame)
{
	if (frame.getVideoMode().getPixelFormat() != openni::PIXEL_FORMAT_DEPTH_1_MM && 
		frame.getVideoMode().getPixelFormat() != openni::PIXEL_FORMAT_DEPTH_100_UM)
	{
		printf("Unexpected frame format\n");
	}
	openni::DepthPixel* pDepth = (openni::DepthPixel*)frame.getData();
	int middleIndex = (frame.getHeight()+1)*frame.getWidth()/2;
	// printf("[%08llu] %8d\n", (long long)frame.getTimestamp(), pDepth[middleIndex]);	
}