#include "globject.hpp"

using namespace std;
using namespace wezside;

string GLObject::getName()
{
    return m_name;
}
void GLObject::createVBO()
{
}
void GLObject::destroyVBO()
{
}
void GLObject::onKey(unsigned char key, int x, int y)
{
}
void GLObject::draw()
{
}
void GLObject::resize(int w, int h)
{
}
GLuint GLObject::getActiveIndexBuffer(void)
{
	return activeIndexBuffer;
}
void GLObject::setActiveIndexBuffer(GLuint bufferIndex)
{
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