/*
 * material.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: nbingham
 */

#include "material.h"
#include "light.h"
#include "lodepng.h"

GLuint whitehdl::vertex = 0;
GLuint whitehdl::fragment = 0;
GLuint whitehdl::program = 0;

GLuint gouraudhdl::vertex = 0;
GLuint gouraudhdl::fragment = 0;
GLuint gouraudhdl::program = 0;

GLuint phonghdl::vertex = 0;
GLuint phonghdl::fragment = 0;
GLuint phonghdl::program = 0;

GLuint customhdl::vertex = 0;
GLuint customhdl::fragment = 0;
GLuint customhdl::program = 0;

GLuint texturehdl::vertex = 0;
GLuint texturehdl::fragment = 0;
GLuint texturehdl::program = 0;
GLuint texturehdl::texture = 0;

extern string working_directory;

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

	if (vertex == 0 && fragment == 0 && program == 0)
	{
		/* DONE Assignment 4: Load and link the shaders. Keep in mind that vertex, fragment,
		 * and program are static variables meaning they are *shared across all instances of
		 * this class. So you only have to initialize them once when the first instance of
		 * the class is created.
		 */
		vertex = load_shader_file(working_directory + "res/white.vx", GL_VERTEX_SHADER);
		fragment = load_shader_file(working_directory + "res/white.ft", GL_FRAGMENT_SHADER);
		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
	}

}

whitehdl::~whitehdl()
{

}

void whitehdl::apply(const vector<lighthdl*> &lights)
{
	glUseProgram(program);
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

	if (vertex == 0 && fragment == 0 && program == 0)
	{
		/* DONE Assignment 4: Load and link the shaders. Keep in mind that vertex, fragment,
		 * and program are static variables meaning they are *shared across all instances of
		 * this class. So you only have to initialize them once when the first instance of
		 * the class is created.
		 */
		vertex = load_shader_file(working_directory + "res/gouraud.vx", GL_VERTEX_SHADER);
		fragment = load_shader_file(working_directory + "res/gouraud.ft", GL_FRAGMENT_SHADER);
		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);

	}
}

gouraudhdl::~gouraudhdl()
{

}

void gouraudhdl::apply(const vector<lighthdl*> &lights)
{
	// DONE Assignment 4: Apply the shader program and pass it the necessary uniform values
	glUseProgram(program);
	int num_p = 0, num_d = 0, num_s = 0;
	for(int i=0; i<lights.size(); i++){
		if(lights[i] -> type == "point"){
			lights[i] -> apply("plights[" + to_string(num_p) + "]", program);
			num_p++;
		}
		else if(lights[i] -> type == "directional"){
			lights[i] -> apply("dlights[" + to_string(num_d) + "]", program);
			num_d++;
		}
		else if(lights[i] -> type == "spot"){
			lights[i] -> apply("slights[" + to_string(num_s) + "]", program);
			num_s++;
		}
	}

	GLint num_plights = glGetUniformLocation(program, "num_plights");
	GLint num_dlights = glGetUniformLocation(program, "num_dlights");
	GLint num_slights = glGetUniformLocation(program, "num_slights");

	GLint emission_hdl = glGetUniformLocation(program, "emission");
	GLint ambient_hdl = glGetUniformLocation(program, "ambient");
	GLint diffuse_hdl = glGetUniformLocation(program, "diffuse");
	GLint specular_hdl = glGetUniformLocation(program, "specular");
	GLint shininess_hdl = glGetUniformLocation(program, "shininess");

	glUniform1i(num_plights, num_p);
	glUniform1i(num_dlights, num_d);
	glUniform1i(num_slights, num_s);
	glUniform3f(emission_hdl, emission[0], emission[1], emission[2]);
	glUniform3f(ambient_hdl, ambient[0], ambient[1], ambient[2]);
	glUniform3f(diffuse_hdl, diffuse[0], diffuse[1], diffuse[2]);
	glUniform3f(specular_hdl, specular[0], specular[1], specular[2]);
	glUniform1f(shininess_hdl, shininess);
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
	ambient = vec3f(0.2, 0.2, 0.2);
	diffuse = vec3f(0.8, 0.8, 0.8);
	specular = vec3f(0.0, 0.0, 0.0);
	shininess = 0.2;

	if (vertex == 0 && fragment == 0 && program == 0)
	{
		/* DONE Assignment 4: Load and link the shaders. Keep in mind that vertex, fragment,
		 * and program are static variables meaning they are *shared across all instances of
		 * this class. So you only have to initialize them once when the first instance of
		 * the class is created.
		 */
		vertex = load_shader_file(working_directory + "res/phong.vx", GL_VERTEX_SHADER);
		fragment = load_shader_file(working_directory + "res/phong.ft", GL_FRAGMENT_SHADER);
		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
	}
}

