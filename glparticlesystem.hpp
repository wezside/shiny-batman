#ifndef __GL_PARTICLESYSTEM__
#define __GL_PARTICLESYSTEM__

#include "globject.hpp"

#define NUM_PARTICLES 1000

namespace wezside
{
	class GLParticleSystem : public GLObject
	{
		private:
			float m_time;
			float m_color[4];

			unsigned int m_nTexMapX;
			unsigned int m_nTexMapY;   			

			GLuint textureID, timeLoc, centerPosLoc;
			typedef struct ParticleVertex
			{
				float ABCD[4]; // Start
				float XYZW[4]; // End
				float RGBA[4];
				float LIFE;
			} ParticleVertex;				

		public:
			// Default constructor
			GLParticleSystem() : m_time(0.0f),
								 m_nTexMapX(4),
								 m_nTexMapY(4)
								 {};

			
			// Copy constructor
			GLParticleSystem(const GLParticleSystem& glo) {};

			// Copy Assignment operator
			GLParticleSystem& operator=(GLParticleSystem&) {};

			// Destructor
			~GLParticleSystem() {};

			void createVBO();
			void destroyVBO();
			void draw();
            void update();
			void resize(int, int);
	};
}
#endif // __GL_PARTICLESYSTEM__