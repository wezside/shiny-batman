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
            GLObject( std::string value = "default" );
            ~GLObject();

            std::string getName();
            void createVBO();
            void destroyVBO();
            void createShaders( void );
            void destroyShaders( void );

            GLuint getIndexBufferId( void );
            GLuint getActiveIndexBuffer( void );
            void setActiveIndexBuffer( GLuint );
    };
};
#endif // GLOBJECT_H
