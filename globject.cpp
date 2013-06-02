#include "globject.hpp"

using namespace std;
using namespace wezside;

string GLObject::getName()
{
    return name;
}

void GLObject::createVBO()
{
    cout << "GLObject::createVBO" << endl;
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

    glUtil.exitOnGLError("ERROR: Could not create a VBO");
}

void GLObject::destroyVBO()
{
    cout << "GLObject::destroyVBO" << endl;
	GLenum ErrorCheckValue = glGetError();

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &bufferID);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // glDeleteBuffers( 2, IndexbufferID );

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoID);

    glUtil.exitOnGLError("ERROR: Could not destroy the VBO");
}

void GLObject::loadShader()
{
    std::cout << "GLObject::loadShader()" << std::endl;   
    createShader(vertexShader, GL_VERTEX_SHADER);
    createShader(fragmentShader, GL_FRAGMENT_SHADER);
}

void GLObject::loadShader(const char* fname, GLenum shader)
{
    std::cout << "GLObject::loadShader(const char*, GLuint)" << std::endl;

    // Load file 
    std::fstream fShader(fname, std::ios::in);
    std::string strShader;
     
    if (fShader.is_open())
    {
        std::stringstream buffer;
        buffer << fShader.rdbuf();
        strShader = buffer.str();
    }
    createShader(strShader.c_str(), shader);
}

void GLObject::createShader(const GLchar* shaderSrc, GLenum shader)
{
    std::cout << "GLObject::createShader(const GLchar*, GLenum)" << std::endl;
    GLenum ErrorCheckValue = glGetError();
    if (shader == GL_VERTEX_SHADER)
    {
        vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderId, 1, &shaderSrc, NULL);
        glCompileShader(vertexShaderId);
    }
    if (shader == GL_FRAGMENT_SHADER)
    {
        fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderId, 1, &shaderSrc, NULL);
        glCompileShader(fragmentShaderId);
    }
    glUtil.exitOnGLError("ERROR: Could not create the shaders");
}
void GLObject::glslProgram()
{
    GLint tmp;
    glGetIntegerv(GL_CURRENT_PROGRAM, &tmp);
    std::cout << "GLObject::Current GLSLS Program " << programID << std::endl;    
    GLenum ErrorCheckValue = glGetError();
    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderId);  
    glAttachShader(programID, fragmentShaderId);    
    glLinkProgram(programID);
    glUseProgram(programID);

    glUtil.exitOnGLError("ERROR: Could not create the GLSL program");
}
void GLObject::destroyShaders()
{
    cout << "GLObject::destroyShaders" << endl;
	GLenum ErrorCheckValue = glGetError();

	glUseProgram(0);

	glDetachShader(programID, vertexShaderId);
	glDetachShader(programID, fragmentShaderId);

	glDeleteShader(fragmentShaderId);
	glDeleteShader(vertexShaderId);

	glDeleteProgram(programID);

    glUtil.exitOnGLError("ERROR: Could not destroy the shaders");
}
void GLObject::onKey(unsigned char key, int x, int y)
{
    switch ( key )
    {
        case 'T':
        case 't':
        {
            setActiveIndexBuffer(getActiveIndexBuffer() == 1 ? 0 : 1);
            break;
        }
        default: break;
    }
}
void GLObject::resize(int w, int h)
{
    projectionMatrix =
        glUtil.createProjectionMatrix(
            60,
            (float)w / h,
            1.0f,
            100.0f
        );

    glUseProgram(programID);
    glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, projectionMatrix.m);
    glUseProgram(0);    
}

GLuint GLObject::getActiveIndexBuffer(void)
{
	return activeIndexBuffer;
}

void GLObject::setActiveIndexBuffer(GLuint bufferIndex)
{
    activeIndexBuffer = bufferIndex;
    // TODO: Find a fix for this - indices are destroyed once data is uploaded to GPU
    indexBufferID[0] = 2;
    indexBufferID[1] = 3;
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBufferID[bufferIndex]);
}
