/*
 * material.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: nbingham
 */

#include "material.h"
#include "canvas.h"
#include "light.h"

materialhdl::materialhdl()
{
	type = "material";
}

materialhdl::~materialhdl()
{
}

whitehdl::whitehdl()
{
	type = "white";
}

whitehdl::~whitehdl()
{

}

vec3f whitehdl::shade_vertex(canvashdl *canvas, vec3f vertex, vec3f normal, vector<float> &varying) const
{
	// DONE Assignment 3: Move your solid color shader from the canvas to here.
	vec4f point = canvas -> matrices[canvas -> projection_matrix] * homogenize(vertex);
	point = point / point.data[3];

	return point;
}

vec3f whitehdl::shade_fragment(canvashdl *canvas, vector<float> &varying) const
{
	return vec3f(1.0, 1.0, 1.0);
}

materialhdl *whitehdl::clone() const
{
	whitehdl *result = new whitehdl();
	result->type = type;
	return result;
}

gouraudhdl::gouraudhdl()
{
	type = "gouraud";
	emission = vec3f(0.0, 0.0, 0.0);
	ambient = vec3f(0.1, 0.1, 0.1);
	diffuse = vec3f(1.0, 1.0, 1.0);
	specular = vec3f(1.0, 1.0, 1.0);
	shininess = 1.0;
}

gouraudhdl::~gouraudhdl()
{

}

/* This is the vertex shader for a uniform material. The vertex and normal are in world coordinates, and
 * you have write access to the varying array which is used to pass data to the fragment shader. Don't
 * forget that this data is interpolated before it reaches the fragment shader.
 */
vec3f gouraudhdl::shade_vertex(canvashdl *canvas, vec3f vertex, vec3f normal, vector<float> &varying) const
{
	/* DONE Assignment 3: For flat and gouraud shading, just return the color you passed through the varying array.
	 * The final color is calculated in the vertex shader and passed to the fragment shader.
	 */

	const vector<lighthdl*>* lights;
	canvas -> get_uniform("lights", lights);
	vec3f a = vec3f(0.0, 0.0, 0.0);
	vec3f d = vec3f(0.0, 0.0, 0.0);
	vec3f s = vec3f(0.0, 0.0, 0.0);

	for(int i=0; (lights != NULL) && (i<lights->size()); i++){

		(*lights)[i] -> shade(a, d, s, vertex, normal, shininess);

	}

	vec3f color = a*this->ambient +d*this ->diffuse +s*this->specular + this->emission ;

	varying.push_back(color.data[0]);
	varying.push_back(color.data[1]);
	varying.push_back(color.data[2]);


	vec4f point = canvas -> matrices[canvas -> projection_matrix] * homogenize(vertex);
	point = point / point.data[3];

	return point;
}

vec3f gouraudhdl::shade_fragment(canvashdl *canvas, vector<float> &varying) const
{
	/* DONE Assignment 3: For flat and gouraud shading, just return the color you passed through the varying array.
	 * The final color is calculated in the vertex shader and passed to the fragment shader.
	 */
	vec3f color = clamp(vec3f(varying[0], varying[1], varying[2]), 0.0f, 1.0f);
	return color;
}

materialhdl *gouraudhdl::clone() const
{
	gouraudhdl *result = new gouraudhdl();
	result->type = type;
	result->emission = emission;
	result->ambient = ambient;
	result->diffuse = diffuse;
	result->specular = specular;
	result->shininess = shininess;
	return result;
}

phonghdl::phonghdl()
{
	type = "phong";
	emission = vec3f(0.0, 0.0, 0.0);
	ambient = vec3f(0.1, 0.1, 0.1);
	diffuse = vec3f(1.0, 1.0, 1.0);
	specular = vec3f(1.0, 1.0, 1.0);
	shininess = 1.0;
}

phonghdl::~phonghdl()
{

}

/* This is the vertex shader for a uniform material. The vertex and normal are in world coordinates, and
 * you have write access to the varying array which is used to pass data to the fragment shader. Don't
 * forget that this data is interpolated before it reaches the fragment shader.
 */
vec3f phonghdl::shade_vertex(canvashdl *canvas, vec3f vertex, vec3f normal, vector<float> &varying) const
{
	/* DONE Assignment 3: Implement phong shading, doing the same thing that you did to implement the gouraud
	 * and flat. The difference is that the normals have been interpolated and passed into the fragment shader
	 * instead of the color. The final color is calculated in the fragment shader.
	 */
	vec4f point = canvas -> matrices[canvas -> projection_matrix] * homogenize(vertex);
	point = point / point.data[3];

	varying.push_back(vertex[0]);
	varying.push_back(vertex[1]);
	varying.push_back(vertex[2]);
	varying.push_back(normal[0]);
	varying.push_back(normal[1]);
	varying.push_back(normal[2]);

	return point;
}

