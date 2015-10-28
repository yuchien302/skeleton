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
mat4f lighthdl::rotate(float angle, vec3f axis, mat4f matrix)
{
	// Done Assignment 1: Multiply the active matrix by a rotation matrix.
	// angel: degree
	float c = cos(angle);
	float s = sin(angle);
	float x = axis.data[0];
	float y = axis.data[1];
	float z = axis.data[2];

	mat4f rotate_mat = mat4f( x*x*(1.0-c)+c,   x*y*(1.0-c)-z*s, x*z*(1.0-c)+y*s, 0.0,
							  y*x*(1.0-c)+z*s, y*y*(1.0-c)+c,   y*z*(1.0-c)-x*s, 0.0,
							  x*z*(1.0-c)-y*s, y*z*(1.0-c)+x*s, z*z*(1.0-c)+c,   0.0,
							  0.0, 			   0.0, 			0.0,             1.0);

	return matrix * rotate_mat;
}
mat4f lighthdl::translate(vec3f direction, mat4f matrix)
{
	// Done Assignment 1: Multiply the active matrix by a translation matrix.
	float x = direction.data[0];
	float y = direction.data[1];
	float z = direction.data[2];

	mat4f translate_mat = mat4f( 1.0, 0.0, 0.0, x,
							  	 0.0, 1.0, 0.0, y,
								 0.0, 0.0, 1.0, z,
								 0.0, 0.0, 0.0, 1.0);

	return matrix * translate_mat;

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

	ambient += this->ambient;
	diffuse += this->diffuse * diff_cosine;
	specular += this->specular * spec_pf;
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
	//position = model -> position;
	//mat4f homo_pos = homogenize(model -> position);
	//homo_pos = rotate()
	model ->before_draw(canvas);
	vec4f homo_pos = canvas -> matrices[canvas -> modelview_matrix] *
			  vec4f(0.0,0.0,0.0,1.0);
	position = vec3f (homo_pos/homo_pos[3]);
	model ->after_draw(canvas);

}



void pointhdl::shade(vec3f &ambient, vec3f &diffuse, vec3f &specular, vec3f vertex, vec3f normal, float shininess) const
{
	/* TEST Assignment 3: Implement a point light. See the OpenGL Orange Book in the references section
	 * of the course website. Its under the section about emulating the fixed function pipeline.
	 */
	vec3f toLight = position - vertex;
	float d = mag(toLight);
	toLight = toLight/d;
	vec3f toEye = norm(-vertex);
	normal = norm(normal);
	float diff_cosine = max((float)0.0, (float)dot(normal, toLight));
	float spec_base = max((float)0.0, (float)dot(normal, norm(toLight+toEye)));
	float spec_pf;

	if(diff_cosine == 0)
		spec_pf = 0;
	else
		spec_pf = pow(spec_base, shininess);

	float decay = 1.0 / (attenuation[0] + attenuation[1]*d + attenuation[2]*d*d);

	//cout <<"decay"<< decay << endl;
	ambient += this->ambient * decay;
	diffuse += this->diffuse * diff_cosine * decay;
	specular += this->specular * spec_pf * decay;
	//cout<<normal<<endl;
	//cout << "dif_cosin"<< diff_cosine << "spec" << spec_base<<endl;
	//cout << "a: "<< this->ambient << "d: " << this->diffuse<<endl;
	//cout << ambient << " " << diffuse << " " <<specular<<endl;
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
	vec3f toSurface = position - vertex;
	float d = mag(toSurface);
	toSurface = norm(toSurface);
	vec3f eye = norm(vertex);

	float diff_cosine = max((float)0.0, (float)dot(normal, toSurface));
	float spec_base = max((float)0.0, (float) dot(normal, eye));
	float spec_pf;

	float spot_cosine =  -1*dot(toSurface,norm (direction));
	vec3f now_attenuation = vec3f(0.0, 0.0, 0.0);
	if (spot_cosine < cutoff)
		now_attenuation = vec3f(0.0, 0.0, 0.0);
	else
		now_attenuation = ((float)pow(spot_cosine, exponent)) * attenuation;

	if(diff_cosine == 0)
		spec_pf = 0;
	else
		spec_pf = pow(spec_base, shininess);

	float decay = 1.0 / (attenuation[0] + attenuation[1]*d + attenuation[2]*d*d);
	ambient += this->ambient * decay;
	diffuse += this->diffuse * diff_cosine * decay;
	specular += this->specular * spec_pf * decay;
}
