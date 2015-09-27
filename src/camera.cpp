/*
 * camera.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: nbingham
 */

#include "camera.h"
#include "object.h"

camerahdl::camerahdl()
{
	position = vec3f(0.0, 0.0, 0.0);
	orientation = vec3f(0.0, 0.0, 0.0);
	model = NULL;
	type = "camera";
	focus = NULL;
	radius = 10.0f;
}

camerahdl::~camerahdl()
{

}

void camerahdl::view(canvashdl *canvas)
{
	/* (untested) Done Assignment 1: Do the necessary modelview transformations to move
	 * the camera into place.
	 */

//	vec3f up = vec3f(0.0, 1.0, 0.0);
//	vec3f eye = orientation;
//	vec3f at = position;
//	if(focus != NULL){
//		eye = focus -> position;
//	}
	vec3f up = ror3(vec3f(0.0, 1.0, 0.0), orientation);
	vec3f eye = position;
	vec3f at = eye + ror3(vec3f(0.0, 0.0, -1.0), orientation);
	if(focus != NULL){
		at = focus ->position;
		eye = at - ror3(vec3f(0.0, 0.0, -radius), orientation);
	}

	canvas -> look_at(eye, at, up);


}

orthohdl::orthohdl()
{
	left = -10.0;
	right = 10.0;
	bottom = -10.0;
	top = 10.0;
	front = 2.0;
	back = 101.0;
	type = "ortho";
}

orthohdl::~orthohdl()
{
}

void orthohdl::project(canvashdl *canvas)
{
	//  (untested) Done Assignment 1: Use the canvashdl::ortho function to set up an orthographic projection
	canvas -> set_matrix(canvashdl::projection_matrix);
	canvas -> load_identity();
	canvas -> ortho(left, right, bottom, top, front, back);
}

frustumhdl::frustumhdl()
{
	left = -1.0;
	right = 1.0;
	bottom = -1.0;
	top = 1.0;
	front = 2.0;
	back = 101.0;
	type = "frustum";
}

frustumhdl::~frustumhdl()
{

}

void frustumhdl::project(canvashdl *canvas)
{
	// (untested) Done Assignment 1: Use the canvashdl::frustum function to set up a perspective projection
	canvas -> set_matrix(canvashdl::projection_matrix);
	canvas -> load_identity();
	canvas -> frustum(left, right, bottom, top, front, back);
}

perspectivehdl::perspectivehdl()
{
	fovy = m_pi/4.0;
	aspect = 1.0;
	front = 2.0;
	back = 101.0;
	type = "perspective";
}

perspectivehdl::~perspectivehdl()
{

}

void perspectivehdl::project(canvashdl *canvas)
{
	//  (untested) Done Assignment 1: Use the canvashdl::perspective function to set up a perspective projection
	canvas -> set_matrix(canvashdl::projection_matrix);
	canvas -> load_identity();
	canvas -> perspective(fovy, aspect, front, back);
}
