/*
 * model.h
 *
 *  Created on: Dec 3, 2014
 *      Author: nbingham
 */

#include "common/standard.h"
#include "scene.h"
#include "object.h"
#include "vrml/scene.h"

#ifndef model_h
#define model_h

struct modelhdl : objecthdl
{
	modelhdl();
	modelhdl(string filename);
	~modelhdl();

	void load_obj(string filename);
	void load_mtl(string filename);

	void load_wrl(string filename);
};

struct definition
{
	mat4f model;
	mat3f norm;
	parse_vrml::node syntax;
};

vector<vec3f> interpret_properties(mat4f model, mat3f norm, const parse_vrml::node &syntax, map<string, definition> &names, vector<string> parent);
vector<vec3f> interpret_properties(mat4f model, mat3f norm, const parse_vrml::nodeDeclaration &syntax, map<string, definition> &names, vector<string> parent);
pair<string, phonghdl> interpret_appearance(const parse_vrml::node &syntax, map<string, definition> &names, vector<string> parent);
pair<string, phonghdl> interpret_appearance(const parse_vrml::nodeDeclaration &syntax, map<string, definition> &names, vector<string> parent);
rigidhdl interpret_geometry(mat4f model, mat3f norm, const parse_vrml::node &syntax, map<string, definition> &names, vector<string> parent);
rigidhdl interpret_geometry(mat4f model, mat3f norm, const parse_vrml::nodeDeclaration &syntax, map<string, definition> &names, vector<string> parent);
objecthdl interpret_interpolator(mat4f model, mat3f norm, const parse_vrml::node &syntax, map<string, definition> &names, vector<string> parent);
objecthdl interpret_interpolator(mat4f model, mat3f norm, const parse_vrml::nodeDeclaration &syntax, map<string, definition> &names, vector<string> parent);
objecthdl interpret_sensors(mat4f model, mat3f norm, const parse_vrml::node &syntax, map<string, definition> &names, vector<string> parent);
objecthdl interpret_sensors(mat4f model, mat3f norm, const parse_vrml::nodeDeclaration &syntax, map<string, definition> &names, vector<string> parent);
objecthdl interpret_groups(mat4f model, mat3f norm, const parse_vrml::node &syntax, map<string, definition> &names, vector<string> parent);
objecthdl interpret_groups(mat4f model, mat3f norm, const parse_vrml::nodeDeclaration &syntax, map<string, definition> &names, vector<string> parent);
void interpret_route(const parse_vrml::routeDeclaration &syntax, map<pair<string, string>, pair<string, string> > &routes, vector<string> parent);

#endif
