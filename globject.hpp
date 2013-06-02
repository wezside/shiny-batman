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
            typedef struct
            {
                float XYZW[4];
                float RGBA[4];
            } Vertex;
            std::string name;
            GLuint
                VertexShaderId,
                FragmentShaderId,
                ProgramId,
                VaoId,
                BufferId,
                IndexBufferId[2],
            	ActiveIndexBuffer;  

            const GLchar* VertexShader;
            const GLchar* FragmentShader;

        public:
            GLObject(std::string value = "default") : 
                ActiveIndexBuffer(0), 
                name(value), 
                VertexShader(
                "#version 330\n"\

                "layout(location=0) in vec4 in_Position;\n"\
                "layout(location=1) in vec4 in_Color;\n"\
                "out vec4 ex_Color;\n"\

                "void main(void)\n"\
                "{\n"\
                "   gl_Position = in_Position;\n"\
                "   ex_Color = in_Color;\n"\
                "}\n"),
                FragmentShader(
                "#version 330\n"\

                "in vec4 ex_Color;\n"\
                "out vec4 out_Color;\n"\

                "void main(void)\n"\
                "{\n"\
                "   out_Color = ex_Color;\n"\
                "}\n"                
                    ){};
            ~GLObject()
            {
                destroyVBO();
                destroyShaders();
            }

            virtual void createShader(const GLchar*, GLenum);
            virtual std::string getName();
            virtual void display(){};
            virtual void createVBO();
            virtual void destroyVBO();
            virtual void destroyShaders(void);
            virtual void loadShader(void);
            virtual void loadShader(const char*, GLenum);
            virtual void glslProgram();
            virtual void onKey(unsigned char, int, int);

            virtual GLuint getIndexBufferId(void);
            virtual GLuint getActiveIndexBuffer(void);
            virtual void setActiveIndexBuffer(GLuint);
    };
};
#endif // GLOBJECT_H
