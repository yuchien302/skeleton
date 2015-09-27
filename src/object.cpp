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
	// (untested) Done Assignment 1: Send the rigid body geometry to the renderer
	canvas -> draw_triangles(geometry, indices);
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
	// (untested) Done Assignment 1: Send transformations and geometry to the renderer to draw the object
	canvas -> set_matrix(canvashdl::modelview_matrix);

	canvas -> load_identity();

	for (int i = 0; i < rigid.size(); i++){
		canvas -> translate(position);
		rigid[i].draw(canvas);
		canvas -> translate(-position);
	}

	// TODO Assignment 3: Pass the material as a uniform into the renderer
}

/* draw_bound
 *
 * Create a representation for the bounding box and
 * render it.
 */
void objecthdl::draw_bound(canvashdl *canvas)
{

	canvas->set_matrix(canvashdl::modelview_matrix);
	canvas -> translate(position);

	int bound_vertices_number = 6;
	int bound_y_index_offset = 2;
	int bound_z_index_offset = 2;

	vector<vec8f> geometry = vector<vec8f>();
	vector<int> indices = vector<int>();

	for(int z = 0; z < 2; z++)
	for(int y = 0; y < 2; y++)
	for (int x = 0; x < 2; x++)
		geometry.push_back(vec8f(bound.data[x], bound[y+bound_y_index_offset], bound[z+bound_z_index_offset], 0.0, 0.0, 0.0, 0.0, 0.0));


	for(int i = 0; i < 4; i++){
		indices.push_back(i);
		indices.push_back((i+1)%4);
	}
	for(int i = 0; i < 4; i++){
		indices.push_back(i+4);
		indices.push_back(((i+1)%4) + 4);
	}
	for(int i = 0; i < 4; i++){
		indices.push_back(i);
		indices.push_back(i+4);
	}
	canvas -> draw_lines(geometry, indices);
	canvas -> translate(- position);
	/* TODO Assignment 1: Generate the geometry for the bounding box and send the necessary
	 * transformations and geometry to the renderer
	 */

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
	vector<vec8f> geometry = vector<vec8f>();
	vector<int> indices = vector<int>();
	int index = 0;
	for (int i = 0; i < rigid.size(); i++){
		if(face){
			//assert (rigid[i].geometry.size()%3 == 0);
			for(int n = 0; n < rigid[i].indices.size()/3; n++){
				vec3f point1 = vec3f(rigid[i].geometry[3*i].data[0], rigid[i].geometry[3*i][1], rigid[i].geometry[3*i][2]);
				vec3f point2 = vec3f(rigid[i].geometry[3*i+1].data[0], rigid[i].geometry[3*i+1][1], rigid[i].geometry[3*i+1][2]);
				vec3f point3 = vec3f(rigid[i].geometry[3*i+2].data[0], rigid[i].geometry[3*i+2][1], rigid[i].geometry[3*i+2][2]);
				vec3f vec12 = point2 - point1;
				vec3f vec13 = point3 - point1;
				vec3f direction = norm(cross(vec12, vec13));
				vec3f start = (point1+ point2+ point3) / float(3.0);
				geometry.push_back(start);
				geometry.push_back(start + direction);
				indices.push_back(index++);
				indices.push_back(index++);

			}
		}
		else{
			for(int g = 0; g < rigid[i].geometry.size(); i++){
				vec3f normal_start = vec3f(rigid[i].geometry[g].data[0], rigid[i].geometry[g].data[1], rigid[i].geometry[g].data[2]);
				vec3f normal_end = vec3f(rigid[i].geometry[g].data[0] + rigid[i].geometry[g].data[3],
										   rigid[i].geometry[g].data[1] + rigid[i].geometry[g].data[4],
										   rigid[i].geometry[g].data[2] + rigid[i].geometry[g].data[5]);
				geometry.push_back(normal_start);
				geometry.push_back(normal_end);
				indices.push_back(index++);
				indices.push_back(index++);
			}
		}
	}
	canvas -> draw_lines(geometry, indices);
	// TODO Assignment 3: clear the material in the uniform list before rendering
}
