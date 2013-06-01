#include "globject.hpp"

using namespace wezside;
using namespace std;

typedef struct
{
    float XYZW[4];
    float RGBA[4];
} Vertex;


const GLchar* VertexShader =
{
    "#version 330\n"\

    "layout(location=0) in vec4 in_Position;\n"\
    "layout(location=1) in vec4 in_Color;\n"\
    "out vec4 ex_Color;\n"\

    "void main(void)\n"\
    "{\n"\
    "	gl_Position = in_Position;\n"\
    "	ex_Color = in_Color;\n"\
    "}\n"
};

const GLchar* FragmentShader =
{
    "#version 330\n"\

    "in vec4 ex_Color;\n"\
    "out vec4 out_Color;\n"\

    "void main(void)\n"\
    "{\n"\
    "	out_Color = ex_Color;\n"\
    "}\n"
};


string GLObject::getName()
{
    return name;
}

void GLObject::createVBO()
{
    cout << "GLObject::createVBO" << endl;

    Vertex Vertices[] =
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
    GLubyte Indices[] = 
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

    GLubyte AlternateIndices[] = 
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

    GLenum ErrorCheckValue = glGetError();
    const size_t BufferSize = sizeof( Vertices );
    const size_t VertexSize = sizeof( Vertices[0] );
    const size_t RgbOffset = sizeof( Vertices[0].XYZW );

	glGenVertexArrays( 1, &VaoId );
	glBindVertexArray( VaoId );

	glGenBuffers( 1, &BufferId );
	glBindBuffer( GL_ARRAY_BUFFER, BufferId );
	glBufferData( GL_ARRAY_BUFFER, BufferSize, Vertices, GL_STATIC_DRAW );

    // Indicate how the vertice data is stored
	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0 );

	// Indicate where the colour data is stored
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, VertexSize, ( GLvoid* ) RgbOffset );

	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

    glGenBuffers( 2, IndexBufferId );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[0]);    
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( Indices ), Indices, GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[1]);
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( AlternateIndices ), AlternateIndices, GL_STATIC_DRAW );

    // Copy
    // memcpy( IndexBufferId, IndexBufferId, sizeof(IndexBufferId));
    // glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[0]);
    // cout << IndexBufferId[0] << endl;

	ErrorCheckValue = glGetError();
	if ( ErrorCheckValue != GL_NO_ERROR )
	{
		fprintf(
			stderr,
			"ERROR: Could not create a VBO: %s \n",
			gluErrorString(ErrorCheckValue)
		);

		exit(-1);
	}
}

void GLObject::destroyVBO()
{
    cout << "GLObject::destroyVBO" << endl;
	GLenum ErrorCheckValue = glGetError();

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &BufferId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // glDeleteBuffers( 2, IndexBufferId );

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not destroy the VBO: %s \n",
			gluErrorString(ErrorCheckValue)
		);

		exit(-1);
	}
}

void GLObject::loadShader()
{
    printf("GLObject::loadShader()");
    createShader(VertexShader, GL_VERTEX_SHADER);
    createShader(FragmentShader, GL_FRAGMENT_SHADER);
}

void GLObject::loadShader(const char* fname, GLuint shader)
{
    printf("GLObject::loadShader(const char*, GLuint)");

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

void GLObject::createShader(const GLchar* shaderSrc, GLuint shader)
{
    GLuint shaderID;
	GLenum ErrorCheckValue = glGetError();
    if (ProgramId == 0) ProgramId = glCreateProgram();
    switch(shader)
    {
        case GL_VERTEX_SHADER: 
            shaderID = VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
            break;

        case GL_FRAGMENT_SHADER: 
            shaderID = FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
            break;
    }
    glShaderSource(shaderID, 1, &shaderSrc, NULL);
    glCompileShader(shaderID);
    glAttachShader(ProgramId, shaderID);
	glLinkProgram(ProgramId);
    glUseProgram(ProgramId);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n",
			gluErrorString(ErrorCheckValue)
		);
		exit(-1);
	}
}

void GLObject::destroyShaders()
{
    cout << "GLObject::destroyShaders" << endl;
	GLenum ErrorCheckValue = glGetError();

	glUseProgram(0);

	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);

	glDeleteProgram(ProgramId);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not destroy the shaders: %s \n",
			gluErrorString(ErrorCheckValue)
		);

		exit(-1);
	}
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
GLuint GLObject::getIndexBufferId(void)
{
	// return IndexBufferId[ ActiveIndexBuffer ];
}

GLuint GLObject::getActiveIndexBuffer(void)
{
	return ActiveIndexBuffer;
}

void GLObject::setActiveIndexBuffer(GLuint bufferIndex)
{
    ActiveIndexBuffer = bufferIndex;
    // TODO: Find a fix for this - indices are destroyed once data is uploaded to GPU
    IndexBufferId[0] = 2;
    IndexBufferId[1] = 3;
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[bufferIndex]);
}
