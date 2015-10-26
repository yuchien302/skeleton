/*
 * light.cpp
 *
 *  Created on: Dec 17, 2014
 *      Author: nbingham
 */

#include "light.h"
#include "object.h"
#include "canvas.h"

lighthdl::lighthdl()
{
	model = NULL;
	type = "light";
}

lighthdl::lighthdl(const vec3f &ambient, const vec3f &diffuse, const vec3f &specular)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	model = NULL;
	type = "light";
}

lighthdl::~lighthdl()
{

}

directionalhdl::directionalhdl() : lighthdl(white*0.1f, white*0.5f, white)
{
	type = "directional";
}

directionalhdl::directionalhdl(const vec3f &direction, const vec3f &ambient, const vec3f &diffuse, const vec3f &specular) : lighthdl(ambient, diffuse, specular)
{
	type = "directional";
}

directionalhdl::~directionalhdl()
{

}

void directionalhdl::update(canvashdl *canvas)
{
	/* TODO Assignment 3: Update the direction of the light using the orientation of the attached model.
	 * The easiest thing is to do translations and rotations like you were going to render the object, and
	 * then just multiply some initial direction vector by the normal matrix.
	 */


}

void directionalhdl::shade(vec3f &ambient, vec3f &diffuse, vec3f &specular, vec3f vertex, vec3f normal, float shininess) const
{
	/* TODO Assignment 3: Implement a directional light. See the OpenGL Orange Book in the references section
	 * of the course website. Its under the section about emulating the fixed function pipeline.
	 */
	// need to modify spec_base

	float diff_cosine = max((float)0.0, (float)dot(normal, norm(direction)));
	//float spec_base = max((float)0.0, (float)dot(normal, norm(half_vector)));
	float spec_base = max((float)0.0, (float)dot(normal, norm(direction)));
	float spec_pf;

	if(diff_cosine == 0)
		spec_pf = 0;
	else
		spec_pf = pow(spec_base, shininess);

	ambient += ambient;
	diffuse += diffuse * diff_cosine;
	specular += specular * spec_pf;
}

pointhdl::pointhdl() : lighthdl(white*0.1f, white*0.5f, white)
{
	this->attenuation = vec3f(1.0, 0.14, 0.7);
	type = "point";
}

pointhdl::pointhdl(const vec3f &position, const vec3f &attenuation, const vec3f &ambient, const vec3f &diffuse, const vec3f &specular) : lighthdl(ambient, diffuse, specular)
{
	this->attenuation = attenuation;
	type = "point";
}

pointhdl::~pointhdl()
{

}

void pointhdl::update(canvashdl *canvas)
{
	/* TODO Assignment 3: Update the position of the light using the position of the attached model.
	 * The easiest thing is to do translations and rotations like you were going to render the object, and
	 * then just multiply the origin by the modelview matrix.
	 */
	position = vec3f (canvas -> matrices[canvas -> modelview_matrix] *
			  homogenize(model -> position));
}

void pointhdl::shade(vec3f &ambient, vec3f &diffuse, vec3f &specular, vec3f vertex, vec3f normal, float shininess) const
{
	/* TEST Assignment 3: Implement a point light. See the OpenGL Orange Book in the references section
	 * of the course website. Its under the section about emulating the fixed function pipeline.
	 */
	vec3f toSurface = norm(position - vertex);
	vec3f eye = norm(vertex);

	float diff_cosine = max((float)0.0, (float)dot(normal, toSurface));
	float spec_base = max((float)0.0, (float) eye);
	float spec_pf;

	if(diff_cosine == 0)
		spec_pf = 0;
	else
		spec_pf = pow(spec_base, shininess);

	ambient += ambient * attenuation;
	diffuse += diffuse * diff_cosine * attenuation;
	specular += specular * spec_pf * attenuation;
}

spothdl::spothdl() : lighthdl(white*0.1f, white*0.5f, white)
{
	this->attenuation = vec3f(1.0, 0.14, 0.7);
	this->cutoff = 0.5;
	this->exponent = 1.0;
	type = "spot";
}

spothdl::spothdl(const vec3f &attenuation, const float &cutoff, const float &exponent, const vec3f &ambient, const vec3f &diffuse, const vec3f &specular) : lighthdl(ambient, diffuse, specular)
{
	this->attenuation = attenuation;
	this->cutoff = cutoff;
	this->exponent = exponent;
	type = "spot";
}

spothdl::~spothdl()
{

}

void spothdl::update(canvashdl *canvas)
{
	/* TODO Assignment 3: Update both the direction and position of the light using the position and orientation
	 * of the attached model. See above.
	 */
	position = vec3f (canvas -> matrices[canvas -> modelview_matrix] *
				  homogenize(model -> position));
	direction = vec3f (canvas -> matrices[canvas -> modelview_matrix] *
			  homogenize(model -> orientation));
}

void spothdl::shade(vec3f &ambient, vec3f &diffuse, vec3f &specular, vec3f vertex, vec3f normal, float shininess) const
{
	/* TODO Assignment 3: Implement a spot light. See the OpenGL Orange Book in the references section
	 * of the course website. Its under the section about emulating the fixed function pipeline.
	 */
	vec3f toSurface = norm(position - vertex);
	vec3f eye = norm(vertex);

	float diff_cosine = max((float)0.0, (float)dot(normal, toSurface));
	float spec_base = max((float)0.0, (float) eye);
	float spec_pf;

	float spot_cosine =  -1*dot(toSurface,norm (direction));
	if (spot_cosine < cutoff)
		attenuation = 0.0;
	else
		attenuation = pow(spot_cosine, exponent);

	if(diff_cosine == 0)
		spec_pf = 0;
	else
		spec_pf = pow(spec_base, shininess);

	ambient += ambient * attenuation;
	diffuse += diffuse * diff_cosine * attenuation;
	specular += specular * spec_pf * attenuation;
}
