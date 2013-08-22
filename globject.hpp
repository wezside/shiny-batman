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

    

        protected:
            typedef struct
            {
                float XYZW[4];
                float RGBA[4];
                float UV[2];
            } Vertex;
            std::string m_name;
            GLuint
                vaoID,
                bufferID,
                programID,
                indexBufferID[2],
                activeIndexBuffer,
                samplerLoc,
                texCoordLoc,
                lerpLoc,
                projectionMatrixUniformLocation,
                viewMatrixUniformLocation,
                modelMatrixUniformLocation;

            GLUtils::Matrix modelMatrix,
                            projectionMatrix,
                            viewMatrix;


            GLUtils glUtil;
            int screenWidth, screenHeight;
            unsigned int currentFPS;
            bool m_isVBOCreated;

        public:
				GLObject(const GLObject& glo) {};
				GLObject& operator=(GLObject&) {};          
				GLObject(std::string value = "default") : 
                m_name(value),
                activeIndexBuffer(0), 
                modelMatrix(GLUtils::IDENTITY_MATRIX),
                projectionMatrix(GLUtils::IDENTITY_MATRIX),
                viewMatrix(GLUtils::IDENTITY_MATRIX),
                screenWidth(1024),
                screenHeight(576),
                m_isVBOCreated(0),
                programID(0){};
            virtual ~GLObject(){}
            virtual std::string getName();
            virtual void draw();
            virtual void update(){};
            virtual void createVBO();
            virtual void destroyVBO();
            virtual int start(){};

            virtual void updateScreen(int, int);
            virtual void resize(int, int);
            virtual void onKey(unsigned char, int, int);
            virtual void setFPS(unsigned int);
            virtual void setProgramID(GLuint i) 
            {
                programID = i;
            };
            virtual GLuint getProgramID() 
            {
                return programID;
            }
            virtual GLuint getActiveIndexBuffer(void);
            virtual void setActiveIndexBuffer(GLuint);
            virtual bool isVBOCreated() { return m_isVBOCreated;}
    };
};
#endif // GLOBJECT_H
