/*
 * object.h
 *
 *  Created on: Dec 3, 2014
 *      Author: nbingham
 */

#include "core/geometry.h"
#include "common/standard.h"
#include "opengl.h"

#include "material.h"

using namespace core;

#ifndef object_h
#define object_h

struct lighthdl;

/* This represents a rigid body, which
 * is just a group of geometry to be
 * rendered together. Its grouped in
 * this way so that you can apply different
 * materials to different parts of the
 * same model.
 */
struct rigidhdl
{
	rigidhdl();
	~rigidhdl();

	vector<string> name;
	vector<vec8f> geometry;
	vector<int> indices;
	string material;

	vector<map<double, vec3f> > positions;
	vector<map<double, vec4d> > orientations;
	vector<vec3f> center;
	vector<vec3f> scale;
	vector<vec4d> scale_orientation;

	vec3f get_position(int frame, double pos, double fraction, double step, int method);
	vec4d get_orientation(int frame, double pos, double fraction, double step, int method);
	float get_B_coef(double t, double i);
	void draw(double pos, double fraction, double step, int position_interpolator=0, int orientation_interpolator=0);
};

struct objecthdl
{
	objecthdl();
	objecthdl(const objecthdl &o);
	virtual ~objecthdl();

	vector<rigidhdl> rigid;
	map<string, materialhdl*> material;


	vec3f position;
	vec3f orientation;
	float scale;

	double start_time;
	double minstep;
	double animation_length;
	int position_interpolator;
	int orientation_interpolator;

	// The bounding box of this object
	// (left, right, bottom, top, front, back)
	vec6f bound;

	void before_draw();
	void after_draw();

	void draw(const vector<lighthdl*> &lights);
	void draw_bound();
	void draw_normals(bool face = false);

	objecthdl &operator=(objecthdl o);
};

#endif
