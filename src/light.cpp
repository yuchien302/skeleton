/*
 * light.cpp
 *
 *  Created on: Dec 17, 2014
 *      Author: nbingham
 */

#include "light.h"
#include "object.h"
#include "opengl.h"

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

void directionalhdl::update()
{

	/* TODO Assignment 3: Update the direction of the light using the orientation of the attached model.
	 * The easiest thing is to do translations and rotations like you were going to render the object, and
	 * then just multiply some initial direction vector by the normal matrix.
	 */
//	model ->before_draw(canvas);
//
//	vec4f homo_pos = canvas -> matrices[canvas -> modelview_matrix] * vec4f(0.0, 0.0, 0.0, 1.0);
//	vec3f position = homo_pos/homo_pos[3];
//
//	vec4f homo_direction = canvas -> matrices[canvas -> modelview_matrix] * vec4f(0.0, 0.0, -1.0, 1.0);
//	direction = norm(vec3f(homo_direction/homo_direction[3]) - position);
//
//	model ->after_draw(canvas);

}

void directionalhdl::apply(string name, GLuint program)
{

	/* TODO Assignment 4: Pass all necessary uniforms to the shaders for the directional light.
	 */

//	float pf = 0.0; // power factor
//	float nDotVP = max(0.0f, dot(normal, norm(direction)));
//
//	if (nDotVP == 0.0)
//		pf = 0.0;
//	else
//		pf = pow(nDotVP, shininess);
//
//	ambient  += this->ambient;
//	diffuse  += this->diffuse * nDotVP;
//	specular += this->specular * pf;
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

void pointhdl::update()
{

	/* TODO Assignment 3: Update the position of the light using the position of the attached model.
	 * The easiest thing is to do translations and rotations like you were going to render the object, and
	 * then just multiply the origin by the modelview matrix.
	 */
	//	position = vec3f (canvas -> matrices[canvas -> modelview_matrix] *
	//				  homogenize(model -> position));


	model ->before_draw();
//	vec4f homo_pos = canvas -> matrices[canvas -> modelview_matrix] * vec4f(0.0, 0.0, 0.0, 1.0);
//	position = vec3f (homo_pos/homo_pos[3]);
	model ->after_draw();

}


void pointhdl::apply(string name, GLuint program)
{
	/* TODO Assignment 4: Pass all necessary uniforms to the shaders for point lights.
	 */

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

void spothdl::update()
{

	/* TODO Assignment 3: Update both the direction and position of the light using the position and orientation
	 * of the attached model. See above.
	 */
//	model ->before_draw(canvas);
//
//	vec4f homo_pos = canvas -> matrices[canvas -> modelview_matrix] * vec4f(0.0, 0.0, 0.0, 1.0);
//	position = homo_pos/homo_pos[3];
//
//	vec4f homo_direction = canvas -> matrices[canvas -> modelview_matrix] * vec4f(0.0, 0.0, -1.0, 1.0);
//	direction = norm(vec3f(homo_direction/homo_direction[3]) - position);
//
//	model ->after_draw(canvas);
}

void spothdl::apply(string name, GLuint program)
{
	/* TODO Assignment 4: Pass all necessary uniforms to the shaders for spot lights.
	 */
//	vec3f VP = position - vertex;
//	float d = mag(VP);
//	VP = norm(VP);
//	vec3f eye = norm(vertex);
//
//	float attenuate = 1.0 / (attenuation[0] + attenuation[1] * d + attenuation[2] * d * d);
//
//	float spotDot =  dot(-VP, norm(direction));
//
//	if (spotDot < cutoff)
//		attenuate = 0.0f;
//	else
//		attenuate *= ((float)pow(spotDot, exponent));
//
//	vec3f halfVector = norm(VP + eye);
//	float nDotVP = max(0.0f, dot(normal, VP));
//	float nDotHV = max(0.0f, dot(normal, halfVector));
//	float pf;
//
//	if (nDotVP == 0.0f)
//		pf = 0.0f;
//	else
//		pf = pow(nDotHV, shininess);
//
//	ambient  += this->ambient * attenuate;
//	diffuse  += this->diffuse * nDotVP * attenuate;
//	specular += this->specular * pf * attenuate;

}
