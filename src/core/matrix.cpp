/*
 * matrix.cpp
 *
 *  Created on: Nov 10, 2015
 *      Author: nbingham
 */

#include "matrix.h"

namespace core
{

mat<float, 4, 4> rotate_matrix(float angle, vec<float, 3> axis)
{
	float x = axis[0];
	float y = axis[1];
	float z = axis[2];
	float c = cos(angle);
	float s = sin(angle);

	return mat<float, 4, 4>(x*x*(1-c) + c  , x*y*(1-c) - z*s, x*z*(1-c) + y*s, 0.0f,
							y*x*(1-c) + z*s, y*y*(1-c) + c  , y*z*(1-c) - x*s, 0.0f,
							x*z*(1-c) - y*s, y*z*(1-c) + x*s, z*z*(1-c) + c  , 0.0f,
							0.0f           , 0.0f           , 0.0f           , 1.0f);
}

/* translate
 *
 * Multiply the active matrix by a translation matrix.
 * This implements: https://www.opengl.org/sdk/docs/man2/xhtml/glTranslate.xml
 */
mat<float, 4, 4> translate_matrix(vec<float, 3> direction)
{
	return mat<float, 4, 4>(1.0, 0.0, 0.0, direction[0],
				 	 	 	0.0, 1.0, 0.0, direction[1],
							0.0, 0.0, 1.0, direction[2],
							0.0, 0.0, 0.0, 1.0);
}

/* scale
 *
 * Multiply the active matrix by a scaling matrix.
 * This implements: https://www.opengl.org/sdk/docs/man2/xhtml/glScale.xml
 */
mat<float, 4, 4> scale_matrix(vec<float, 3> size)
{
	return mat<float, 4, 4>(size[0], 0.0, 0.0, 0.0,
				 	 	 	0.0, size[1], 0.0, 0.0,
							0.0, 0.0, size[2], 0.0,
							0.0, 0.0, 0.0, 1.0);
}

}
