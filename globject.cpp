#include "globject.hpp"

using namespace std;
using namespace wezside;

string GLObject::getName()
{
    return m_name;
}

void GLObject::createVBO()
{
/*    cout << "GLObject::createVBO" << endl;

    Vertex vertices[] =
    {
        { { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // 0

        // Top
        { { -0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.5f, 1.0f } }, // 1
        { { 0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.5f, 1.0f } }, // 2
        { { 0.0f, 0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.5f, 1.0f } }, //3
        { { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.5f, 1.0f } }, // 4

        // Bottom
        { { -0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 5
        { { 0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 6
        { { 0.0f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } }, //7
        { { 0.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },    // 8

        // Left
        { { -0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 9
        { { -0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 10
        { { -0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } }, //11
        { { -1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },    // 12

        // Right
        { { 0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 13
        { { 0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 14
        { { 0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } }, //15
        { { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }  // 16
    };


    // Describes which indices to use from the Vertices array
    // If you look at the drawing this does actually make sense as
    // each array element from Vertices marked by these indices will
    // make up a triangle
    GLubyte indices[] = 
    {
        // Top
        0, 1, 3,
        0, 3, 2,
        3, 1, 4,
        3, 4, 2,

        // Bottom
        0, 5, 7,
        0, 7, 6,
        7, 5, 8,
        7, 8, 6,

        // Left
        0, 9, 11,
        0, 11, 10,
        11, 9, 12,
        11, 12, 10,

        // Right
        0, 13, 15,
        0, 15, 14,
        15, 13, 16,
        15, 16, 14
    };

    GLubyte alternateIndices[] = 
    {
        // Outer square border:
        3, 4, 16,
        3, 15, 16,
        15, 16, 8,
        15, 7, 8,
        7, 8, 12,
        7, 11, 12,
        11, 12, 4,
        11, 3, 4,

        // Inner square
        0, 11, 3,
        0, 3, 15,
        0, 15, 7,
        0, 7, 11
    };

    glUtil.translateMatrix(&viewMatrix, 0, 0, -2.0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glUtil.exitOnGLError("ERROR: Could not set OpenGL depth testing options");
     
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    glUtil.exitOnGLError("ERROR: Could not set OpenGL culling options");

    modelMatrixUniformLocation = glGetUniformLocation(programID, "ModelMatrix");
    viewMatrixUniformLocation = glGetUniformLocation(programID, "ViewMatrix");
    projectionMatrixUniformLocation = glGetUniformLocation(programID, "ProjectionMatrix");

    glUtil.exitOnGLError("ERROR: Could not get shader uniform locations");

    const size_t bufferSize = sizeof( vertices );
    const size_t vertexSize = sizeof( vertices[0] );
    const size_t rgbOffset = sizeof( vertices[0].XYZW );

	glGenVertexArrays( 1, &vaoID );
	glBindVertexArray( vaoID );

	glGenBuffers( 1, &bufferID );
	glBindBuffer( GL_ARRAY_BUFFER, bufferID );
	glBufferData( GL_ARRAY_BUFFER, bufferSize, vertices, GL_STATIC_DRAW );

    // Indicate how the vertice data is stored
	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, vertexSize, 0 );

	// Indicate where the colour data is stored
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, vertexSize, ( GLvoid* ) rgbOffset );

	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

    glGenBuffers( 2, indexBufferID );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBufferID[0]);    
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBufferID[1]);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( alternateIndices ), alternateIndices, GL_STATIC_DRAW );

    glUtil.exitOnGLError("ERROR: Could not create a VBO");*/
}

void GLObject::destroyVBO()
{
/*    cout << "GLObject::destroyVBO" << endl;
	GLenum ErrorCheckValue = glGetError();

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &bufferID);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // glDeleteBuffers( 2, IndexbufferID );

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoID);

    glUtil.exitOnGLError("ERROR: Could not destroy the VBO");*/
}


void GLObject::onKey(unsigned char key, int x, int y)
{
/*    switch ( key )
    {
        case 'T':
        case 't':
        {
            setActiveIndexBuffer(getActiveIndexBuffer() == 1 ? 0 : 1);
            break;
        }
        default: break;
    }*/
}
void GLObject::draw()
{
    // Draw stuff here
    // Make the shader program active
/*    if (programID == 0) return;
    glUseProgram(programID);
    glUtil.exitOnGLError("ERROR: Could not use the shader program");    

    // Update the shader Uniform variables
    glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, modelMatrix.m);
    glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, viewMatrix.m);
    glUtil.exitOnGLError("ERROR: Could not set the shader uniforms");

    glBindVertexArray(vaoID);
    glUtil.exitOnGLError("ERROR: Could not bind the VAO for drawing purposes");
    if (getActiveIndexBuffer() == 0)
    {
        glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_BYTE, NULL);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);
    }    
    glUtil.exitOnGLError("ERROR: Could not draw");

    // Cleanup
    glBindVertexArray(0);
    glUseProgram(0);    */
}
void GLObject::resize(int w, int h)
{
/*    screenWidth = w;
    screenHeight = h;
    printf("GLObject::resize(%d,%d)\n", w, h);
    projectionMatrix =
        glUtil.createProjectionMatrix(
            60,
            (float)w / h,
            1.0f,
            100.0f
        );
    // Move the Eye(Cam) space backwards so we can see all
    glUtil.translateMatrix(&viewMatrix, 0, 0, -2);
    glUseProgram(programID);
    glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, projectionMatrix.m);
    glUseProgram(0);  */  
}

GLuint GLObject::getActiveIndexBuffer(void)
{
	return activeIndexBuffer;
}

void GLObject::setActiveIndexBuffer(GLuint bufferIndex)
{
/*    activeIndexBuffer = bufferIndex;
    // TODO: Find a fix for this - indices are destroyed once data is uploaded to GPU
    indexBufferID[0] = 2;
    indexBufferID[1] = 3;
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBufferID[bufferIndex]);*/
}

void GLObject::setFPS(unsigned int fps)
{
    currentFPS = fps;
}

void GLObject::updateScreen(int w, int h)
{
    screenWidth = w;
    screenHeight = h;
}