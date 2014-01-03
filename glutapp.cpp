#include "glutapp.hpp"

using namespace std;
using namespace wezside;

int GlutApp::windowHandle( 0 );
float GlutApp::fps( 0 );
unsigned int GlutApp::frameCount( 0 );
unsigned int GlutApp::currentTime( 0 );
unsigned int GlutApp::previousTime( 0 );

GlutApp *GlutApp::m_self = NULL;
GlutApp::GlutApp(int width, int height)
{
	m_width = width;
	m_height = height;
	m_manualLoop = 1;
	m_bAutoFullscreen = 0;
    GlutApp::m_self = this;
	vertexShader =  "#version 330\n"\
					"layout(location=0) in vec4 in_Position;\n"\
					"layout(location=1) in vec4 in_Color;\n"\
					"out vec4 ex_Color;\n"\
					"uniform mat4 modelMatrix;\n"\
					"uniform mat4 viewMatrix;\n"\
					"uniform mat4 projectionMatrix;\n"\
					"void main(void)\n"\
					"{\n"\
					"   gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * in_Position;\n"\
					"   ex_Color = in_Color;\n"\
					"}\n";

	fragmentShader = 	"#version 330\n"\
						"in vec4 ex_Color;\n"\
						"out vec4 out_Color;\n"\
						"void main(void)\n"\
						"{\n"\
						"   out_Color = ex_Color;\n"\
						"}\n";
}

GlutApp::~GlutApp()
{  
	destroyShaders();
	for (tbb::concurrent_vector<GLObject*>::iterator it = v.begin(); it != v.end(); ++it)
	{
		printf("~GLObject Mem Addr::%p\n", *it);
		if (!(*it)) delete *it;
		*it = NULL;
	}
	v.clear();
}
void GlutApp::glutKeyboard(unsigned char key, int x, int y)
{
	GlutApp::m_self->onKey(key, x, y);
}
void GlutApp::glutResize(int w, int h)
{
	GlutApp::m_self->resize(w, h);
}
void GlutApp::glutDisplay(void)
{
	GlutApp::m_self->display();
}
void GlutApp::glutIdle(void)
{
	GlutApp::m_self->idle();
}
void GlutApp::glutTimer( int value )
{	
	GlutApp::m_self->timer( value );
}
void GlutApp::glutCleanup( void )
{
	GlutApp::m_self->cleanup();
}
void GlutApp::init(int argc, char** argv)
{
    GLenum glewInitResult;
    cout << "GlutApp::initialize()" << endl;

    // Initialize FreeGlut
    initWindow(argc, argv);

    // Specifically needed for OpenGL 3.x
    // Temp fix for this issue http://www.opengl.org/wiki/OpenGL_Loading_Library
    // GLEW's problem is that it calls glGetString(GL_EXTENSIONS) which causes GL_INVALID_ENUM on GL 3.2 core context
	// Currently, GLEW uses glGetString(GL_EXTENSIONS) which is not legal code
	// in GL 3.3, therefore GLEW would fail if we don't set this to TRUE.
	// GLEW will avoid looking for extensions and will just get function pointers for all GL functions.
    glewExperimental = GL_TRUE;
	glewInitResult = glewInit();
	
    if (GLEW_OK != glewInitResult)
    {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString( glewInitResult )
		);
		exit(EXIT_FAILURE);
	}

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);

    initGLStates();

    // Set intitial background colour
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
}

void GlutApp::initGLStates()
{
	glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    // glEnable(GL_SCISSOR); // Used with Ortho-Projection to clip outside viewport
}

void GlutApp::initWindow( int argc, char* argv[] )
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3); // Need to make sure these are set according to GL_VERSION
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_CONTINUE_EXECUTION
	);
	glutInitWindowSize(m_width, m_height);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	windowHandle = glutCreateWindow( WINDOW_TITLE_PREFIX );
	if ( windowHandle < 1 )
	{
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
		);
		exit(EXIT_FAILURE);
	}
	glutReshapeFunc(glutResize);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);
    glutTimerFunc(0, glutTimer, 0);
	glutKeyboardFunc(glutKeyboard);
    glutCloseFunc(glutCleanup);

	if (m_bAutoFullscreen == 1) glutFullScreenToggle();

	printf("[%s] %s\n", "glutapp", "Window intialised.");

	// glViewport(0, 0, m_width, m_height);
