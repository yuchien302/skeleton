/*
 * object.cpp
 *
 *  Created on: Jan 2, 2015
 *      Author: nbingham
 */

#include "object.h"
#include <assert.h>

rigidhdl::rigidhdl()
{
}

rigidhdl::~rigidhdl()
{

}

vec3f rigidhdl::get_position(int frame, double pos, double fraction, double step, int method)
{
	if (frame >= positions.size() || positions[frame].size() == 0)
		return vec3f(0.0f, 0.0f, 0.0f);
	else if (pos < positions[frame].begin()->first)
		return positions[frame].begin()->second;

	if (method == 0) // none
	{
		// TODO Assignment 5: implement position frame sampling
		return positions[frame][pos];
	}
	else if (method == 1) // lerp
	{
		// TODO Assignment 5: use linear interpolation between position frames
	}
	else if (method == 2) // hermite
	{
		// TODO Assignment 5: use hermite interpolation between position frames
	}
	else if (method == 3) // catmull rom
	{
		// TODO Assignment 5: use catmull rom interpolation between position frames
	}
	// TODO Assignment 5: try out any other interpolation methods that sound interesting to you

	return positions[frame].begin()->second;
}

vec4d rigidhdl::get_orientation(int frame, double pos, double fraction, double step, int method)
{
	if (frame >= orientations.size() || orientations[frame].size() == 0)
		return vec4d(0.0f, 0.0f, 1.0f, 0.0f);
	else if (pos < orientations[frame].begin()->first)
		return orientations[frame].begin()->second;

	if (method == 0) // none
	{
		// TODO Assignment 5: implement orientation frame sampling
		return orientations[frame][pos];
	}
	if (method == 1) // lerp
	{
		// TODO Assignment 5: use linear interpolation between orientation frames
	}
	else if (method == 2) // slerp
	{
		// TODO Assignment 5: use spherical linear interpolation between orientation frames
	}
	else if (method == 3) // squad
	{
		// TODO Assignment 5: use spherical quadratic interpolation between orientation frames
	}
	// TODO Assignment 5: try out any other interpolation methods that sound interesting to you

	return orientations[frame].begin()->second;
}

/* draw
 *
 * Draw a rigid body.
 */
void rigidhdl::draw(double pos, double fraction, double step, int position_interpolator, int orientation_interpolator)
{
	glPushMatrix();
	assert(name.size() == positions.size());
	assert(name.size() == center.size());
	assert(name.size() == scale.size());
	assert(name.size() == scale_orientation.size());

	for (int i = 0; i < (int)name.size(); i++)
	{
		if (i < (int)positions.size())
		{
			vec3f position = get_position(i, pos, fraction, step, position_interpolator);
			glTranslatef(position[0], position[1], position[2]);
		}

		if (i < (int)center.size())
			glTranslatef(center[i][0], center[i][1], center[i][2]);

		if (i < (int)orientations.size())
		{
			vec4d orientation = get_orientation(i, pos, fraction, step, orientation_interpolator);
			glRotatef(radtodeg(orientation[3]), orientation[0], orientation[1], orientation[2]);
		}

		if (i < (int)scale_orientation.size())
			glRotatef(radtodeg(scale_orientation[i][3]), scale_orientation[i][0], scale_orientation[i][1], scale_orientation[i][2]);

		if (i < (int)scale.size())
			glScalef(scale[i][0], scale[i][1], scale[i][2]);

		if (i < (int)scale_orientation.size())
			glRotatef(radtodeg(-scale_orientation[i][3]), scale_orientation[i][0], scale_orientation[i][1], scale_orientation[i][2]);

		if (i < (int)center.size())
			glTranslatef(-center[i][0], -center[i][1], -center[i][2]);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(float)*8, ((float*)geometry.data()));
	glNormalPointer(GL_FLOAT, sizeof(float)*8, ((float*)geometry.data()) + 3);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float)*8, ((float*)geometry.data()) + 6);

	glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, indices.data());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}

objecthdl::objecthdl()
{
	position = vec3f(0.0f, 0.0f, 0.0f);
	orientation = vec3f(0.0f, 0.0f, 0.0f);
	bound = vec6f(1.0e6f, -1.0e6f, 1.0e6f, -1.0e6f, 1.0e6f, -1.0e6f);
	scale = 1.0;
	start_time = 0.0f;
	minstep = 0.0;
	animation_length = 1.0;
	position_interpolator = 0;
	orientation_interpolator = 0;
}

