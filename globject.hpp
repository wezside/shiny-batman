#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

namespace wezside
{  
    class GLObject
    {
        private:
            GLObject(const GLObject&);
            GLObject& operator=(GLObject&);        

        protected:
            std::string name;
            GLuint
                VertexShaderId,
                FragmentShaderId,
                ProgramId,
                VaoId,
                BufferId,
                IndexBufferId[2],
            	ActiveIndexBuffer,
                buffer1,buffer2;        
            virtual void createShader(const GLchar*, GLuint);

        public:
            GLObject(std::string value = "default") : ActiveIndexBuffer(0), name(value), ProgramId(0) {}
            ~GLObject()
            {
                destroyVBO();
                destroyShaders();
            }

            virtual std::string getName();
            virtual void display(){};
            virtual void createVBO();
            virtual void destroyVBO();
            virtual void destroyShaders(void);
            virtual void loadShader();
            virtual void loadShader(const char*, GLuint);
            virtual void onKey(unsigned char, int, int);

            virtual GLuint getIndexBufferId(void);
            virtual GLuint getActiveIndexBuffer(void);
            virtual void setActiveIndexBuffer(GLuint);
    };
};
#endif // GLOBJECT_H
