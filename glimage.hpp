#ifndef __GL_PARTICLESYSTEM__
#define __GL_PARTICLESYSTEM__

#include "globject.hpp"
#include "opencv2/opencv.hpp"

#define NUM_PARTICLES 1000

namespace wezside
{
	class GLImage : public GLObject
	{
		private:
			float m_time;
			float m_color[4];

			unsigned int m_nTexMapX;
			unsigned int m_nTexMapY;   			

			cv::Mat m_tex;

			typedef struct ParticleVertex
			{
				float ABCD[4]; // Start
				float XYZW[4]; // End
				float RGBA[4];
				float LIFE;
				float VELOCITY;
			} ParticleVertex;				
			
			GLuint textureID, 
				   timeLoc, 
				   centerPosLoc,
				   waveTimeLoc,
				   waveWidthLoc,
				   waveHeightLoc;

			GLfloat waveTime,
					waveWidth,
					waveHeight,
					waveFreq;

		public:
			// Default constructor
			GLImage() : m_time(0.0f),
								 m_nTexMapX(4),
								 m_nTexMapY(4),
								 waveTime(0.0),
								 waveWidth(0.1),
								 waveHeight(0.5),
								 waveFreq(0.001)
								 {};

			
			// Copy constructor
			GLImage(const GLImage& glo) {};

			// Copy Assignment operator
			GLImage& operator=(GLImage&) {};

			// Destructor
			~GLImage() {};

			void createVBO();
			void destroyVBO();
			void draw();
            void update();
			void resize(int, int);
	};
}
#endif // __GL_PARTICLESYSTEM__