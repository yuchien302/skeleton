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
	//canvas
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
	// DONE Assignment 1: Send transformations and geometry to the renderer to draw the object
	canvas -> set_matrix(canvashdl::modelview_matrix);
	before_draw(canvas);
	for (int i = 0; i < rigid.size(); i++){
		// DONE Assignment 3: Pass the material as a uniform into the renderer
		canvas->uniform["current_material"] = material[rigid[i].material] ;
		rigid[i].draw(canvas);
	}
	after_draw(canvas);

}


/* draw_bound
 *
 * Create a representation for the bounding box and
 * render it.
 */
void objecthdl::draw_bound(canvashdl *canvas)
{
	/* DONE Assignment 1: Generate the geometry for the bounding box and send the necessary
	 * transformations and geometry to the renderer
	 */
	canvas->set_matrix(canvashdl::modelview_matrix);
	before_draw(canvas);

	vector<vec8f> geometry = vector<vec8f>();
	vector<int> indices = vector<int>();

	float xmin = bound.data[0];
	float xmax = bound.data[1];
	float ymin = bound.data[2];
	float ymax = bound.data[3];
	float zmin = bound.data[4];
	float zmax = bound.data[5];


	geometry.push_back(vec8f(xmax, ymax, zmax, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	geometry.push_back(vec8f(xmin, ymax, zmax, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	geometry.push_back(vec8f(xmin, ymin, zmax, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	geometry.push_back(vec8f(xmax, ymin, zmax, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	geometry.push_back(vec8f(xmax, ymax, zmin, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	geometry.push_back(vec8f(xmin, ymax, zmin, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	geometry.push_back(vec8f(xmin, ymin, zmin, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
	geometry.push_back(vec8f(xmax, ymin, zmin, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0));


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

	// DONE Assignment 3: clear the material in the uniform list
	//cout<<"bound fk"<<endl;
	canvas -> uniform.clear();
	canvas -> uniform["current_material"] = new whitehdl();
	//cout<<"bound fk"<<endl;
	canvas -> draw_lines(geometry, indices);
	after_draw(canvas);
	//cout<<"bound fk"<<endl;
	canvas -> uniform.clear();
	//cout<<"bound fk"<<endl;


}

/* draw_normals
 *
 * create a representation of the normals for this object.
 * If face is false, render the vertex normals. Otherwise,
 * calculate the normals for each face and render those.
 */
void objecthdl::draw_normals(canvashdl *canvas, bool face)
{
	/* DONE Assignment 1: Generate the geometry to display the normals and send the necessary
	 * transformations and geometry to the renderer
	 */
	vector<vec8f> geometry = vector<vec8f>();
	vector<int> indices = vector<int>();
	int index = 0;
	float normal_length = 0.1;

	for (int i = 0; i < rigid.size(); i++){
		if(face){
			//assert (rigid[i].geometry.size()%3 == 0);
			for(int n = 0; n < rigid[i].indices.size()/3; n++){

				vec3f point1 = vec3f(rigid[i].geometry[rigid[i].indices[3*n]].data[0], rigid[i].geometry[rigid[i].indices[3*n]].data[1], rigid[i].geometry[rigid[i].indices[3*n]].data[2]);
				vec3f point2 = vec3f(rigid[i].geometry[rigid[i].indices[3*n+1]].data[0], rigid[i].geometry[rigid[i].indices[3*n+1]].data[1], rigid[i].geometry[rigid[i].indices[3*n+1]].data[2]);
				vec3f point3 = vec3f(rigid[i].geometry[rigid[i].indices[3*n+2]].data[0], rigid[i].geometry[rigid[i].indices[3*n+2]].data[1], rigid[i].geometry[rigid[i].indices[3*n+2]].data[2]);
				vec3f vec12 = point2 - point1;
				vec3f vec13 = point3 - point1;
				vec3f direction = norm(cross(vec13, vec12));
				vec3f start = (point1+ point2+ point3) / float(3.0);
				geometry.push_back(start);
				geometry.push_back(start + ( normal_length * direction));

				indices.push_back(index++);
				indices.push_back(index++);
			}
		}
		else{
			for(int g = 0; g < rigid[i].geometry.size(); g++){

				vec3f start = vec3f(rigid[i].geometry[g].data[0], rigid[i].geometry[g].data[1], rigid[i].geometry[g].data[2]);

				vec3f direction = vec3f(rigid[i].geometry[g].data[3], rigid[i].geometry[g].data[4], rigid[i].geometry[g].data[5]);

				geometry.push_back(start);
				geometry.push_back(start + ( normal_length * direction));
				
				indices.push_back(index++);
				indices.push_back(index++);
			}
		}
	}
	before_draw(canvas);
	// DONE Assignment 3: clear the material in the uniform list
	canvas -> uniform.clear();
	canvas -> uniform["current_material"] = new whitehdl();
	canvas -> draw_lines(geometry, indices);
	after_draw(canvas);
	canvas -> uniform.clear();

}

void objecthdl::before_draw(canvashdl *canvas){
	canvas -> translate(position);

	canvas -> rotate(orientation[0], vec3f(1.0, 0.0, 0.0));
	canvas -> rotate(orientation[1], vec3f(0.0, 1.0, 0.0));
	canvas -> rotate(orientation[2], vec3f(0.0, 0.0, 1.0));

	canvas -> scale(vec3f(scale, scale, scale));
}


void objecthdl::after_draw(canvashdl *canvas){
	canvas -> scale(vec3f(1.0/scale, 1.0/scale, 1.0/scale));

	canvas -> rotate(-orientation[2], vec3f(0.0, 0.0, 1.0));
	canvas -> rotate(-orientation[1], vec3f(0.0, 1.0, 0.0));
	canvas -> rotate(-orientation[0], vec3f(1.0, 0.0, 0.0));

	canvas -> translate(-position);
}


