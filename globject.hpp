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

            std::string getName();
            void display();
            void createVBO();
            void destroyVBO();
            void createShaders(void);
            void destroyShaders(void);

            GLuint getIndexBufferId(void);
            GLuint getActiveIndexBuffer(void);
            void setActiveIndexBuffer(GLuint);
    };
};
#endif // GLOBJECT_H
