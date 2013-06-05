This is a scaffold project to build reusable "GLObjects" with which to draw OpenGL grahics with.

Glut is simply used as a container but for work purposes something like OpenFrameworks is better suited as it provides better cross-platform support. 

Features:
---------
- Simple GLObject inheritence chain
- GLSensorViewer will pull RGB cam data from 3D sensor using OpenNI2 SDK
- GLObject will perform a basic draw operation using Vertex Buffer Objects

To do:
------
- ~~Split display into update and draw methods for easier integration with OF~~
- Create an example GL Object which doesn't initialise in createVBO() but rather in resize()
	o reason for this is with use of textures to know the screensize
	o alternatively resize the texture on screen resize