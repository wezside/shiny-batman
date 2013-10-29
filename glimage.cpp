#include "glparticlesystem.hpp"

void wezside::GLImage::createVBO()
{
	m_tex = cv::imread("data/background.jpg");
	m_nTexMapX = m_tex.cols;
	m_nTexMapY = m_tex.rows;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	samplerLoc = glGetUniformLocation(programID, "u_texture");  
	modelMatrixUniformLocation = glGetUniformLocation(programID, "u_modelMatrix");
	viewMatrixUniformLocation = glGetUniformLocation(programID, "u_viewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(programID, "u_projectionMatrix");  

	glUtil.exitOnGLError("ERROR: Could not create a VBO");
	
	Vertex vertices[] =
	{
			// XYZW						// RGBA						// UV
		{{ 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
		{{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }},
		{{ 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }},
		{{ 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 0.0f }}
	};
	const size_t vertexSize = sizeof(vertices[0]);
	const size_t rgbOffset = sizeof(vertices[0].XYZW) + sizeof(vertices[1].XYZW);    
	const size_t texOffset = sizeof(vertices[0].ABCD) + sizeof(vertices[1].XYZW) + sizeof(vertices[2].RGBA);    
	const size_t velOffset = sizeof(vertices[0].ABCD) + sizeof(vertices[1].XYZW) + sizeof(vertices[2].RGBA) + sizeof(vertices[2].LIFE);

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
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*) endOffset);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*) rgbOffset);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*) lifeOffset);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*) velOffset);

	// Enable the vertex attributes 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glUtil.exitOnGLError("ERROR: Could not create a VBO");

	// Texture initialise
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// Generate texture objects
	glGenTextures(1, &textureID);

	// Bind the texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Upload texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_nTexMapX, m_nTexMapY, 0, GL_BGR, GL_UNSIGNED_BYTE, m_tex.data);
	glUtil.exitOnGLError("ERROR: Could not create a VBO");

	// Set the filtering mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Projeciton Matrix
	projectionMatrix =
		glUtil.createOrthogonalMatrix(
			-1, 100.0, 0.0, (float)screenWidth, 0.0, (float)screenHeight
	);
/*    projectionMatrix =
        glUtil.createProjectionMatrix(
            60,
            (float)screenWidth / screenHeight,
            1.0f,
            1000.0f
        );
*/
	glUseProgram(programID);
	glUniform1i(samplerLoc, 0);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, projectionMatrix.m);
	glUniform1f(timeLoc, 0.0f);
	glUniform3f(centerPosLoc, 0.0f, 0.0f, 0.0f);
	glUniform1f(waveTimeLoc, waveTime);
	glUniform1f(waveWidthLoc, waveWidth);
	glUniform1f(waveHeightLoc, waveHeight);	
	glUseProgram(0);	
	m_isVBOCreated = 1;
}
void wezside::GLImage::resize(int w, int h)
{
}
void wezside::GLImage::update()
{
	m_time += 0.0005;
	waveTime += waveFreq;

	if (m_time >= 1.0f)
	{
		m_time = 0.0f;
	}

	glUseProgram(programID);
	glUniform1f(timeLoc, m_time);
	glUniform1f(waveTimeLoc, waveTime);	
	glUseProgram(0);
}
void wezside::GLImage::draw()
{
	glUseProgram(programID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(samplerLoc, 0);


	glBindVertexArray(vaoID);
	if (glUtil.exitOnGLError("ERROR: Could not bind the VAO for drawing purposes") == EXIT_FAILURE) return;

	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);
	if (glUtil.exitOnGLError("ERROR: Could not draw") == EXIT_FAILURE) return;

	// Cleanup
	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0); 
}
void wezside::GLImage::destroyVBO()
{
	printf("%s\n", "GLImage::destroyVBO()");
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);    
}