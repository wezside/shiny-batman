#ifndef GLUTAPP_H
#define GLUTAPP_H
#define WINDOW_TITLE_PREFIX "Sample App"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glutapp.hpp"
#include "globject.hpp"

namespace wezside 
{
    class GlutApp
    {
        private:
            void initWindow(int, char*[]);
            // void keyboardFunction( unsigned char, int, int );

            const static int width = 640;
            const static int height = 480;
            static unsigned int frameCount;
            static int windowHandle;
            static GlutApp *m_self;

        public:

            GlutApp();
            ~GlutApp();

            GLObject* glo;

            void init(int argc, char** argv);
            void initGLStates();

            void resize(int w, int h);
            void render();
            void display();
            void idle();
            void timer(int value);
            void onKey(unsigned char key, int x, int y);
            void cleanup();

            // void setWidth( int w ) { width = w; };
            // void setHeight( int h ) { height = h; };
            int getWidth() {return width;};
            int getHeight() {return height;};

            // 
            static void glutResize(int, int);
            static void glutDisplay();
            static void glutTimer(int);
            static void glutIdle(void);
            static void glutKeyboard(unsigned char key, int x, int y);
            static void glutCleanup(void);
    };

};
#endif // GLUTAPP_H
