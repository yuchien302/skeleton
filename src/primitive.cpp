/*
 * primitive.cpp
 *
 *  Created on: Dec 21, 2014
 *      Author: nbingham
 */

#include "primitive.h"

/* trianglehdl
 *
 * Generate the geometry and indices required to make a simple triangle.
 * Just for debugging
 */
trianglehdl::trianglehdl()
{
	rigid.push_back(rigidhdl());
	rigid[0].geometry.reserve(3);
	rigid[0].geometry.push_back(vec8f(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f(0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f(0.0, 0.0, -0.5, 0.0, 0.0, 1.0, 0.0, 0.0));
	rigid[0].indices.push_back(0);
	rigid[0].indices.push_back(1);
	rigid[0].indices.push_back(2);

}

trianglehdl::~trianglehdl()
{

}

/* boxhdl
 *
 * Generate the geometry and indices required to make a box.
 */
boxhdl::boxhdl(float width, float height, float depth)
{
	/* Done Assignment 1: Generate the geometry and indices required to make a box.
	 * Calculate its bounding box.
	 */
	rigid.push_back(rigidhdl());

	// push 8 vertex to geometry
	rigid[0].geometry.reserve(8);
	rigid[0].geometry.push_back(vec8f(width/2.0, height/2.0, depth/2.0, 1.0/sqrt(3), 1.0/sqrt(3), 1.0/sqrt(3), 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f(-width/2.0, height/2.0, depth/2.0, -1.0/sqrt(3), 1.0/sqrt(3), 1.0/sqrt(3), 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f(-width/2.0, -height/2.0, depth/2.0, -1.0/sqrt(3), -1.0/sqrt(3), 1.0/sqrt(3), 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f(width/2.0, -height/2.0, depth/2.0, 1.0/sqrt(3), -1.0/sqrt(3), 1.0/sqrt(3), 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f(width/2.0, height/2.0, -depth/2.0, 1.0/sqrt(3), 1.0/sqrt(3), -1.0/sqrt(3), 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f(-width/2.0, height/2.0, -depth/2.0, -1.0/sqrt(3), 1.0/sqrt(3), -1.0/sqrt(3), 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f(-width/2.0, -height/2.0, -depth/2.0, -1.0/sqrt(3), -1.0/sqrt(3), -1.0/sqrt(3), 0.0, 0.0));
	rigid[0].geometry.push_back(vec8f(width/2.0, -height/2.0, -depth/2.0, 1.0/sqrt(3), -1.0/sqrt(3),-1.0/sqrt(3), 0.0, 0.0));

	// push 12 lines in to indices

	rigid[0].indices.push_back(2);
	rigid[0].indices.push_back(1);
	rigid[0].indices.push_back(0);
	rigid[0].indices.push_back(3);
	rigid[0].indices.push_back(2);
	rigid[0].indices.push_back(0);
	for(int i = 0; i < 4; i++){
		rigid[0].indices.push_back(i);

		rigid[0].indices.push_back((i+1)%4);
		rigid[0].indices.push_back(i+4);
	}
	for(int i = 0; i < 4; i++){

		rigid[0].indices.push_back((i+1)%4);
		rigid[0].indices.push_back((i+1)%4+4);
		rigid[0].indices.push_back(i+4);
	}
	rigid[0].indices.push_back(5);
	rigid[0].indices.push_back(6);
	rigid[0].indices.push_back(4);
	rigid[0].indices.push_back(6);
	rigid[0].indices.push_back(7);
	rigid[0].indices.push_back(4);

	// set bounding box
	bound = vec6f(-width, width, -height, height, -depth, depth);

	// DONE Assignment 3: Set up the material properties for this object
	rigid[0].material = "Material__1";
	material["Material__1"] = new whitehdl();
}

boxhdl::~boxhdl()
{

}

/* spherehdl
 *
 * Generate the geometry and indices required to make a sphere.
 */
spherehdl::spherehdl(float radius, int levels, int slices)
{
	rigid.push_back(rigidhdl());

	rigid[0].geometry.reserve(2 + (levels-1)*slices);
	rigid[0].geometry.push_back(vec8f(0.0, 0.0, radius, 0.0, 0.0, 1.0, 0.0, 0.0));
	for (int i = 1; i < levels; i++)
		for (int j = 0; j < slices; j++)
		{
			vec3f dir(sin(m_pi*(float)i/(float)levels)*cos(2.0*m_pi*(float)j/(float)slices),
					  sin(m_pi*(float)i/(float)levels)*sin(2.0*m_pi*(float)j/(float)slices),
					  cos(m_pi*(float)i/(float)levels));
			rigid[0].geometry.push_back(vec8f(radius*dir[0], radius*dir[1], radius*dir[2],
									 dir[0], dir[1], dir[2], 0.0, 0.0));
		}
	rigid[0].geometry.push_back(vec8f(0.0, 0.0, -radius, 0.0, 0.0, -1.0, 0.0, 0.0));

	for (int i = 0; i < slices; i++)
	{
		rigid[0].indices.push_back(1 + (i+1)%slices);
		rigid[0].indices.push_back(1 + i);
		rigid[0].indices.push_back(0);
	}

	for (int i = 0; i < levels-2; i++)
		for (int j = 0; j < slices; j++)
		{
			rigid[0].indices.push_back(1 + i*slices + j);
			rigid[0].indices.push_back(1 + i*slices + (j+1)%slices);
			rigid[0].indices.push_back(1 + (i+1)*slices + j);

			rigid[0].indices.push_back(1 + (i+1)*slices + j);
			rigid[0].indices.push_back(1 + i*slices + (j+1)%slices);
			rigid[0].indices.push_back(1 + (i+1)*slices + (j+1)%slices);
		}

	for (int i = 0; i < slices; i++)
	{
		rigid[0].indices.push_back(1 + (levels-1)*slices);
		rigid[0].indices.push_back(1 + (levels-2)*slices + i);
		rigid[0].indices.push_back(1 + (levels-2)*slices + (i+1)%slices);
	}

	bound = vec6f(-radius, radius, -radius, radius, -radius, radius);

	// DONE Assignment 3: Set up the material properties for this object
	rigid[0].material = "Material__1";
	material["Material__1"] = new whitehdl();
}

spherehdl::~spherehdl()
{

}

/* cylinderhdl
 *
 * Generate the geometry and indices required to make a cylinder.
 */
cylinderhdl::cylinderhdl(float radius, float height, int slices)
{
	/* Done Assignment 1: Generate the geometry and indices required to make a cylinder.
	 * Calculate its bounding box.
	 */
	rigid.push_back(rigidhdl());
	rigid[0].geometry.reserve(2*slices + 2);

	//top circle
	rigid[0].geometry.push_back(vec8f(0.0, height/2.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0));
	for (int i = 0; i < slices; i++){
		vec3f dir(cos(2.0*m_pi/slices*i), sin(2.0*m_pi/slices*i), 0.0);
		rigid[0].geometry.push_back(vec8f(radius * dir[0], height/2.0, radius * dir[1], dir[0], 0.0, dir[1], 0.0, 0.0));
	}
	//bottom circle
	for (int i = 0; i < slices; i++){
			vec3f dir(cos(2.0*m_pi/slices*i), sin(2.0*m_pi/slices*i), 0.0);
			rigid[0].geometry.push_back(vec8f(radius * dir[0], -height/2.0, radius * dir[1], dir[0], 0.0, dir[1], 0.0, 0.0));
		}
	rigid[0].geometry.push_back(vec8f(0.0, -height/2.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0));
	
	//top circle
	for (int i = 0; i < slices; i++){
		rigid[0].indices.push_back(i+1);
		rigid[0].indices.push_back(((i+1)%slices) +1);
		rigid[0].indices.push_back(0);
	}
	//bottom circle
	for (int i = 0; i < slices; i++){
		rigid[0].indices.push_back(((i+1)%slices) +1 + slices);
		rigid[0].indices.push_back(i+1 + slices);
		rigid[0].indices.push_back(rigid[0].geometry.size() - 1);
	}
	// side
	for (int i = 0; i < slices; i++){
		rigid[0].indices.push_back(((i+1)%slices) +1);
		rigid[0].indices.push_back(i+1);
		rigid[0].indices.push_back(i+1 +slices);

		rigid[0].indices.push_back(((i+1)%slices) +1);
		rigid[0].indices.push_back(i+1 + slices);
		rigid[0].indices.push_back(((i+1)%slices) +1 + slices);
	}
	bound = vec6f(-radius, radius, -height/ 2.0, height/2.0, -radius, radius);


	// DONE Assignment 3: Set up the material properties for this object
	rigid[0].material = "Material__1";
	material["Material__1"] = new whitehdl();
}

cylinderhdl::~cylinderhdl()
{

}

/* pyramidhdl
 *
 * Generate the geometry and indices required to make a pyramid.
 */
pyramidhdl::pyramidhdl(float radius, float height, int slices)
{
	/* Done Assignment 1: Generate the geometry and indices required to make a pyramid.
	 * Calculate its bounding box.
	 */
	rigid.push_back(rigidhdl());
	rigid[0].geometry.reserve(slices + 2);
	
	//top center
	rigid[0].geometry.push_back(vec8f(0.0, height/2.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0));

	//bot circle
	for (int i = 0; i < slices; i++){
		vec3f dir(cos(2.0*m_pi/slices*i), sin(2.0*m_pi/slices*i), 0.0);
		rigid[0].geometry.push_back(vec8f(radius * dir[0], -height/2.0, -radius * dir[1], dir[0], 0.0, -dir[1], 0.0, 0.0));
	}
	rigid[0].geometry.push_back(vec8f(0.0, -height/2.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0));

	//bot circle
	for (int i = 0; i < slices; i++){


		rigid[0].indices.push_back(((i+1)%slices) +1);
		rigid[0].indices.push_back(rigid[0].geometry.size() - 1);
		rigid[0].indices.push_back(i+1);

	}

	// side
	for (int i = 0; i < slices; i++){
		rigid[0].indices.push_back(i+1);
		rigid[0].indices.push_back(0);
		rigid[0].indices.push_back(((i+1)%slices) +1);

	}
	bound = vec6f(-radius, radius, -height/ 2.0, height/2.0, -radius, radius);

	// DONE Assignment 3: Set up the material properties for this object
	rigid[0].material = "Material__1";
	material["Material__1"] = new whitehdl();
}

pyramidhdl::~pyramidhdl()
{

}
