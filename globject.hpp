#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glutils.hpp"

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
                float UV[2];
            } Vertex;
            std::string name;
            GLuint
                vertexShaderId,
                fragmentShaderId,
                programID,
                vaoID,
                bufferID,
                indexBufferID[2],
                activeIndexBuffer,
                samplerLoc,
                texCoordLoc,
                projectionMatrixUniformLocation,
                viewMatrixUniformLocation,
                modelMatrixUniformLocation;

            GLUtils::Matrix modelMatrix,
                            projectionMatrix,
                            viewMatrix;

            const GLchar* vertexShader;
            const GLchar* fragmentShader;
            GLUtils glUtil;
            int screenWidth, screenHeight;

        public:
            GLObject(std::string value = "default") : 
                name(value), 
                activeIndexBuffer(0), 
                modelMatrix(GLUtils::IDENTITY_MATRIX),
                projectionMatrix(GLUtils::IDENTITY_MATRIX),
                viewMatrix(GLUtils::IDENTITY_MATRIX),
                vertexShader(
                "#version 330\n"\

                "layout(location=0) in vec4 in_Position;\n"\
                "layout(location=1) in vec4 in_Color;\n"\
                "out vec4 ex_Color;\n"\
                "uniform mat4 modelMatrix;\n"\
                "uniform mat4 viewMatrix;\n"\
                "uniform mat4 projectionMatrix;\n"\
                "void main(void)\n"\
                "{\n"\
                "   gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * in_Position;\n"\
                "   ex_Color = in_Color;\n"\
                "}\n"),
                fragmentShader(
                "#version 330\n"\

                "in vec4 ex_Color;\n"\
                "out vec4 out_Color;\n"\

                "void main(void)\n"\
                "{\n"\
                "   out_Color = ex_Color;\n"\
                "}\n"),
                screenWidth(1024),
                screenHeight(576){};
            ~GLObject()
            {
                destroyVBO();
                destroyShaders();
            }
            virtual void createShader(const GLchar*, GLenum);
            virtual std::string getName();
            virtual void draw();
            virtual void update(){};
            virtual void createVBO();
            virtual void destroyVBO();
            virtual void destroyShaders(void);
            virtual void loadShader(void);
            virtual void loadShader(const char*, GLenum);
            virtual void glslProgram();
            virtual void resize(int, int);
            virtual void onKey(unsigned char, int, int);

            virtual GLuint getActiveIndexBuffer(void);
            virtual void setActiveIndexBuffer(GLuint);
    };
};
#endif // GLOBJECT_H
