#include "glutils.hpp"

const wezside::GLUtils::Matrix wezside::GLUtils::IDENTITY_MATRIX = {{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
}};

float wezside::GLUtils::cotangent(float angle)
{
	return (float)(1.0 / tan(angle));
}

float wezside::GLUtils::degreesToRadians(float degrees)
{
	return degrees * (float)(PI / 180);
}

float wezside::GLUtils::radiansToDegrees(float radians)
{
	return radians * (float)(180 / PI);
}

wezside::GLUtils::Matrix wezside::GLUtils::multiplyMatrices(const Matrix* m1, const Matrix* m2)
{
	wezside::GLUtils::Matrix out = GLUtils::IDENTITY_MATRIX;
	unsigned int row, column, row_offset;

	for (row = 0, row_offset = row * 4; row < 4; ++row, row_offset = row * 4)
		for (column = 0; column < 4; ++column)
			out.m[row_offset + column] =
				(m1->m[row_offset + 0] * m2->m[column + 0]) +
				(m1->m[row_offset + 1] * m2->m[column + 4]) +
				(m1->m[row_offset + 2] * m2->m[column + 8]) +
				(m1->m[row_offset + 3] * m2->m[column + 12]);

	return out;
}

void wezside::GLUtils::scaleMatrix(Matrix* m, float x, float y, float z)
{
	Matrix scale = IDENTITY_MATRIX;

	scale.m[0] = x;
	scale.m[5] = y;
	scale.m[10] = z;

	memcpy(m->m, multiplyMatrices(m, &scale).m, sizeof(m->m));
}

void wezside::GLUtils::translateMatrix(Matrix* m, float x, float y, float z)
{
	Matrix translation = IDENTITY_MATRIX;
	
	translation.m[12] = x;
	translation.m[13] = y;
	translation.m[14] = z;

	memcpy(m->m, multiplyMatrices(m, &translation).m, sizeof(m->m));
}

void wezside::GLUtils::rotateAboutX(Matrix* m, float angle)
{
	Matrix rotation = IDENTITY_MATRIX;
	float sine = (float)sin(angle);
	float cosine = (float)cos(angle);
	
	rotation.m[5] = cosine;
	rotation.m[6] = -sine;
	rotation.m[9] = sine;
	rotation.m[10] = cosine;

	memcpy(m->m, multiplyMatrices(m, &rotation).m, sizeof(m->m));
}

void wezside::GLUtils::rotateAboutY(Matrix* m, float angle)
{
	Matrix rotation = IDENTITY_MATRIX;
	float sine = (float)sin(angle);
	float cosine = (float)cos(angle);
	
	rotation.m[0] = cosine;
	rotation.m[8] = sine;
	rotation.m[2] = -sine;
	rotation.m[10] = cosine;

	memcpy(m->m, multiplyMatrices(m, &rotation).m, sizeof(m->m));
}

void wezside::GLUtils::rotateAboutZ(Matrix* m, float angle)
{
	Matrix rotation = IDENTITY_MATRIX;
	float sine = (float)sin(angle);
	float cosine = (float)cos(angle);
	
	rotation.m[0] = cosine;
	rotation.m[1] = -sine;
	rotation.m[4] = sine;
	rotation.m[5] = cosine;

	memcpy(m->m, multiplyMatrices(m, &rotation).m, sizeof(m->m));
}

wezside::GLUtils::Matrix wezside::GLUtils::createProjectionMatrix(
	float fovy,
	float aspect_ratio,
	float near_plane,
	float far_plane
)
{
	Matrix out = {{ 0 }};

	const float
		y_scale = cotangent(degreesToRadians(fovy / 2)),
		x_scale = y_scale / aspect_ratio,
		frustum_length = far_plane - near_plane;

	out.m[0] = x_scale;
	out.m[5] = y_scale;
	out.m[10] = -((far_plane + near_plane) / frustum_length);
	out.m[11] = -1;
	out.m[14] = -((2 * near_plane * far_plane) / frustum_length);
	
	return out;
}

wezside::GLUtils::Matrix wezside::GLUtils::createOrthogonalMatrix(
	float near,
	float far,
	float left,
	float right,
	float bottom,
	float top
	)
{
	float a = 2.0f / (right - left);
    float b = 2.0f / (top - bottom);
    float c = -2.0f / (far - near);

    float tx = - (right + left)/(right - left);
    float ty = - (top + bottom)/(top - bottom);
    float tz = - (far + near)/(far - near);

    Matrix out = {{
		a, 0, 0, 0,
		0, b, 0, 0,
		0, 0, c, 0,
		tx, ty, tz, 1
    }};

	// These paramaters are lens properties.
    // The "near" and "far" create the Depth of Field.
    // The "left", "right", "bottom" and "top" represent the rectangle formed
    // by the near area, this rectangle will also be the size of the visible area.
     
    // First Column 0, 4, 8, 12
/*    out.m[0] = 2.0 / (right - left);
    out.m[4] = 0.0;
    out.m[8] = 0.0;
    out.m[12] = 0.0;
 
    // Second Column 1,5,9,13
    out.m[1] = 0.0;
    out.m[5] = 2.0 / (top - bottom);
    out.m[9] = 0.0;
    out.m[13] = 0.0;
 
    // Third Column 2,6,10,14
    out.m[2] = 0.0;
    out.m[6] = 0.0;
    out.m[10] = -2.0/(far - near);
    out.m[14] = 0.0;
 
    // Fourth Column 3,7,11,15
    out.m[3] = -(right + left) / (right - left);
    out.m[7] = -(top + bottom) / (top - bottom);
    out.m[11] = -(far + near) / (far - near);
    out.m[15] = 1.0;	*/

    return out;
}

void wezside::GLUtils::exitOnGLError(const char* error_message)
{
	const GLenum ErrorValue = glGetError();

	if (ErrorValue != GL_NO_ERROR)
	{
		const char* APPEND_DETAIL_STRING = ": %s\n";
		const size_t APPEND_LENGTH = strlen(APPEND_DETAIL_STRING) + 1;
		const size_t message_length = strlen(error_message);
		char* display_message = (char*)malloc(message_length + APPEND_LENGTH);

		memcpy(display_message, error_message, message_length);
		memcpy(&display_message[message_length], APPEND_DETAIL_STRING, APPEND_LENGTH);

		fprintf(stderr, display_message, gluErrorString(ErrorValue));

		free(display_message);
		exit(EXIT_FAILURE);
	}
}