/*	glutMouseFunc(mouseWrapper);
	glutMotionFunc(motionWrapper);
	glutPassiveMotionFunc(passiveMotionWrapper);
	glutMouseWheelFunc(mouseWheelWrapper) ;

	glutKeyboardFunc(keyboardWrapper);
	glutKeyboardUpFunc(keyboardUpWrapper);
	glutSpecialFunc(specialWrapper);
	glutSpecialUpFunc(specialUpWrapper);*/
}
void GlutApp::render(void)
{
  	m_manualLoop ? glutMainLoopEvent() : glutMainLoop();
}
void GlutApp::onKey(unsigned char key, int x, int y)
{
    for (tbb::concurrent_vector<GLObject*>::iterator it = v.begin() ; it != v.end(); ++it)
		(*it)->onKey(key, x, y);

	switch (key)
	{
		case 'W':
		case 'w': glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
		case 'S':
		case 's': glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;		
		case 'f': 
		case 'F': glutFullScreenToggle(); break;
		case 27 : glutDestroyWindow(windowHandle); break;
		default: break;
	}
}
void GlutApp::loadShader()
{
    std::cout << "GlutApp::loadShader()" << std::endl;   
    GLenum ErrorCheckValue = glGetError();
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShader, NULL);
    glCompileShader(vertexShaderId);

    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, &fragmentShader, NULL);
    glCompileShader(fragmentShaderId);
    glUtil.exitOnGLError("ERROR: Could not create the shaders");    
}

void GlutApp::loadShader(const char* fname, GLenum shader)
{
    std::cout << "GlutApp::loadShader(const char*, GLuint)" << std::endl;

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

void GlutApp::createShader(const GLchar* shaderSrc, GLenum shader)
{
    std::cout << "GlutApp::createShader(const GLchar*, GLenum)" << std::endl;
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
GLuint GlutApp::glslProgram()
{
    GLint tmp;
    GLenum ErrorCheckValue = glGetError();
	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderId);  
	glUtil.exitOnGLError("ERROR: Could not attach Vertex Shader");
	glAttachShader(programID, fragmentShaderId);    
	glUtil.exitOnGLError("ERROR: Could not attach Fragment Shader");
	glLinkProgram(programID);
	glUtil.exitOnGLError("ERROR: Could not link shader program");
	glUseProgram(programID);
	glUtil.exitOnGLError("ERROR: Could not use shader program");

	glGetIntegerv(GL_CURRENT_PROGRAM, &tmp);
	std::cout << "GlutApp::Current GLSLS Program " << programID << std::endl;    

/*	for (tbb::concurrent_vector<GLObject*>::iterator it = v.begin(); it != v.end(); ++it)
	{
		(*it)->setProgramID(programID);
		(*it)->createVBO();
	}*/
	return programID;
}
void GlutApp::destroyShaders()
{
    cout << "GlutApp::destroyShaders" << endl;
	GLenum ErrorCheckValue = glGetError();

	glUseProgram(0);

	glDetachShader(programID, vertexShaderId);
	glDetachShader(programID, fragmentShaderId);

	glDeleteShader(fragmentShaderId);
	glDeleteShader(vertexShaderId);

	glDeleteProgram(programID);

    glUtil.exitOnGLError("ERROR: Could not destroy the shaders");
}
void GlutApp::resize(int w, int h)
{
	m_width = w;
	m_height = h;
	// printf("[%s]resize(%d, %d)\n", "glutapp", w, h );
	for (tbb::concurrent_vector<GLObject*>::iterator it = v.begin(); it != v.end(); ++it)
		(*it)->resize(w, h);
	glViewport(0, 0, m_width, m_height);	
}
void GlutApp::display(void)
{
    ++frameCount;
    // calculateFPS();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Call GLObj display here
	for (tbb::concurrent_vector<GLObject*>::iterator it = v.begin(); it != v.end(); ++it)
	{
		// if (!(*it)) continue;
		if (!(*it)->isVBOCreated())
		{
			(*it)->updateScreen(m_width, m_height);
			(*it)->setProgramID(programID);
			(*it)->createVBO();				
		}
		(*it)->setFPS(frameCount);
		(*it)->update();
		(*it)->draw();
	}

	// Constant FPS
	previousTime = currentTime;
	int timeInterval = currentTime - previousTime;
    if (timeInterval < 30)
    	usleep(30*1000 - timeInterval);
    currentTime = glutGet(GLUT_ELAPSED_TIME);

	glutSwapBuffers();
	glutPostRedisplay();
}
void GlutApp::idle( void )
{
	glutSwapBuffers();
	glutPostRedisplay();
}
void GlutApp::timer(int value)
{
	if (0 != value)
	{
		char* tempString = (char*) malloc(512 + strlen(WINDOW_TITLE_PREFIX));
		sprintf(
			tempString,
			"%s: %d Frames Per Second @ %d x %d",
			WINDOW_TITLE_PREFIX,
			frameCount * 4,
			m_width,
			m_height
		);
		glutSetWindowTitle(tempString);
		free(tempString);
	}
	frameCount = 0;
	glutTimerFunc(250, glutTimer, 1);
}
void GlutApp::cleanup(void) 
{
	printf("%s\n", "GlutApp::cleanup");	
	
	glutLeaveMainLoop();
	raise(SIGINT);
}