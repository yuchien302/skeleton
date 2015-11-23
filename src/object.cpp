/*
 * object.cpp
 *
 *  Created on: Jan 2, 2015
 *      Author: nbingham
 */

#include "object.h"

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
void rigidhdl::draw()
{
	// Done Assignment 1: Send the rigid body geometry to the renderer

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(float)*8, (float*) geometry.data());
	glNormalPointer(GL_FLOAT, sizeof(float)*8, (float*) geometry.data()+3);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float)*8, (float*) geometry.data()+6);

	glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, indices.data());
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

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
void objecthdl::draw(const vector<lighthdl*> &lights)
{
	// DONE Assignment 1: Send transformations and geometry to the renderer to draw the object
	// DONE Assignment 3: Pass the material as a uniform into the renderer
	glMatrixMode(GL_MODELVIEW);
	before_draw();
	for (int i = 0; i < rigid.size(); i++){
		// DONE Assignment 3: Pass the material as a uniform into the renderer
		material[rigid[i].material] -> apply(lights);
		rigid[i].draw();
	}
	after_draw();
}


/* draw_bound
 *
 * Create a representation for the bounding box and
 * render it.
 */
void objecthdl::draw_bound()
{
	/* DONE Assignment 1: Generate the geometry for the bounding box and send the necessary
	 * transformations and geometry to the renderer
	 */

	vector<vec3f> vertices = vector<vec3f>();
	vector<int> indices = vector<int>();

	float xmin = bound.data[0];
	float xmax = bound.data[1];
	float ymin = bound.data[2];
	float ymax = bound.data[3];
	float zmin = bound.data[4];
	float zmax = bound.data[5];


	vertices.push_back(vec3f(xmax, ymax, zmax));
	vertices.push_back(vec3f(xmin, ymax, zmax));
	vertices.push_back(vec3f(xmin, ymin, zmax));
	vertices.push_back(vec3f(xmax, ymin, zmax));
	vertices.push_back(vec3f(xmax, ymax, zmin));
	vertices.push_back(vec3f(xmin, ymax, zmin));
	vertices.push_back(vec3f(xmin, ymin, zmin));
	vertices.push_back(vec3f(xmax, ymin, zmin));


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

	glMatrixMode(GL_MODELVIEW);
	before_draw();

	materialhdl* m = new whitehdl();
	m -> apply(vector<lighthdl*>());

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*3, vertices.data());
	glDrawElements(GL_LINES, (int)indices.size(), GL_UNSIGNED_INT, indices.data());
	glDisableClientState(GL_VERTEX_ARRAY);
	glUseProgram(0);

	after_draw();

}

/* draw_normals
 *
 * create a representation of the normals for this object.
 * If face is false, render the vertex normals. Otherwise,
 * calculate the normals for each face and render those.
 */
void objecthdl::draw_normals(bool face)
{
	/* DONE Assignment 1: Generate the geometry to display the normals and send the necessary
	 * transformations and geometry to the renderer
	 */
	vector<vec3f> normals = vector<vec3f>();
	vector<int> indices = vector<int>();
	int index = 0;
	float normal_length = 0.1;

	for (int i = 0; i < rigid.size(); i++){
		if(face){
			for(int n = 0; n < rigid[i].indices.size()/3; n++){

				vec3f point1 = vec3f(rigid[i].geometry[rigid[i].indices[3*n]].data[0], rigid[i].geometry[rigid[i].indices[3*n]].data[1], rigid[i].geometry[rigid[i].indices[3*n]].data[2]);
				vec3f point2 = vec3f(rigid[i].geometry[rigid[i].indices[3*n+1]].data[0], rigid[i].geometry[rigid[i].indices[3*n+1]].data[1], rigid[i].geometry[rigid[i].indices[3*n+1]].data[2]);
				vec3f point3 = vec3f(rigid[i].geometry[rigid[i].indices[3*n+2]].data[0], rigid[i].geometry[rigid[i].indices[3*n+2]].data[1], rigid[i].geometry[rigid[i].indices[3*n+2]].data[2]);
				vec3f vec12 = point2 - point1;
				vec3f vec13 = point3 - point1;
				vec3f direction = norm(cross(vec13, vec12));
				vec3f start = (point1+ point2+ point3) / float(3.0);
				normals.push_back(start);
				normals.push_back(start + ( normal_length * direction));

				indices.push_back(index++);
				indices.push_back(index++);
			}
		}
		else{
			for(int g = 0; g < rigid[i].geometry.size(); g++){

				vec3f start = vec3f(rigid[i].geometry[g].data[0], rigid[i].geometry[g].data[1], rigid[i].geometry[g].data[2]);

				vec3f direction = vec3f(rigid[i].geometry[g].data[3], rigid[i].geometry[g].data[4], rigid[i].geometry[g].data[5]);

				normals.push_back(start);
				normals.push_back(start + ( normal_length * direction));

				indices.push_back(index++);
				indices.push_back(index++);
			}
		}
	}

	glMatrixMode(GL_MODELVIEW);
	before_draw();

	materialhdl* m = new whitehdl();
	m -> apply(vector<lighthdl*>());

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(GLfloat)*3, normals.data());
	glDrawElements(GL_LINES, (int)indices.size(), GL_UNSIGNED_INT, indices.data());
	glDisableClientState(GL_VERTEX_ARRAY);
	glUseProgram(0);

	after_draw();
}

void objecthdl::before_draw(){
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	glRotatef(radtodeg(orientation[0]), 1.0, 0.0, 0.0);
	glRotatef(radtodeg(orientation[1]), 0.0, 1.0, 0.0);
	glRotatef(radtodeg(orientation[2]), 0.0, 0.0, 1.0);
	glScalef(scale, scale, scale);
}

void objecthdl::after_draw(){
	glPopMatrix();
}