phonghdl::~phonghdl()
{

}

void phonghdl::apply(const vector<lighthdl*> &lights)
{
	// DONE Assignment 4: Apply the shader program and pass it the necessary uniform values
	glUseProgram(program);
	int num_p = 0, num_d = 0, num_s = 0;
	for(int i=0; i<lights.size(); i++){
		if(lights[i] -> type == "point"){
			lights[i] -> apply("plights[" + to_string(num_p) + "]", program);
			num_p++;
		}
		else if(lights[i] -> type == "directional"){
			lights[i] -> apply("dlights[" + to_string(num_d) + "]", program);
			num_d++;
		}
		else if(lights[i] -> type == "spot"){
			lights[i] -> apply("slights[" + to_string(num_s) + "]", program);
			num_s++;
		}
	}

	GLint num_plights = glGetUniformLocation(program, "num_plights");
	GLint num_dlights = glGetUniformLocation(program, "num_dlights");
	GLint num_slights = glGetUniformLocation(program, "num_slights");

	GLint emission_hdl = glGetUniformLocation(program, "emission");
	GLint ambient_hdl = glGetUniformLocation(program, "ambient");
	GLint diffuse_hdl = glGetUniformLocation(program, "diffuse");
	GLint specular_hdl = glGetUniformLocation(program, "specular");
	GLint shininess_hdl = glGetUniformLocation(program, "shininess");

	glUniform1i(num_plights, num_p);
	glUniform1i(num_dlights, num_d);
	glUniform1i(num_slights, num_s);
	glUniform3f(emission_hdl, emission[0], emission[1], emission[2]);
	glUniform3f(ambient_hdl, ambient[0], ambient[1], ambient[2]);
	glUniform3f(diffuse_hdl, diffuse[0], diffuse[1], diffuse[2]);
	glUniform3f(specular_hdl, specular[0], specular[1], specular[2]);
	glUniform1f(shininess_hdl, shininess);
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

	if (vertex == 0 && fragment == 0 && program == 0)
	{
		/* DONE Assignment 4: Load and link the shaders. Keep in mind that vertex, fragment,
		 * and program are static variables meaning they are *shared across all instances of
		 * this class. So you only have to initialize them once when the first instance of
		 * the class is created.
		 */
		vertex = load_shader_file(working_directory + "res/custom.vx", GL_VERTEX_SHADER);
		fragment = load_shader_file(working_directory + "res/custom.ft", GL_FRAGMENT_SHADER);
		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
	}
}

customhdl::~customhdl()
{

}

