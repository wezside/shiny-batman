#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <stdio.h>
#include <string.h>
#include <iostream>
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

        public:
            GLObject(std::string value = "default") : ActiveIndexBuffer(0), name(value) {}
            ~GLObject()
            {
                destroyVBO();
                destroyShaders();
            }

            virtual std::string getName();
            virtual void display(){};
            virtual void createVBO();
            virtual void destroyVBO();
            virtual void createShaders(void);
            virtual void destroyShaders(void);
            virtual void onKey(unsigned char key, int x, int y);

            virtual GLuint getIndexBufferId(void);
            virtual GLuint getActiveIndexBuffer(void);
            virtual void setActiveIndexBuffer(GLuint);
    };
};
#endif // GLOBJECT_H
