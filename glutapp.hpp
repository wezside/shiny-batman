#ifndef GLUTAPP_H
#define GLUTAPP_H
#define WINDOW_TITLE_PREFIX "Sample App"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <signal.h>
#include <unistd.h>
#include "glutapp.hpp"
#include "globject.hpp"
#include "glutils.hpp"
#include "tbb/concurrent_vector.h"

namespace wezside 
{
    class GlutApp
    {
        private:
            void initWindow(int, char*[]);
            // void keyboardFunction( unsigned char, int, int );

            GlutApp(const GlutApp&);
            GlutApp& operator=(GlutApp&);
            GLUtils glUtil;
            int m_width;
            int m_height;
            static float fps;
            static unsigned int frameCount;
            static unsigned int currentTime;
            static unsigned int previousTime;
            static int windowHandle;
            static GlutApp *m_self;
            bool m_manualLoop;
            bool m_bAutoFullscreen;
            bool m_constantFPS;
            
        protected:            
            GLuint
                vertexShaderId,
                fragmentShaderId,
                programID;

            const GLchar* vertexShader;
            const GLchar* fragmentShader;                

        public:

            GlutApp(const GLObject& glo) {};
            GlutApp& operator=(GLObject&) {};     
            GlutApp(int width = 1024, int height = 576);
            ~GlutApp();

            // std::vector<GLObject*> v;
			tbb::concurrent_vector<GLObject*> v;

            void init(int, char**);
            void initGLStates();

            virtual void render();
            virtual void resize(int, int);
            virtual void display();
            virtual void idle();
            virtual void timer(int);
            virtual void onKey(unsigned char, int, int);
            virtual void cleanup();
            virtual void loadShader(void);
            virtual void loadShader(const char*, GLenum);
            virtual void destroyShaders(void);
            virtual void createShader(const GLchar*, GLenum);
            virtual GLuint glslProgram();
            virtual void setManualLoop(bool loop) {m_manualLoop = loop;}
            virtual bool getManualLoop() {return m_manualLoop;}

            // void setWidth( int w ) { width = w; };
            // void setHeight( int h ) { height = h; };
            int getWidth() {return m_width;};
            int getHeight() {return m_height;};
            bool setAutoFS(bool fs){m_bAutoFullscreen = fs;}

            static void glutResize(int, int);
            static void glutDisplay();
            static void glutTimer(int);
            static void glutIdle(void);
            static void glutKeyboard(unsigned char key, int x, int y);
            static void glutCleanup(void);

    };

};
#endif // GLUTAPP_H
