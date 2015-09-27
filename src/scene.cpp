/*
 * scene.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: nbingham
 */

#include "scene.h"
#include "camera.h"
#include "object.h"
#include "light.h"

#include "primitive.h"
#include "model.h"

scenehdl::scenehdl()
{
	canvas = NULL;
	active_camera = -1;
	active_object = -1;
	render_normals = none;
	render_lights = false;
	render_cameras = false;
}

scenehdl::~scenehdl()
{

}

/* draw
 *
 * Update the locations of all of the lights, draw all of the objects, and
 * if enabled, draw the normals, the lights, the cameras, etc.
 */
void scenehdl::draw()
{
	/* (untested) Done Assignment 1: Draw all of the objects, and
	 * if enabled, draw the normals and the cameras.
	 */
	canvas -> set_matrix(canvashdl::projection_matrix);
	canvas -> load_identity();
	cameras[active_camera] -> project(canvas);

	canvas -> set_matrix(canvashdl::modelview_matrix);
	canvas -> load_identity();

	cameras[active_camera] -> view(canvas);



	for (int i = 0; i < objects.size(); i++){
		bool is_camera = false;
		for (int j = 0; j < cameras.size() && !is_camera; j++){
			if (cameras[j] != NULL && cameras[j]->model == objects[i]){
				is_camera = true;
			}
		}
		if(!is_camera){
			objects[i] -> draw(canvas);
			if(render_normals)
				objects[i] -> draw_normals(canvas, render_normals==scenehdl::face);
			if(i == active_object){
				objects[i] -> draw_bound(canvas);
			}
		}
	}





	/* TODO Assignment 3: Clear the uniform variables and pass the vector of
	 * lights into the renderer as a uniform variable.
	 * TODO Assignment 3: Update the light positions and directions
	 * TODO Assignment 3: Render the lights
	 */
}

bool scenehdl::active_camera_valid()
{
	return (active_camera >= 0 && active_camera < cameras.size() && cameras[active_camera] != NULL);
}

bool scenehdl::active_object_valid()
{
	return (active_object >= 0 && active_object < objects.size() && objects[active_object] != NULL);
}
