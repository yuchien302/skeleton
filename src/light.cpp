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

	/* DONE Assignment 3: Update the direction of the light using the orientation of the attached model.
	 * The easiest thing is to do translations and rotations like you were going to render the object, and
	 * then just multiply some initial direction vector by the normal matrix.
	 */
	model ->before_draw();
	mat4f modelview;
	glGetFloatv(GL_TRANSPOSE_MODELVIEW_MATRIX, (float*)modelview.data);

	vec4f homo_pos = modelview * vec4f(0.0, 0.0, 0.0, 1.0);
	vec3f position = vec3f (homo_pos/homo_pos[3]);

	vec4f homo_direction = modelview * vec4f(0.0, 0.0, -1.0, 1.0);
	direction = norm(vec3f(homo_direction/homo_direction[3]) - position);

	model ->after_draw();

}

void directionalhdl::apply(string name, GLuint program)
{

	/* DONE Assignment 4: Pass all necessary uniforms to the shaders for the directional light.
	 */
	GLint ambient_hdl = glGetUniformLocation(program, (name + ".ambient").c_str());
	GLint diffuse_hdl = glGetUniformLocation(program, (name + ".diffuse").c_str());
	GLint specular_hdl = glGetUniformLocation(program, (name + ".specular").c_str());

	GLint direction_hdl = glGetUniformLocation(program, (name + ".direction").c_str());

	glUniform3f(ambient_hdl, ambient[0], ambient[1], ambient[2]);
	glUniform3f(diffuse_hdl, diffuse[0], diffuse[1], diffuse[2]);
	glUniform3f(specular_hdl, specular[0], specular[1], specular[2]);

	glUniform3f(direction_hdl, direction[0], direction[1], direction[2]);

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

	/* DONE Assignment 3: Update the position of the light using the position of the attached model.
	 * The easiest thing is to do translations and rotations like you were going to render the object, and
	 * then just multiply the origin by the modelview matrix.
	 */

	model ->before_draw();
	mat4f modelview;
	glGetFloatv(GL_TRANSPOSE_MODELVIEW_MATRIX, (float*)modelview.data);

	vec4f homo_pos = modelview * vec4f(0.0, 0.0, 0.0, 1.0);
	position = vec3f (homo_pos/homo_pos[3]);
	model ->after_draw();
}


void pointhdl::apply(string name, GLuint program)
{
	/* DONE Assignment 4: Pass all necessary uniforms to the shaders for point lights.
	 */

	GLint ambient_hdl = glGetUniformLocation(program, (name + ".ambient").c_str());
	GLint diffuse_hdl = glGetUniformLocation(program, (name + ".diffuse").c_str());
	GLint specular_hdl = glGetUniformLocation(program, (name + ".specular").c_str());

	GLint attenuation_hdl = glGetUniformLocation(program, (name + ".attenuation").c_str());

	GLint position_hdl = glGetUniformLocation(program, (name + ".position").c_str());

	glUniform3f(ambient_hdl, ambient[0], ambient[1], ambient[2]);
	glUniform3f(diffuse_hdl, diffuse[0], diffuse[1], diffuse[2]);
	glUniform3f(specular_hdl, specular[0], specular[1], specular[2]);

	glUniform3f(attenuation_hdl, attenuation[0], attenuation[1], attenuation[2]);

	glUniform3f(position_hdl, position[0], position[1], position[2]);

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

	/* DONE Assignment 3: Update both the direction and position of the light using the position and orientation
	 * of the attached model. See above.
	 */
	model ->before_draw();
	mat4f modelview;
	glGetFloatv(GL_TRANSPOSE_MODELVIEW_MATRIX, (float*)modelview.data);

	vec4f homo_pos = modelview * vec4f(0.0, 0.0, 0.0, 1.0);
	position = vec3f (homo_pos/homo_pos[3]);

	vec4f homo_direction = modelview * vec4f(0.0, 0.0, -1.0, 1.0);
	direction = norm(vec3f(homo_direction/homo_direction[3]) - position);

	model ->after_draw();

}

void spothdl::apply(string name, GLuint program)
{
	/* TODO Assignment 4: Pass all necessary uniforms to the shaders for spot lights.
	 */
	GLint ambient_hdl = glGetUniformLocation(program, (name + ".ambient").c_str());
	GLint diffuse_hdl = glGetUniformLocation(program, (name + ".diffuse").c_str());
	GLint specular_hdl = glGetUniformLocation(program, (name + ".specular").c_str());

	GLint attenuation_hdl = glGetUniformLocation(program, (name + ".attenuation").c_str());
	GLint cutoff_hdl = glGetUniformLocation(program, (name + ".cutoff").c_str());
	GLint exponent_hdl = glGetUniformLocation(program, (name + ".exponent").c_str());

	GLint position_hdl = glGetUniformLocation(program, (name + ".position").c_str());
	GLint direction_hdl = glGetUniformLocation(program, (name + ".direction").c_str());

	glUniform3f(ambient_hdl, ambient[0], ambient[1], ambient[2]);
	glUniform3f(diffuse_hdl, diffuse[0], diffuse[1], diffuse[2]);
	glUniform3f(specular_hdl, specular[0], specular[1], specular[2]);

	glUniform3f(attenuation_hdl, attenuation[0], attenuation[1], attenuation[2]);
	glUniform1f(cutoff_hdl, cutoff);
	glUniform1f(exponent_hdl, exponent);

	glUniform3f(position_hdl, position[0], position[1], position[2]);
	glUniform3f(direction_hdl, direction[0], direction[1], direction[2]);

}
