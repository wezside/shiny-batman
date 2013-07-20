#ifndef __GL_UTILS_H__
#define __GL_UTILS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

namespace wezside
{
	class GLUtils
	{
	private:
		
	public:
		typedef struct Matrix
		{
			float m[16];
		} Matrix;
		static const double PI = 3.14159265358979323846;
		static const Matrix IDENTITY_MATRIX;

		float cotangent(float angle);
		float degreesToRadians(float degrees);
		float radiansToDegrees(float radians);
		Matrix multiplyMatrices(const Matrix* m1, const Matrix* m2);
		void scaleMatrix(Matrix* m, float x, float y, float z);
		void translateMatrix(Matrix* m, float x, float y, float z);
		void rotateAboutX(Matrix* m, float angle);
		void rotateAboutY(Matrix* m, float angle);
		void rotateAboutZ(Matrix* m, float angle);
		Matrix createProjectionMatrix(
			float fovy,
			float aspect_ratio,
			float near_plane,
			float far_plane
		);
		Matrix createOrthogonalMatrix(
			float near,
			float far,
			float left,
			float right,
			float bottom,
			float top
		);
		int exitOnGLError(const char* error_message);

	};
};
#endif // __GL_UTILS_H__