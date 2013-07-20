#ifndef __GL_VIDEO__
#define __GL_VIDEO__

#include <stdlib.h>  
#include <time.h>
#include <boost/thread.hpp>
#include "globject.hpp"
#include "opencv2/opencv.hpp"

#define TEXTURE_SIZE 128
#define MAX_DEPTH 10000
#define DEFAULT_DISPLAY_MODE DISPLAY_MODE_IMAGE
#define MIN_NUM_CHUNKS(data_size, chunk_size)   (((data_size)-1) / (chunk_size) + 1)
#define MIN_CHUNKS_SIZE(data_size, chunk_size)  (MIN_NUM_CHUNKS(data_size, chunk_size) * (chunk_size))

namespace wezside
{
	class GLVideo : public GLObject
	{
		private:
			GLuint textureID;	
			std::vector<cv::Mat> m_frames;

			int m_currentFrame;

			unsigned int m_cols;
			unsigned int m_rows;
			unsigned int m_gridpos;
			unsigned int m_playcount;
			unsigned int m_nTexMapX;
			unsigned int m_nTexMapY;   	
			unsigned int m_nOriginalTexMapX;
			unsigned int m_nOriginalTexMapY;
			unsigned int m_video_fps;
			unsigned long int m_tStart;

			float m_width;
			float m_height;
			float m_quality;
			float m_fCropFill;
			float m_nOffsetX, m_nOffsetY;

			bool m_grab;
			bool m_autoplay;
			bool m_playing;
			bool m_loaded;
			bool m_firstFrameLoaded;
			bool m_isVBOCreated;
			bool m_bAutosize;

			boost::mutex guard;	
			boost::thread* thr;
			std::string m_name;

		public:
			GLVideo() 
			{
				m_tStart = 0;
				m_playing = 0;
				m_currentFrame = 0;
				m_nOffsetX = 0.0f;
				m_nOffsetY = 0.0f;
			};
			GLVideo(std::string video_fname, 
					float quality = 0.5,
					unsigned int width = 480,
					unsigned int height = 480,
					unsigned int cols = 5,
					unsigned int rows = 3) : 
								    m_quality(quality),
								    m_nOriginalTexMapX(width),
								    m_nOriginalTexMapY(height),
								    m_width(width),
								    m_height(height),
								    m_cols(cols),
								    m_rows(rows),
									m_playcount(0),
									m_fCropFill(0.0f),
									m_nOffsetX(0.0f),
									m_nOffsetY(0.0f),
									m_currentFrame(0),
									m_playing(0),
									m_tStart(0),
									m_loaded(0),
									m_gridpos(0),
									m_firstFrameLoaded(0),
									m_isVBOCreated(0),
									m_autoplay(0),
									m_bAutosize(1),
									m_name(video_fname),
									thr(NULL)
			{ 
				printf("[%s] %s %d:%d\n", m_name.c_str(), "GLVideo()", m_cols, m_rows);
				float texW = static_cast<float>(m_nOriginalTexMapX) * m_quality;
				float texH = static_cast<float>(m_nOriginalTexMapY) * m_quality;
				m_nTexMapX = static_cast<unsigned int>(texW);
				m_nTexMapY = static_cast<unsigned int>(texH);
				printf("[%s] Texture size %d x %d\n", m_name.c_str(), m_nTexMapX, m_nTexMapY);
			};

			// Copy constructor
			GLVideo(const GLVideo& glo) 
			{
				printf("%s\n", "[Copy constructor] GLVideo()");
			};

			// Copy Assignment operator
			GLVideo& operator=(GLVideo&) {};      									

			~GLVideo()
			{
				printf("[%s] %s %p\n", m_name.c_str(), "~GLVideo()", thr);
				// Ask thread to stop
				if (thr != NULL)
				{
					// Join - wait when thread actually exits
					thr->interrupt();
					thr->join();
					delete thr;
					thr = NULL;
					printf("[%s] %s\n", m_name.c_str(), "Thread destroyed.");
				}
				m_frames.clear();
			};

			int start(void);

			// Use only if you don't know the filename at construction
			int init(std::string video_fname)
			{
				m_name = video_fname;
				printf("%s%s\n", "GLVideo::init(std::string video_fname)", video_fname.c_str());
			};
			void createVBO();
			void destroyVBO();
			void draw();
            void update();
			void resize(int, int);
			void processFrame();

			// Setters
			void setSize(float w, float h)
			{
				m_width = w;
				m_height = h;
			}
			void setQuality(float fQuality) 
			{
				m_quality = fQuality;
				float texW = static_cast<float>(m_nOriginalTexMapX) * m_quality;
				float texH = static_cast<float>(m_nOriginalTexMapY) * m_quality;
				m_nTexMapX = static_cast<unsigned int>(texW);
				m_nTexMapY = static_cast<unsigned int>(texH);
			}
			void setGridPosition(int i) 
			{
				m_gridpos = static_cast<unsigned int>(i);
				m_nOffsetX = (m_gridpos % m_cols) * m_width;
				m_nOffsetY = (m_gridpos / m_cols) * (m_height + m_fCropFill);
				printf("[%s] Grid [%d] px Position %f:%f\n", m_name.c_str(), m_gridpos, m_nOffsetX, m_nOffsetY);
			}
			void setPlaycount(unsigned int i) {m_playcount = i;}
			void setAutoplay(bool ap) {m_autoplay = ap;}

			// Getters
			unsigned int getGridPosition() {return m_gridpos;}
			bool getAutoplay() {return m_autoplay;}
			std::string getName() {return m_name;}
			unsigned int getPlayCount() {return m_playcount;}

			bool isPlaying() {return m_playing;}
			bool isLoaded() {return m_loaded;}
			bool isFFLoaded() {return m_firstFrameLoaded;}
			bool isVBOCreated() { return m_isVBOCreated;}

			
	};
}
#endif //__GL_VIDEO__
