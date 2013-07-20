#include "glvideo.hpp"

void wezside::GLVideo::processFrame()
{
	try
	{
		cv::VideoCapture m_video;
		bool ret = m_video.open(m_name);
	    if(!m_video.isOpened() || ret == 0)
	    {
	    	printf("%s %s\n", "Error: Failed to open video file", m_name.c_str());
	    	thr->interrupt();
	    } 

	    m_video_fps = (unsigned int)m_video.get(CV_CAP_PROP_FPS);
		double dframes = m_video.get(CV_CAP_PROP_FRAME_COUNT);
		double dcurrent = m_video.get(CV_CAP_PROP_POS_FRAMES);

	    printf("[%s] %s [%d]\n", m_name.c_str(), "FPS", m_video_fps);
	    printf("[%s] %s [%d]\n", m_name.c_str(), "FRAME COUNT", (int)dframes);
	    printf("[%s] %s [%d]\n", m_name.c_str(), "CURRENT FRAME", (int)dcurrent);

	    if (dframes == 0)
	    {
	    	m_video.release();
	    	printf("[%s] %s\n", m_name.c_str(), "[Error] Video file incomplete.");
	    	thr->interrupt();
	    }
	 	for(;;)
	    {
	        try
	        {
	            boost::this_thread::interruption_point();      
	            boost::mutex::scoped_lock lock(guard);

	            cv::Mat temp(m_nOriginalTexMapX, m_nOriginalTexMapY, CV_8UC3);
	            m_video >> temp;
	            
	            // For playback we skip the first frame as we already decoded
	            if (m_frames.size() == 1 && 
	                m_firstFrameLoaded == 1)
	            {
	                m_video >> temp;
	            }
	            cv::resize(temp, temp, cv::Size(m_nTexMapX, m_nTexMapY), 0, 0, cv::INTER_AREA);
	            m_frames.push_back(temp.clone());

	            if (m_frames.size() == 1) m_firstFrameLoaded = 1;
	            if (m_frames.size() == 1 && m_autoplay == 0)
	            {
					// if (m_video.isOpened()) m_video.release();
					m_video.release();
					thr->interrupt();
					printf("[%s] Frames decoded %d::%d\n", m_name.c_str(), m_frames.size(), (int)dframes);
	            }

	            if ((int)dframes == m_frames.size())
	            {
	                // Join - wait when thread actually exits
	                m_playing = 1;
	                m_loaded = 1;
					if (m_video.isOpened()) m_video.release();
					thr->interrupt();
					printf("[%s] Frames decoded %d::%d\n", m_name.c_str(), m_frames.size(), (int)dframes);
	            } 
	            boost::this_thread::interruption_point();		
	        }
	        catch(boost::thread_interrupted&)
	        {
				printf("[%s] Thread stopped. [%p]\n", m_name.c_str(), thr);
	            printf("[%s] Took %f seconds\n", m_name.c_str(), (float)(clock()-m_tStart)/CLOCKS_PER_SEC);
	            return;
	        }
	    }	
	}
	catch( cv::Exception& e )
	{
		const char* err_msg = e.what();
		std::cout << "exception caught: " << err_msg << std::endl;
		return;
	}	
}
int wezside::GLVideo::start()
{
    printf("[%s] %s\n", m_name.c_str(), "wezside::GLVideo::start()");
    m_tStart = clock();
    thr = new boost::thread(&wezside::GLVideo::processFrame, this);
	thr->join();
	if (thr != NULL)
	{
		delete thr;
		thr = NULL;    
	}
	m_tStart = 0;
	printf("[%s] %s\n", m_name.c_str(), "Thread destroyed.");
    return EXIT_SUCCESS;
}
void wezside::GLVideo::createVBO()
{
    printf("[%s] %s [%d]\n", m_name.c_str(), "GLVideo::createVBO", programID);
/*	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glUtil.exitOnGLError("ERROR: Could not set OpenGL depth testing options");
	 */
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glUtil.exitOnGLError("ERROR: Could not set OpenGL culling options");
    glUseProgram(programID);

    texCoordLoc = glGetUniformLocation(programID, "in_TexCoord");
    samplerLoc = glGetUniformLocation(programID, "s_texture");  
    modelMatrixUniformLocation = glGetUniformLocation(programID, "modelMatrix");
    viewMatrixUniformLocation = glGetUniformLocation(programID, "viewMatrix");
    projectionMatrixUniformLocation = glGetUniformLocation(programID, "projectionMatrix");  
    
    float txhh = 0.0f;
	if (m_bAutosize == 1)
	{
		m_width = screenWidth / m_cols;
		printf("[%s] Screen: %dx%d\n", m_name.c_str(), screenWidth, screenHeight);
		int diff = m_width * m_rows - screenHeight;
		if (diff > 0)
		{
			// Crop
			m_fCropFill = ((int)diff / m_rows) / 2.0f;
			m_height = m_width - m_fCropFill * 2.0f;
			txhh = m_height / m_width + m_fCropFill / m_width;
			printf("[%s] Diff %d\n", m_name.c_str(), diff);
			printf("[%s] ------ m_fCropFill %f | texture %f\n", m_name.c_str(), m_fCropFill, m_fCropFill / m_width);
			m_nOffsetX = (m_gridpos % m_cols) * m_width;
			m_nOffsetY = (m_gridpos / m_cols) * m_height;			
			printf("[%s] ------ txhh %f | %f == %f\n", m_name.c_str(), txhh, (1.0f - txhh), m_fCropFill / m_width);
			// txhh = 1.0f;
		}
		else if (diff < 0)
		{
			// Fill
			printf("[%s] Diff %d\n", m_name.c_str(), diff);
			m_fCropFill = ((int)(diff * -1.0f) / m_rows) / 2.0f;
			printf("[%s] ++++++ m_fCropFill %f\n", m_name.c_str(), m_fCropFill);
			m_height = m_width + m_fCropFill * 2.0f;
			txhh = m_width / m_height;
			printf("[%s] ++++++ txhh %f | %f == %f\n", m_name.c_str(), txhh, (1.0f - txhh)/2.0f, m_fCropFill / m_width);
			m_nOffsetX = (m_gridpos % m_cols) * m_width;
			m_nOffsetY = (m_gridpos / m_cols) * (m_height + m_fCropFill);			
		}
		else if (diff == 0)
		{
			m_height = m_width;	
			txhh = 1.0f;
			m_nOffsetX = (m_gridpos % m_cols) * m_width;
			m_nOffsetY = (m_gridpos / m_cols) * (m_height + m_fCropFill);		
			printf("[%s] ====== m_fCropFill %f\n", m_name.c_str(), 0.0f);
		} 
		printf("[%s] New Size %fx%f\n", m_name.c_str(), m_width, m_height);
	}

	// Update xoffset and y offset with new height and width
	// setGridPosition(m_gridpos);	

    float wwww = m_width + m_nOffsetX;
    float hhhh = m_height + m_nOffsetY;
    // printf("[%s] XYZW %f:%f\n", m_name.c_str(), wwww, hhhh);

    // Note: The problem with this sort of packing is - you can't update
    // portions of data - the entire vertex array need to be unpacked to the GPU.
    // A better approach is to use XYWZ XYWZ XYWZ RGBA RGBA RGBA UV UV UV UV 
    // Currently it is XYWZ RGBA UV XYWZ RGBA UV. The former means we can use 
    // glBufferSubData to only update a sub section.
	Vertex vertices[] =
	{
			// XYZW							       // RGBA						// UV
		{{ m_nOffsetX, hhhh      , 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, {  0.0f, m_fCropFill / m_width }},
		{{ wwww      , hhhh      , 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, {  1.0f, m_fCropFill / m_width }},
		{{ m_nOffsetX, m_nOffsetY, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, txhh }},
		{{ wwww      , m_nOffsetY, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, {  1.0f, txhh }}
	};

    const size_t vertexSize = sizeof(vertices[0]);
    const size_t rgbOffset = sizeof(vertices[0].XYZW);    
    const size_t texOffset = sizeof(vertices[0].XYZW) + sizeof(vertices[0].RGBA);

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    // Describe vertex attributes stored in GPU's memory
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertexSize, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*) rgbOffset);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*) texOffset);

	// Enable the vertex attributes 
	// 0-Vertices 
	// 1-Colour
    // 2-Texture UV 
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
 	glUtil.exitOnGLError("ERROR: Could not create a VBO");

 	// Texture initialise
 	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

 	// Generate texture objects
 	glGenTextures(1, &textureID);

 	// Bind the texture object
 	glActiveTexture(GL_TEXTURE0);
 	glBindTexture(GL_TEXTURE_2D, textureID);

 	// Reserve data space on GPU
 	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_nTexMapX, m_nTexMapY, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
	// glUtil.exitOnGLError("ERROR: Could not create a VBO");

 	// Set the filtering mode
 	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Orthogonal Projeciton
	projectionMatrix =
		glUtil.createOrthogonalMatrix(
			-1, 100.0, 0.0, (float)screenWidth, 0.0, (float)screenHeight
	);

    glUseProgram(programID);
 	glUniform1i(samplerLoc, 0);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, projectionMatrix.m);
    glUseProgram(0);
    m_isVBOCreated = 1;
    printf("[%s] %s\n", m_name.c_str(), "GLVideo::createVBO END");
}
void wezside::GLVideo::resize(int w, int h)
{
    screenWidth = w;
    screenHeight = h;
    // printf("[%s] GLVideo::resize(%d,%d)\n", m_name.c_str(), w, h);

	// Orthogonal Projeciton
	projectionMatrix =
		glUtil.createOrthogonalMatrix(
			-1, 100.0, 0.0, (float)w, 0.0, (float)h
	);
    // glUtil.translateMatrix(&viewMatrix, 0, 0, 0);
	// printf("[%s] %s %d\n", m_name.c_str(), "resize()", programID);
    if (programID == 0) return;
	glUseProgram(programID);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, projectionMatrix.m);
	glUtil.exitOnGLError("[resize] ERROR: Could not set uniform 'projectionMatrixUniformLocation'");    
	glUseProgram(0);
   
}
void wezside::GLVideo::update()
{
    if (m_frames.size() > 0 && 
        m_currentFrame != m_frames.size())
    {
        if (m_playing == 1)
        {
			m_currentFrame++;  
/*			printf("[%s] %-3d::%d::%s::%s::%d::%d\n", 
								m_name.c_str(),
								m_currentFrame, 
								m_frames.size(), 
								m_playing == 1 ? "playing" :	 "not playing", 
								m_loaded == 1 ? "loaded" : "still loading",
								m_texture.cols, 
								m_texture.rows, 
								programID);   */
            if (m_currentFrame == m_frames.size())
            {
                m_playing = 0;
                m_currentFrame = 0;  
                ++m_playcount;
                m_frames.erase(m_frames.begin()+1, m_frames.end());
                m_autoplay = 0;
                printf("[%s] %s (%d)\n", m_name.c_str(), "Frames cleared", m_frames.size());
            }
        }
    }
}
void wezside::GLVideo::draw()
{
    if (m_firstFrameLoaded == 0 || programID == 0) return;
    // if (m_frames.size() >= m_currentFrame) return;

    // Make the shader program active
    glUseProgram(programID);
    // printf("[%s] Shader program: %d\n", m_name.c_str(), programID);
    std::string s = "[" + m_name + "] ERROR (draw): Could not use the shader program";
    if (glUtil.exitOnGLError(s.c_str()) == EXIT_FAILURE) return;    

    // Update the shader Uniform variables
	glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, modelMatrix.m);
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, viewMatrix.m);
	glUtil.exitOnGLError("ERROR: Could not set the shader view + model uniforms");

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
 
    // Use texture bound to location 0 (TEXTURE0)
    glUniform1i(samplerLoc, 0);
    // printf("%d::%d::%d::%d::%d::%d\n", m_texture.cols, m_texture.rows, m_currentFrame, m_frames.size(), programID, sizeof(m_frames.at(m_currentFrame).data));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_nTexMapX, m_nTexMapY, 0, GL_BGR, GL_UNSIGNED_BYTE, m_frames[m_currentFrame].data);
    // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_nTexMapX, m_nTexMapY, GL_BGR, GL_UNSIGNED_BYTE,  m_frames.at(m_currentFrame).data);
	if (glUtil.exitOnGLError("ERROR: Could not bind the texture") == EXIT_FAILURE) 
		return;

	glBindVertexArray(vaoID);
	if (glUtil.exitOnGLError("ERROR: Could not bind the VAO for drawing purposes") == EXIT_FAILURE)
	 return;

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if (glUtil.exitOnGLError("ERROR: Could not draw") == EXIT_FAILURE) return;

	// Cleanup
	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void wezside::GLVideo::destroyVBO()
{
    printf("%s\n", "GLVideo::destroyVBO()");
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);    
}