vec3f phonghdl::shade_fragment(canvashdl *canvas, vector<float> &varying) const
{
	/* DONE Assignment 3: Implement phong shading, doing the same thing that you did to implement the gouraud
	 * and flat. The difference is that the normals have been interpolated and passed into the fragment shader
	 * instead of the color. The final color is calculated in the fragment shader.
	 */
	const vector<lighthdl*>* lights;
	canvas -> get_uniform("lights", lights);
	vec3f a = vec3f(0.0, 0.0, 0.0);
	vec3f d = vec3f(0.0, 0.0, 0.0);
	vec3f s = vec3f(0.0, 0.0, 0.0);


	vec3f vertex = vec3f(varying[0], varying[1], varying[2]);
	vec3f normal = vec3f(varying[3], varying[4], varying[5]);

	for(int i=0; (lights != NULL) && (i<lights->size()); i++){

		(*lights)[i] -> shade(a, d, s, vertex, normal, shininess);

	}

	vec3f color = a*this->ambient +d*this ->diffuse +s*this->specular + this->emission ;
	color = clamp(color, 0.0f, 1.0f);

	return color;
}

materialhdl *phonghdl::clone() const
{
	phonghdl *result = new phonghdl();
	result->type = type;
	result->emission = emission;
	result->ambient = ambient;
	result->diffuse = diffuse;
	result->specular = specular;
	result->shininess = shininess;
	return result;
}

customhdl::customhdl()
{
	type = "custom";
	emission = vec3f(0.0, 0.0, 0.0);
	ambient = vec3f(0.1, 0.1, 0.1);
	diffuse = vec3f(1.0, 1.0, 1.0);
	specular = vec3f(1.0, 1.0, 1.0);
	shininess = 1.0;
}

customhdl::~customhdl()
{

}

vec3f customhdl::shade_vertex(canvashdl *canvas, vec3f vertex, vec3f normal, vector<float> &varying) const
{
	/* TODO Assignment 3: Implement flat and gouraud shading: Get the lights from the canvas using get_uniform.
	 * Add up the results of the shade functions for the lights. Transform the vertex and normal from world
	 * coordinates to eye coordinates before passing them into the shade functions. Calculate the final color
	 * and pass that to the fragment shader through the varying array. Pass the necessary data for phong shading
	 * through the varying array.
	 */
	vec4f point = canvas -> matrices[canvas -> projection_matrix] * homogenize(vertex);
	point = point / point.data[3];

	varying.push_back(vertex[0]);
	varying.push_back(vertex[1]);
	varying.push_back(vertex[2]);
	varying.push_back(normal[0]);
	varying.push_back(normal[1]);
	varying.push_back(normal[2]);

	return point;
}

vec3f customhdl::shade_fragment(canvashdl *canvas, vector<float> &varying) const
{
	/* TODO Assignment 3: For flat and gouraud shading, just return the color you passed through the varying array.
	 * Implement phong shading, doing the same thing that you did to implement the gouraud and flat. The difference
	 * is that the normals have been interpolated. Implement the none shading model, this just returns the
	 * color of the material without lighting.
	 */
	const vector<lighthdl*>* lights;
	canvas -> get_uniform("lights", lights);
	vec3f a = vec3f(0.0, 0.0, 0.0);
	vec3f d = vec3f(0.0, 0.0, 0.0);
	vec3f s = vec3f(0.0, 0.0, 0.0);


	vec3f vertex = vec3f(varying[0], varying[1], varying[2]);
	vec3f normal = vec3f(varying[3], varying[4], varying[5]);

	for(int i=0; (lights != NULL) && (i<lights->size()); i++){

		(*lights)[i] -> shade(a, d, s, vertex, normal, shininess);

	}

	vec3f color = a*this->ambient +d*this ->diffuse +s*this->specular + this->emission ;

	color = clamp(color, 0.0f, 1.0f);

	for(int i=0; i<3; i++){
//		float salt = 0.00015 - 0.0003 * (rand() % 100);
		float salt = 0.0;
		color[0] = ceil(color[0] / 0.25) * 0.25 + salt;
		color[1] = ceil(color[1] / 0.25) * 0.25 + salt;
		color[2] = ceil(color[2] / 0.25) * 0.25 + salt;
	}
	color = clamp(color, 0.0f, 1.0f);
	return color;
}

materialhdl *customhdl::clone() const
{
	customhdl *result = new customhdl();
	result->type = type;
	result->emission = emission;
	result->ambient = ambient;
	result->diffuse = diffuse;
	result->specular = specular;
	result->shininess = shininess;
	return result;
}
