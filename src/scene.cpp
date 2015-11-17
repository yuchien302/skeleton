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
	active_camera = -1;
	active_object = -1;
	render_normals = none;
	render_lights = false;
	render_cameras = false;
}

scenehdl::~scenehdl()
{
	for (int i = 0; i < (int)objects.size(); i++)
		delete objects[i];
	objects.clear();

	for (int i = 0; i < (int)cameras.size(); i++)
		delete cameras[i];
	cameras.clear();

	for (int i = 0; i < (int)lights.size(); i++)
		delete lights[i];
	lights.clear();
}

/* draw
 *
 * Update the locations of all of the lights, draw all of the objects, and
 * if enabled, draw the normals, the lights, the cameras, etc.
 */
void scenehdl::draw()
{
	/* DONE Assignment 1: Draw all of the objects, and
	 * if enabled, draw the normals and the cameras.
	 */
	cameras[active_camera] -> project();
	cameras[active_camera] -> view();


	// DONE Assignment 3: Update the light positions and directions
	for (int i=0; i < lights.size(); i++){
		lights[i] -> update();
	}

	for (int i = 0; i < objects.size(); i++) {

		bool is_camera = false;
		bool is_light = false;

		// DONE Assignment 3: Render the lights;

		for (int j = 0; j < cameras.size(); j++){
			if (cameras[j] != NULL && cameras[j] -> model == objects[i]){
				is_camera = true;
				break;

			}
		}

		for (int j = 0; j < lights.size(); j++){
			if (lights[j] != NULL && lights[j] -> model == objects[i]){
				is_light = true;
				break;

			}
		}

		if( (!is_camera && !is_light) || (is_camera && render_cameras) || (is_light && render_lights) ){
			objects[i] -> draw(lights);

			if(render_normals)
				objects[i] -> draw_normals(render_normals==scenehdl::face);

			if(i == active_object)
				objects[i] -> draw_bound();
		}
	}

}

bool scenehdl::active_camera_valid()
{
	return (active_camera >= 0 && active_camera < cameras.size() && cameras[active_camera] != NULL);
}

bool scenehdl::active_object_valid()
{
	return (active_object >= 0 && active_object < objects.size() && objects[active_object] != NULL);
}