objecthdl::objecthdl(const objecthdl &o)
{
	animation_length = o.animation_length;
	minstep = o.minstep;
	position = o.position;
	orientation = o.orientation;
	bound = o.bound;
	scale = o.scale;
	rigid = o.rigid;
	start_time = o.start_time;
	position_interpolator = o.position_interpolator;
	orientation_interpolator = o.orientation_interpolator;
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
	/* TODO Assignment 5: get the current time and use that to calculate the pos and fraction values and to update
	 * the start_time.
	 *
	 * Here are the variables you'll need to work with. They are all member variables of objecthdl.
	 *
	 * start_time		is the time at which the current animation was started in seconds. This needs to be updated
	 * 					every time the animation finishes in order to start a new animation.
	 * animation_length is the total length of the animation in seconds. This is given by the *.wrl model file
	 * minstep 			is the minimum step size in sections between two frames in seconds. This is given by the GUI.
	 *
	 * pos		is a multiple of minstep/animation_length that specifies the current location in
	 * 			the animation as a percentage of animation_length.
	 * fraction is the fractional part of the current location in the animation. Its an
	 * 			interpolator with a value between 0.0 and 1.0 where 1.0 is the next frame.
	 */


	double pos = 0.0;
	double fraction = 0.0;
	double step = 0.0;
	if(start_time >= animation_length) start_time = 0.0;
	start_time += 1;
	pos = start_time * (minstep/animation_length);

	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	glRotatef(radtodeg(orientation[0]), 1.0, 0.0, 0.0);
	glRotatef(radtodeg(orientation[1]), 0.0, 1.0, 0.0);
	glRotatef(radtodeg(orientation[2]), 0.0, 0.0, 1.0);
	glScalef(scale, scale, scale);

	for (int i = 0; i < rigid.size(); i++)
	{

		if (material.find(rigid[i].material) != material.end())
			material[rigid[i].material]->apply(lights);
		else {
			whitehdl().apply(lights);
		}

		// The first three numbers here are pos, fraction, and step
//		rigid[i].draw(0.0, 0.0, 0.0, position_interpolator, orientation_interpolator);
		rigid[i].draw(pos, fraction, step, position_interpolator, orientation_interpolator);
	}

	glPopMatrix();
}

/* draw_bound
 *
 * Create a representation for the bounding box and
 * render it.
 */
void objecthdl::draw_bound()
{
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	glRotatef(radtodeg(orientation[0]), 1.0, 0.0, 0.0);
	glRotatef(radtodeg(orientation[1]), 0.0, 1.0, 0.0);
	glRotatef(radtodeg(orientation[2]), 0.0, 0.0, 1.0);
	glScalef(scale, scale, scale);

	bound[0] -= 0.005;
	bound[1] += 0.005;
	bound[2] -= 0.005;
	bound[3] += 0.005;
	bound[4] -= 0.005;
	bound[5] += 0.005;

	vector<vec8f> bound_geometry;
	vector<int> bound_indices;
	bound_geometry.reserve(8);
	bound_geometry.push_back(vec8f(bound[0], bound[2], bound[4], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[1], bound[2], bound[4], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[1], bound[3], bound[4], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[0], bound[3], bound[4], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[0], bound[2], bound[5], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[1], bound[2], bound[5], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[1], bound[3], bound[5], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_geometry.push_back(vec8f(bound[0], bound[3], bound[5], 0.0, 0.0, 0.0, 0.0, 0.0));
	bound_indices.reserve(24);
	for (int i = 0; i < 4; i++)
	{
		bound_indices.push_back(i);
		bound_indices.push_back((i+1)%4);
		bound_indices.push_back(4+i);
		bound_indices.push_back(4+(i+1)%4);
		bound_indices.push_back(i);
		bound_indices.push_back(4+i);
	}

	whitehdl().apply(vector<lighthdl*>());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(float)*8, ((float*)bound_geometry.data()));
	glNormalPointer(GL_FLOAT, sizeof(float)*8, ((float*)bound_geometry.data()) + 3);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float)*8, ((float*)bound_geometry.data()) + 6);

	glDrawElements(GL_LINES, (int)bound_indices.size(), GL_UNSIGNED_INT, bound_indices.data());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();

	bound[0] += 0.005;
	bound[1] -= 0.005;
	bound[2] += 0.005;
	bound[3] -= 0.005;
	bound[4] += 0.005;
	bound[5] -= 0.005;
}

