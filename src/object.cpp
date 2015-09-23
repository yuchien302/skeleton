/*
 * object.cpp
 *
 *  Created on: Jan 2, 2015
 *      Author: nbingham
 */

#include "object.h"
#include "canvas.h"

rigidhdl::rigidhdl()
{

}

rigidhdl::~rigidhdl()
{

}

/* draw
 *
 * Draw a rigid body.
 */
void rigidhdl::draw(canvashdl *canvas)
{
	canvas -> draw_triangles(geometry, indices);
	// TODO Assignment 1: Send the rigid body geometry to the renderer
}

objecthdl::objecthdl()
{
	position = vec3f(0.0, 0.0, 0.0);
	orientation = vec3f(0.0, 0.0, 0.0);
	bound = vec6f(1.0e6, -1.0e6, 1.0e6, -1.0e6, 1.0e6, -1.0e6);
	scale = 1.0;
}

objecthdl::objecthdl(const objecthdl &o)
{
	position = o.position;
	orientation = o.orientation;
	bound = o.bound;
	scale = o.scale;
	rigid = o.rigid;
	for (map<string, materialhdl*>::const_iterator i = o.material.begin(); i != o.material.end(); i++)
		material.insert(pair<string, materialhdl*>(i->first, i->second->clone()));
}

objecthdl::~objecthdl()
{
	for (map<string, materialhdl*>::iterator i = material.begin(); i != material.end(); i++)
		if (i->second != NULL)
		{
			delete i->second;
			i->second = NULL;
		}

	material.clear();
}

/* draw
 *
 * Draw the model. Don't forget to apply the transformations necessary
 * for position, orientation, and scale.
 */
void objecthdl::draw(canvashdl *canvas)
{
	for (int i = 0; i < rigid.size(); i++){
		rigid[i].draw(canvas);
	}
	// TODO Assignment 1: Send transformations and geometry to the renderer to draw the object
	// TODO Assignment 3: Pass the material as a uniform into the renderer
}

/* draw_bound
 *
 * Create a representation for the bounding box and
 * render it.
 */
void objecthdl::draw_bound(canvashdl *canvas)
{
	/* TODO Assignment 1: Generate the geometry for the bounding box and send the necessary
	 * transformations and geometry to the renderer
	 */
	int bound_vertices_number = 6;
	int bound_y_index_offset = 2;
	int bound_z_index_offset = 2;
	vector<vec8f> points_cloud = new vector<vec8f>;
	for (int x = 0; x < 2; x++)
		for(int y = 0; y < 2; y++)
			for(int z = 0; z < 2; z++){
				points_cloud.push_back(vec8f(bound.data[x], bound[y+bound_y_index_offset], bound[z+bound_z_index_offset], 0.0, 0.0, 0.0, 0.0, 0.0);
				//canvas -> draw_lines(bound[i], bound[j]);
			}
	// TODO Assignment 3: clear the material in the uniform list
}

/* draw_normals
 *
 * create a representation of the normals for this object.
 * If face is false, render the vertex normals. Otherwise,
 * calculate the normals for each face and render those.
 */
void objecthdl::draw_normals(canvashdl *canvas, bool face)
{
	/* TODO Assignment 1: Generate the geometry to display the normals and send the necessary
	 * transformations and geometry to the renderer
	 */

	// TODO Assignment 3: clear the material in the uniform list before rendering
}
