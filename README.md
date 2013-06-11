This is a scaffold project to build reusable "GLObjects" with which to draw OpenGL grahics with.

Glut is simply used as a container but for work purposes something like OpenFrameworks is better suited as it provides better cross-platform support. Currently GLObjects are not OF compatible because they upload their own shaders and alter the projection matrices.

Features:
---------
- Simple GLObject inheritence chain
- GLSensorViewer will pull RGB cam data from 3D sensor using OpenNI2 SDK
- GLObject will perform a basic draw operation using Vertex Buffer Objects

To do:
------
- ~~Split display into update and draw methods for easier integration with OF~~
- Create an example GL Object which doesn't initialise in createVBO() but rather in resize()
	- reason for this is with use of textures to know the screensize
	- alternatively resize the texture on screen resize

Build
-----

- Change the location of the CPPFLAGS shared OpenNI lib in the Makefile
- Run "make"

Custom GLObjects
----------------

The GLObject class is meant to be abstract but at this time contains some default code for testing purposes. The idea though is to easily create your own reusable objects. To create your own drawable OpenGL object the easiest way is to override:

	// Initialise code
	int init(void);

	// Set-up your VBOs
	void createVBO();

	// In a glut app resize is called at least once so this is 
	// a good place to put Projection Matrix code 
	void resize(int, int);

	// Similar to OF this is where values like vertices or structs can be 
	// updated. In the GLSensorViewer I use this to update the 3D camera readings
    void update();
	
	// OpenGL draw code
	// 		Enable active shader program
	// 		Enable active texture
	// 		Unpack new texture data if needed
	// 		Draw VBO arrays or elements
	void draw();

To use this object:

	GLCustomObject glco;
	glco.loadShader();	
	glco.glslProgram();
    glco.createVBO();
    glco.update();
    glco.draw();
    glco.resize();