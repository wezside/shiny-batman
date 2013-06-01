#include "glutapp.hpp"

using namespace std;
using namespace wezside;

int GlutApp::windowHandle( 0 );
unsigned int GlutApp::frameCount( 0 );
GlutApp *GlutApp::m_self = NULL;

GlutApp::GlutApp()
{
    GlutApp::m_self = this;
}

GlutApp::~GlutApp()
{  
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

void GlutApp::init( int argc, char** argv )
{
    GLenum glewInitResult;
    cout << "GlutApp::initialize()" << endl;

    // Initialize FreeGlut
    initWindow( argc, argv );

    // Specifically needed for OpenGL 3.x
    // Temp fix for this issue http://www.opengl.org/wiki/OpenGL_Loading_Library
    // GLEW's problem is that it calls glGetString(GL_EXTENSIONS) which causes GL_INVALID_ENUM on GL 3.2 core context
	// Currently, GLEW uses glGetString(GL_EXTENSIONS) which is not legal code
	// in GL 3.3, therefore GLEW would fail if we don't set this to TRUE.
	// GLEW will avoid looking for extensions and will just get function pointers for all GL functions.
    glewExperimental = GL_TRUE;
	glewInitResult = glewInit();

    if ( GLEW_OK != glewInitResult )
    {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString( glewInitResult )
		);
		exit( EXIT_FAILURE );
	}

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);

    initGLStates();

    glo->createVBO();
    glo->loadShader();

    // Set intitial background colour
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
}

void GlutApp::initGLStates()
{
    glShadeModel( GL_SMOOTH );
}

void GlutApp::initWindow( int argc, char* argv[] )
{
	glutInit( &argc, argv );

	glutInitContextVersion( 3, 3 ); // Need to make sure these are set according to GL_VERSION
	glutInitContextFlags( GLUT_FORWARD_COMPATIBLE );
	glutInitContextProfile( GLUT_CORE_PROFILE );

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
	);

	glutInitWindowSize( width, height );
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );

	windowHandle = glutCreateWindow( WINDOW_TITLE_PREFIX );
	if ( windowHandle < 1 )
	{
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
		);
		exit( EXIT_FAILURE );
	}
	glutReshapeFunc(glutResize);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);
    glutTimerFunc(0, glutTimer, 0);
	glutKeyboardFunc(glutKeyboard);

/*	glutMouseFunc(mouseWrapper);
	glutMotionFunc(motionWrapper);
	glutPassiveMotionFunc(passiveMotionWrapper);
	glutMouseWheelFunc(mouseWheelWrapper) ;

	glutKeyboardFunc(keyboardWrapper);
	glutKeyboardUpFunc(keyboardUpWrapper);
	glutSpecialFunc(specialWrapper);
	glutSpecialUpFunc(specialUpWrapper);*/

    glutCloseFunc(glutCleanup);
}
void GlutApp::render(void)
{
    glutMainLoop();
}
void GlutApp::onKey(unsigned char key, int x, int y)
{
	if (glo != NULL) glo->onKey(key, x, y);
	switch ( key )
	{
		case 'W':
		case 'w': glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
		case 'S':
		case 's': glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;		
		default: break;
	}
}
void GlutApp::resize(int w, int h)
{
	glViewport(0, 0, w, h);
}
void GlutApp::display(void)
{
    ++frameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Call GLObj display here
	glo->display();

    // Draw stuff here
	if (glo->getActiveIndexBuffer() == 0)
	{
		glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_BYTE, NULL);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);
	}

	glutSwapBuffers();
	glutPostRedisplay();
}
void GlutApp::idle( void )
{
	glutPostRedisplay();
}
void GlutApp::timer( int value )
{
	if ( 0 != value )
	{

		char* tempString = ( char* ) malloc( 512 + strlen( WINDOW_TITLE_PREFIX ));
		sprintf(
			tempString,
			"%s: %d Frames Per Second @ %d x %d",
			WINDOW_TITLE_PREFIX,
			frameCount * 4,
			width,
			height
		);

		glutSetWindowTitle( tempString );
		free( tempString );
	}
	frameCount = 0;
	glutTimerFunc( 250, glutTimer, 1 );
}
void GlutApp::cleanup( void )
{
}