void customhdl::apply(const vector<lighthdl*> &lights)
{
	// DONE Assignment 4: Apply the shader program and pass it the necessary uniform values
	glUseProgram(program);
	int num_p = 0, num_d = 0, num_s = 0;
	for(int i=0; i<lights.size(); i++){
		if(lights[i] -> type == "point"){
			lights[i] -> apply("plights[" + to_string(num_p) + "]", program);
			num_p++;
		}
		else if(lights[i] -> type == "directional"){
			lights[i] -> apply("dlights[" + to_string(num_d) + "]", program);
			num_d++;
		}
		else if(lights[i] -> type == "spot"){
			lights[i] -> apply("slights[" + to_string(num_s) + "]", program);
			num_s++;
		}
	}

	GLint num_plights = glGetUniformLocation(program, "num_plights");
	GLint num_dlights = glGetUniformLocation(program, "num_dlights");
	GLint num_slights = glGetUniformLocation(program, "num_slights");

	GLint emission_hdl = glGetUniformLocation(program, "emission");
	GLint ambient_hdl = glGetUniformLocation(program, "ambient");
	GLint diffuse_hdl = glGetUniformLocation(program, "diffuse");
	GLint specular_hdl = glGetUniformLocation(program, "specular");
	GLint shininess_hdl = glGetUniformLocation(program, "shininess");

	glUniform1i(num_plights, num_p);
	glUniform1i(num_dlights, num_d);
	glUniform1i(num_slights, num_s);
	glUniform3f(emission_hdl, emission[0], emission[1], emission[2]);
	glUniform3f(ambient_hdl, ambient[0], ambient[1], ambient[2]);
	glUniform3f(diffuse_hdl, diffuse[0], diffuse[1], diffuse[2]);
	glUniform3f(specular_hdl, specular[0], specular[1], specular[2]);
	glUniform1f(shininess_hdl, shininess);
}

materialhdl *customhdl::clone() const
{
	customhdl *result = new customhdl();
	result->type = type;
	result->emission = emission;
	result->ambient = ambient;
	result->diffuse = diffuse;
	result->specular = specular;
	return result;
}

texturehdl::texturehdl()
{
	type = "texture";

	shininess = 1.0;

	if (vertex == 0 && fragment == 0 && program == 0)
	{
		/* DONE Assignment 4: Load and link the shaders and load the texture Keep in mind that vertex, fragment,
		 * and program are static variables meaning they are *shared across all instances of
		 * this class. So you only have to initialize them once when the first instance of
		 * the class is created.
		 */
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		unsigned width, height;
		vector<unsigned char>image;
		//decode
		unsigned error = lodepng::decode(image, width, height, "res/texture.png");
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


		//glGenerateMipmap(GL_TEXTURE_2D);
		vertex = load_shader_file(working_directory + "res/texture.vx", GL_VERTEX_SHADER);
		fragment = load_shader_file(working_directory + "res/texture.ft", GL_FRAGMENT_SHADER);
		program = glCreateProgram();

		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		glLinkProgram(program);
	}
}

texturehdl::~texturehdl()
{

}

void texturehdl::apply(const vector<lighthdl*> &lights)
{
	// DONE Assignment 4: Apply the shader program and pass it the necessary uniform values

	glUseProgram(program);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	int num_p = 0, num_d = 0, num_s = 0;
	for(int i=0; i<lights.size(); i++){
		if(lights[i] -> type == "point"){
			lights[i] -> apply("plights[" + to_string(num_p) + "]", program);
			num_p++;
		}
		else if(lights[i] -> type == "directional"){
			lights[i] -> apply("dlights[" + to_string(num_d) + "]", program);
			num_d++;
		}
		else if(lights[i] -> type == "spot"){
			lights[i] -> apply("slights[" + to_string(num_s) + "]", program);
			num_s++;
		}
	}

	GLint num_plights = glGetUniformLocation(program, "num_plights");
	GLint num_dlights = glGetUniformLocation(program, "num_dlights");
	GLint num_slights = glGetUniformLocation(program, "num_slights");

	GLint shininess_hdl = glGetUniformLocation(program, "shininess");

	GLint tex_hdl = glGetUniformLocation(program, "tex");


	glUniform1i(num_plights, num_p);
	glUniform1i(num_dlights, num_d);
	glUniform1i(num_slights, num_s);
	glUniform1f(shininess_hdl, shininess);
	glUniform1i(tex_hdl, 0);
}

materialhdl *texturehdl::clone() const
{
	texturehdl *result = new texturehdl();
	result->type = type;
	result->shininess = shininess;
	return result;
}
