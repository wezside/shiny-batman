#include "glparticlesystem.hpp"

void wezside::GLParticleSystem::createVBO()
{
	samplerLoc = glGetUniformLocation(programID, "s_texture");  
	modelMatrixUniformLocation = glGetUniformLocation(programID, "u_modelMatrix");
	viewMatrixUniformLocation = glGetUniformLocation(programID, "u_viewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(programID, "u_projectionMatrix");  

	// unifrom float u_time;
	// uniform float u_centerPosition;
	timeLoc = glGetUniformLocation(programID, "u_time");
	centerPosLoc = glGetUniformLocation(programID, "u_centerPosition");

	glUtil.exitOnGLError("ERROR: Could not create a VBO");

	ParticleVertex vertices[NUM_PARTICLES];
	for (int i = 0; i < NUM_PARTICLES; ++i)
	{
		float xxxx = (((float)(rand() % 10000) / 10000.0f) - 0.5f);
		float yyyy = (((float)(rand() % 10000) / 10000.0f) - 0.5f);
		float rrrr = (((float)(rand() % 10000) / 20000.0f) + 0.5f);
		float gggg = (((float)(rand() % 10000) / 20000.0f) + 0.5f);
		float bbbb = (((float)(rand() % 10000) / 20000.0f) + 0.5f);
		float aaaa = 0.5f;
		printf("%f,%f,%f,%f\n", rrrr, gggg, bbbb, aaaa);

		// Start Position
		vertices[i].ABCD[0] = 0.0f;
		vertices[i].ABCD[1] = 0.0f;
		vertices[i].ABCD[2] = 0.0f;
		vertices[i].ABCD[3] = 0.0f;

		// End Position
		vertices[i].XYZW[0] = xxxx;
		vertices[i].XYZW[1] = yyyy;
		vertices[i].XYZW[2] = 0.0f;
		vertices[i].XYZW[3] = 0.0f;		

		// RGB
		vertices[i].RGBA[0] = rrrr;
		vertices[i].RGBA[1] = gggg;
		vertices[i].RGBA[2] = bbbb;
		vertices[i].RGBA[3] = aaaa;

		// Lifetime
		vertices[i].LIFE = 5.0f;
	}
	const size_t vertexSize = sizeof(vertices[0]);
	const size_t endOffset = sizeof(vertices[0].ABCD);    
	const size_t rgbOffset = sizeof(vertices[0].ABCD) + sizeof(vertices[1].XYZW);    
	const size_t lifeOffset = sizeof(vertices[0].ABCD) + sizeof(vertices[1].XYZW) + sizeof(vertices[2].RGBA);    

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

	// Enable the vertex attributes 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glUtil.exitOnGLError("ERROR: Could not create a VBO");

	// Texture initialise
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Generate texture objects
	glGenTextures(1, &textureID);

	// Bind the texture object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Upload texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_nTexMapX, m_nTexMapY, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);
	glUtil.exitOnGLError("ERROR: Could not create a VBO");

	// Set the filtering mode
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
	glUniform1f(timeLoc, 0.0f);
	glUniform3f(centerPosLoc, 0.0f, 0.0f, 0.0f);
	glUseProgram(0);	
	m_isVBOCreated = 1;
}
void wezside::GLParticleSystem::resize(int w, int h)
{
}
void wezside::GLParticleSystem::update()
{
	m_time += 0.01;
	glUseProgram(programID);
	glUniform1f(timeLoc, m_time);
	glUseProgram(0);
}
void wezside::GLParticleSystem::draw()
{
	glUseProgram(programID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

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
void wezside::GLParticleSystem::destroyVBO()
{
	printf("%s\n", "GLParticleSystem::destroyVBO()");
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);    
}