/* draw_normals
 *
 * create a representation of the normals for this object.
 * If face is false, render the vertex normals. Otherwise,
 * calculate the normals for each face and render those.
 */
void objecthdl::draw_normals(bool face)
{
	float radius = 0.0;
	for (int i = 0; i < 6; i++)
		if (abs(bound[i]) > radius)
			radius = abs(bound[i]);

	vector<vec8f> normal_geometry;
	vector<int> normal_indices;

	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);
	glRotatef(radtodeg(orientation[0]), 1.0, 0.0, 0.0);
	glRotatef(radtodeg(orientation[1]), 0.0, 1.0, 0.0);
	glRotatef(radtodeg(orientation[2]), 0.0, 0.0, 1.0);
	glScalef(scale, scale, scale);

	for (int i = 0; i < rigid.size(); i++)
	{
		if (!face)
		{
			for (int j = 0; j < rigid[i].geometry.size(); j++)
			{
				normal_indices.push_back(normal_geometry.size());
				normal_geometry.push_back(rigid[i].geometry[j]);
				normal_geometry.back().set(3,6,vec3f(0.0, 0.0, 0.0));
				normal_indices.push_back(normal_geometry.size());
				normal_geometry.push_back(rigid[i].geometry[j]);
				normal_geometry.back().set(0,3,(vec3f)(normal_geometry.back()(0,3) + radius*0.1f*normal_geometry.back()(3,6)));
				normal_geometry.back().set(3,6,vec3f(0.0, 0.0, 0.0));
			}
		}
		else
		{
			for (int j = 0; j < rigid[i].indices.size(); j+=3)
			{
				vec3f normal = norm((vec3f)rigid[i].geometry[rigid[i].indices[j + 0]](3,6) +
									(vec3f)rigid[i].geometry[rigid[i].indices[j + 1]](3,6) +
									(vec3f)rigid[i].geometry[rigid[i].indices[j + 2]](3,6));
				vec3f center = ((vec3f)rigid[i].geometry[rigid[i].indices[j + 0]](0,3) +
								(vec3f)rigid[i].geometry[rigid[i].indices[j + 1]](0,3) +
								(vec3f)rigid[i].geometry[rigid[i].indices[j + 2]](0,3))/3.0f;
				normal_indices.push_back(normal_geometry.size());
				normal_geometry.push_back(vec8f(center));
				normal_geometry.back().set(3,8,vec5f(0.0, 0.0, 0.0, 0.0, 0.0));
				normal_indices.push_back(normal_geometry.size());
				normal_geometry.push_back(vec8f(center + radius*0.1f*normal));
				normal_geometry.back().set(3,8,vec5f(0.0, 0.0, 0.0, 0.0, 0.0));
			}
		}

		whitehdl().apply(vector<lighthdl*>());

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(3, GL_FLOAT, sizeof(float)*8, ((float*)normal_geometry.data()));
		glNormalPointer(GL_FLOAT, sizeof(float)*8, ((float*)normal_geometry.data()) + 3);
		glTexCoordPointer(2, GL_FLOAT, sizeof(float)*8, ((float*)normal_geometry.data()) + 6);

		glDrawElements(GL_LINES, (int)normal_indices.size(), GL_UNSIGNED_INT, normal_indices.data());

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		normal_geometry.clear();
		normal_indices.clear();
	}

	glPopMatrix();
}

objecthdl &objecthdl::operator=(objecthdl o)
{
	rigid = o.rigid;
	for (map<string, materialhdl*>::iterator i = o.material.begin(); i != o.material.end(); i++)
		material.insert(pair<string, materialhdl*>(i->first, i->second->clone()));
	position = o.position;
	orientation = o.orientation;
	scale = o.scale;
	bound = o.bound;
	animation_length = o.animation_length;
	minstep = o.minstep;
	return *this;
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

