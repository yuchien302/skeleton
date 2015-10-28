/*
 * canvas.cpp
 *
 *  Created on: Dec 2, 2014
 *      Author: nbingham
 */

#include "canvas.h"
#include "core/geometry.h"
#include "light.h"
#include "material.h"
#include "assert.h"

canvashdl::canvashdl(int w, int h)
{
	last_reshape_time = -1.0;
	width = w;
	height = h;
	reshape_width = w;
	reshape_height = h;

	matrices[viewport_matrix] = mat4f((float)width/2.0, 0.0, 0.0, (float)width/2.0,
									  0.0, (float)height/2.0, 0.0, (float)height/2.0,
									  0.0, 0.0, (float)depth/2.0, (float)depth/2.0,
									  0.0, 0.0, 0.0, 1.0);

	initialized = false;

	color_buffer = new unsigned char[width*height*3];
	depth_buffer = new unsigned short[width*height];

	screen_texture = 0;
	screen_geometry = 0;
	screen_shader = 0;

	active_matrix = modelview_matrix;

	for (int i = 0; i < 4; i++)
		matrices[i] = identity<float, 4, 4>();

	polygon_mode = line;
	shade_model = smooth;
	culling = backface;

	clipping_planes = vector<vec6f>();
	clipping_planes.push_back(vec6f(-1.0, 0.0, 0.0, 1.0, 0.0, 0.0));
	clipping_planes.push_back(vec6f(1.0, 0.0, 0.0, -1.0, 0.0, 0.0));
	clipping_planes.push_back(vec6f(0.0, -1.0, 0.0, 0.0, 1.0, 0.0));
	clipping_planes.push_back(vec6f(0.0, 1.0, 0.0, 0.0, -1.0, 0.0));
	clipping_planes.push_back(vec6f(0.0, 0.0, -1.0, 0.0, 0.0, 1.0));
	clipping_planes.push_back(vec6f(0.0, 0.0, 1.0, 0.0, 0.0, -1.0));



}

canvashdl::~canvashdl()
{
	if (color_buffer != NULL)
	{
		delete [] color_buffer;
		color_buffer = NULL;
	}

	if (depth_buffer != NULL)
	{
		delete [] depth_buffer;
		depth_buffer = NULL;
	}
}

void canvashdl::clear_color_buffer()
{
	memset(color_buffer, 0, width*height*3*sizeof(unsigned char));
}

void canvashdl::clear_depth_buffer()
{
	memset(depth_buffer, 255, width*height*sizeof(unsigned short));
}

void canvashdl::reallocate(int w, int h)
{
	last_reshape_time = -1.0;

	if (color_buffer != NULL)
	{
		delete [] color_buffer;
		color_buffer = NULL;
	}

	if (depth_buffer != NULL)
	{
		delete [] depth_buffer;
		depth_buffer = NULL;
	}

	width = w;
	height = h;

	color_buffer = new unsigned char[w*h*3];
	depth_buffer = new unsigned short[w*h];

	glActiveTexture(GL_TEXTURE0);
	check_error(__FILE__, __LINE__);
	glBindTexture(GL_TEXTURE_2D, screen_texture);
	check_error(__FILE__, __LINE__);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, color_buffer);
	check_error(__FILE__, __LINE__);
}

/* set_matrix
 *
 * Change which matrix is active.
 */
void canvashdl::set_matrix(matrix_id matid)
{
	// DONE Assignment 1: Change which matrix is active.
	active_matrix = matid;
}

/* load_identity
 *
 * Set the active matrix to the identity matrix.
 * This implements: https://www.opengl.org/sdk/docs/man2/xhtml/glLoadIdentity.xml
 */
void canvashdl::load_identity()
{
	// DONE Assignment 1: Set the active matrix to the identity matrix.
	matrices[active_matrix] = identity<float, 4, 4> ();
}

/* rotate
 *
 * Multiply the active matrix by a rotation matrix.
 * This implements: https://www.opengl.org/sdk/docs/man2/xhtml/glRotate.xml
 */
void canvashdl::rotate(float angle, vec3f axis)
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

	matrices[active_matrix] = matrices[active_matrix] * rotate_mat;
}

/* translate
 *
 * Multiply the active matrix by a translation matrix.
 * This implements: https://www.opengl.org/sdk/docs/man2/xhtml/glTranslate.xml
 */
void canvashdl::translate(vec3f direction)
{
	// Done Assignment 1: Multiply the active matrix by a translation matrix.
	float x = direction.data[0];
	float y = direction.data[1];
	float z = direction.data[2];

	mat4f translate_mat = mat4f( 1.0, 0.0, 0.0, x,
							  	 0.0, 1.0, 0.0, y,
								 0.0, 0.0, 1.0, z,
								 0.0, 0.0, 0.0, 1.0);

	matrices[active_matrix] = matrices[active_matrix] * translate_mat;

}

/* scale
 *
 * Multiply the active matrix by a scaling matrix.
 * This implements: https://www.opengl.org/sdk/docs/man2/xhtml/glScale.xml
 */
void canvashdl::scale(vec3f size)
{
	// Done Assignment 1: Multiply the active matrix by a scaling matrix.

	float x = size.data[0];
	float y = size.data[1];
	float z = size.data[2];

	mat4f scale_mat = mat4f( x, 0.0, 0.0, 0.0,
							  0.0, y, 0.0, 0.0,
							  0.0, 0.0, z, 0.0,
							  0.0, 0.0, 0.0, 1.0);

	matrices[active_matrix] = matrices[active_matrix] * scale_mat;
}

/* perspective
 *
 * Multiply the active matrix by a perspective projection matrix.
 * This implements: https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml
 */
void canvashdl::perspective(float fovy, float aspect, float n, float f)
{
	// Done Assignment 1: Multiply the active matrix by a perspective projection matrix.
	float fc = 1.0 / tan(fovy/2.0);

	mat4f perspective_mat = mat4f( fc/aspect, 0.0,   0.0,			  0.0,
							  0.0, 	     fc,  0.0,			  0.0,
							  0.0, 		 0.0,   (f+n)/(n-f), (2*f*n)/(n-f),
							  0.0, 		 0.0,   -1.0, 		  0.0);

	matrices[active_matrix] =  matrices[active_matrix] * perspective_mat;
}

/* frustum
 *
 * Multiply the active matrix by a frustum projection matrix.
 * This implements: https://www.opengl.org/sdk/docs/man2/xhtml/glFrustum.xml
 */
void canvashdl::frustum(float l, float r, float b, float t, float n, float f)
{
	// Done Assignment 1: Multiply the active matrix by a frustum projection matrix.
	assert(r>l);
	assert(t>b);
	assert(f>n);

	float A = (r+l)/(r-l);
	float B = (t+b)/(t-b);
	float C = - (f+n)/(f-n);
	float D = - 2.0*f*n / (f-n);


	mat4f frustum_mat = mat4f( 2.0*n / (r-l), 0.0, A, 0.0,
							   0.0, 2.0*n/(t-b), B, 0.0,
							   0.0, 0.0, C, D,
							   0.0, 0.0, -1.0, 0.0);

	matrices[active_matrix] =  matrices[active_matrix] * frustum_mat;
}

/* ortho
 *
 * Multiply the active matrix by an orthographic projection matrix.
 * This implements: https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml
 */
void canvashdl::ortho(float l, float r, float b, float t, float n, float f)
{
	// Done Assignment 1: Multiply the active matrix by an orthographic projection matrix.
	assert(r>l);
	assert(t>b);
	assert(f>n);

	float tx = - (r+l)/(r-l);
	float ty = - (t+b)/(t-b);
	float tz = - (f+n)/(f-n);


	mat4f ortho_mat = mat4f( 2.0 / (r-l), 0.0, 0.0, tx,
							   0.0, 2.0 / (t-b), 0.0, ty,
							   0.0, 0.0, -2.0 / (f-n), tz,
							   0.0, 0.0, 0.0, 1.0);

	matrices[active_matrix] =  matrices[active_matrix] * ortho_mat;
}

void canvashdl::viewport(int left, int bottom, int right, int top)
{
	matrices[viewport_matrix] = mat4f((float)(right - left)/2.0, 0.0, 0.0, (float)(right + left)/2.0,
									  0.0, (float)(top - bottom)/2.0, 0.0, (float)(top + bottom)/2.0,
									  0.0, 0.0, (float)depth/2.0, (float)depth/2.0,
									  0.0, 0.0, 0.0, 1.0);

	resize(right - left, top - bottom);
}

/* look_at
 *
 * Move and orient the modelview so the camera is at the 'eye' position focused on the 'at'
 * position and rotated so the 'up' vector is up
 * This implements: https://www.opengl.org/sdk/docs/man2/xhtml/gluLookAt.xml
 */
void canvashdl::look_at(vec3f eye, vec3f at, vec3f up)
{
	// Done Assignment 1: Emulate the functionality of gluLookAt

	vec3f F = at - eye;
	vec3f f = norm(F);
	vec3f nup = norm(up);
	vec3f s = cross(f, nup);
	vec3f u = cross(norm(s), f);
	mat4f M = mat4f(s.data[0], s.data[1], s.data[2], 0.0,
					u.data[0], u.data[1], u.data[2], 0.0,
					-f.data[0], -f.data[1], -f.data[2], 0.0,
					0.0, 0.0, 0.0, 1.0);

	mat4f translate_mat = mat4f( 1.0, 0.0, 0.0, -eye[0],
					  		     0.0, 1.0, 0.0, -eye[1],
							     0.0, 0.0, 1.0, -eye[2],
							     0.0, 0.0, 0.0, 1.0);

	matrices[active_matrix] = matrices[active_matrix] * M * translate_mat;

}

void canvashdl::update_normal_matrix()
{
	// DONE Assignment 3: calculate the normal matrix
	matrices[normal_matrix] = transpose(inverse(matrices[modelview_matrix]));
}

/* to_window
 *
 * Given a pixel coordinate (x from 0 to width and y from 0 to height),
 * convert it into window coordinates (x from -1 to 1 and y from -1 to 1).
 */
vec3f canvashdl::to_window(vec2i pixel)
{
	/* Done Assignment 1: Given a pixel coordinate (x from 0 to width and y from 0 to height),
	 * convert it into window coordinates (x from -1 to 1 and y from -1 to 1).
	 */

	float x = ((float) pixel.data[0] / (float) width - 0.5 ) * 2.0;
	float y = (0.5 - (float) pixel.data[1] / (float) height) * 2.0;

	return vec3f(x, y, 1.04);
}

vec3i canvashdl::to_pixel(vec3f window_cordinate)
{

	int x = ((window_cordinate.data[0] / 2.0) + 0.5) * width;
	int y = ((window_cordinate.data[1] / 2.0) + 0.5) * height;

	return vec3i(x, y, 0);
}

/* unproject
 *
 * Unproject a window coordinate into world coordinates.
 * This implements: https://www.opengl.org/sdk/docs/man2/xhtml/gluUnProject.xml
 */
vec3f canvashdl::unproject(vec3f window)
{
	// Done Assignment 1: Unproject a window coordinate into world coordinates.

	vec4f temp = vec4f(window.data[0], window.data[1], window.data[2], 1.0);
	mat4f invPM = inverse(matrices[modelview_matrix]) * inverse(matrices[projection_matrix]);
	vec4f obj = invPM * temp;

	return vec3f(obj.data[0], obj.data[1], obj.data[2]);
}

/* shade_vertex
 *
 * This is the vertex shader.
 * v[0] to v[2] is position
 * v[3] to v[5] is normal
 * v[7] to v[8] is texture coordinates
 * The result from this function is interpolated and passed on to the fragment shader
 * (its also used to draw the geometry during rasterization)
 * Note that the only requirements for the returned values are that the first 3 components
 * be a projected position. The rest are yours to decide what to do with.
 */
vec3f canvashdl::shade_vertex(vec8f v, vector<float> &varying)
{
	// DONE Assignment 1: Do all of the necessary transformations (normal, projection, modelview, etc)
	/* DONE Assignment 3: Get the material from the list of uniform variables and
	 * call its vertex shader.
	 */
	const materialhdl* m;
	//cout<<"QQ"<<endl;
	get_uniform("current_material", m);
	//cout<<m<<endl;
	return m -> shade_vertex(this, vec3f(v), vec3f(v.data[3], v.data[4], v.data[5]), varying);
}

/* shade_fragment
 *
 * This is the fragment shader. The pixel color is determined here.
 * the values for v are the interpolated result of whatever you returned from the vertex shader
 */
vec3f canvashdl::shade_fragment(vector<float> varying)
{
	// Done Assignment 1: Pick a color, any color (as long as it is distinguishable from the background color).
	//return vec3f(1.0, 1.0, 1.0);
	const materialhdl* m;
	get_uniform("current_material", m);
	return m -> shade_fragment(this, varying);
	/* DONE Assignment 3: Get the material from the list of uniform variables and
	 * call its fragment shader.
	 */

}

/* plot
 *
 * Plot a pixel and check it against the depth buffer.
 */
void canvashdl::plot(vec3i xyz, vector<float> varying)
{
	// Done Assignment 1: Plot a pixel, calling the fragment shader.

	int x = xyz.data[0];
	int y = xyz.data[1];
	int z = xyz.data[2];

	/* DONE Assignment 3: Compare the z value against the depth buffer and
	 * only render if its less. Then set the depth buffer.
	 */
	if(x<0 || x>=width || y<0 || y>=height || depth_buffer[(width*y + x)] < z)
		return;

	vec3f frag = shade_fragment(varying);
	color_buffer[ 3* (width*y + x)] = 255 * frag.data[0];
	color_buffer[ 3* (width*y + x) + 1] = 255 * frag.data[1];
	color_buffer[ 3* (width*y + x) + 2] = 255* frag.data[2];
	depth_buffer[(width*y + x)] = z;

}

/* plot_point
 *
 * Plot a point given in window coordinates.
 */
void canvashdl::plot_point(vec3f v, vector<float> varying)
{
	// Done Assignment 1: Plot a point given in window coordinates.
//	if( (v.data[0] < 1 || v.data[0]> -1) && (v.data[1] < 1 || v.data[1]> -1))
	plot(to_pixel(v), varying);
}
vec5i canvashdl::pre_plot(vec3i v1_pixel, vec3i v2_pixel){
	int xmin, xmax;
	int ymin, ymax;
	int octant;
	float slope = (float)(v2_pixel.data[1] - v1_pixel.data[1])/ (float)(v2_pixel.data[0] - v1_pixel.data[0]);

	// compare v1x and v2x and then decide the octant the point belongs to
	if (v1_pixel.data[0] <= v2_pixel.data[0]){
		if (slope > 1) octant = 1;
		else if (slope < -1) octant = 6;
		else if (slope > 0) octant = 0;
		else octant = 7;

	}
	else{
		if (slope > 1) octant = 5;
		else if (slope < -1) octant = 2;
		else if (slope > 0) octant = 4;
		else octant = 3;
	}
	// tranform the points to the 1st octant. (0 <slope < 1, x,y>= 0)
	vec2i v1_t = beforeBreseham(octant, v1_pixel.data[0], v1_pixel.data[1]);
	vec2i v2_t = beforeBreseham(octant, v2_pixel.data[0], v2_pixel.data[1]);
	// find the start and end (x,y)
	xmin = v1_t.data[0]; xmax = v2_t.data[0];
	ymin = v1_t.data[1]; ymax = v2_t.data[1];
	return vec5i(v1_t.data[0], v2_t.data[0], v1_t.data[1], v2_t.data[1], octant);
}

void canvashdl::bresenham(int xmin, int xmax, int ymin, int ymax, int octant, vector<float> v1_varying, vector<float> v2_varying){
	//Breseham's algorithm for plot line
	int delta_y = ymax - ymin;
	int delta_x = xmax - xmin;
	int y = ymin; int x = xmin;
	int d = 2*delta_y - delta_x;
	//cout<<delta_x<<endl;
	while(x < xmax){
		x++;
		if (d <= 0){
			d += 2*delta_y;
		}
		else{
			y++;
			d += 2*delta_y - 2*delta_x;
		}

		// tranform the cordinate back to original octant
		float ratio = (float)(x-xmin)/(float)(delta_x);
		assert (ratio <= 1 && ratio >= 0);
		vec2i xy_plot = afterBreseham(octant, x, y);
		vec3i xyz = vec3i(xy_plot.data[0], xy_plot.data[1], 0);
		// use plot method
		vec3f color1 = vec3f(v1_varying[0], v1_varying[1],v1_varying[2]);
		vec3f color2 = vec3f(v2_varying[0], v2_varying[1],v2_varying[2]);
		vector<float> plotcolor = vector<float>();
		for (int i = 0; i < 3; i++){
			plotcolor.push_back(ratio*(color2.data[i]) + (1.0-ratio)*color1.data[i]);
		}
		plot(xyz, plotcolor);
	}
}
void canvashdl::bresenham_halftri(int &x, int &y , int xmax, int ymax, int octant, int delta_x, int delta_y,vector<float> v1_varying,  int &d){
	//Breseham's algorithm for plot line


	bool octant_flag = (octant ==0 || octant == 3 || octant == 4 || octant == 7);
	bool stop_flag = false;
	while(x < xmax){
		x++; if(!octant_flag) stop_flag = true;
		if (d <= 0){
			d += 2*delta_y;
		}
		else{
			d += 2*delta_y - 2*delta_x;
			y++; if(octant_flag) stop_flag = true;
		}

		// tranform the cordinate back to original octant
		vec2i xy_plot = afterBreseham(octant, x, y);
		vec3i xyz = vec3i(xy_plot.data[0], xy_plot.data[1], 0);
		// use plot method
		plot(xyz, v1_varying);
		if (stop_flag) return;
	}
	return;
}
/* plot_line
 *
 * Plot a line defined by two points in window coordinates. Use Bresenham's
 * Algorithm for this. Don't forget to interpolate the normals and texture
 * coordinates as well.
 */
void canvashdl::plot_line(vec3f v1, vector<float> v1_varying, vec3f v2, vector<float> v2_varying)
{
	// Done Assignment 1: Implement Bresenham's Algorithm.

	vec3i v1_pixel = to_pixel(v1);
	vec3i v2_pixel = to_pixel(v2);
	vec5i xyminmaxoctant = pre_plot(v1_pixel, v2_pixel);
	//if(xyminmaxoctant.data[4] == 0 || xyminmaxoctant.data[4] == 3 ||xyminmaxoctant.data[4] == 4 || xyminmaxoctant.data[4] == 7)
	//{
		bresenham(xyminmaxoctant.data[0], xyminmaxoctant.data[1],
			  xyminmaxoctant.data[2], xyminmaxoctant.data[3], xyminmaxoctant.data[4], v1_varying, v2_varying);
	//}
	//else{
		//bresenham(xyminmaxoctant.data[0], xyminmaxoctant.data[1],
		//	  xyminmaxoctant.data[2], xyminmaxoctant.data[3], xyminmaxoctant.data[4], v2_varying, v1_varying);
	//}

	// TODO Assignment 3: Interpolate the varying values before passing them into plot.
}
vec2i canvashdl::beforeBreseham (int octant, int x, int y){
	switch(octant){
			case 0: return vec2i(x, y);
			case 1: return vec2i(y, x);
			case 2: return vec2i(y, -x);
			case 3: return vec2i(-x, y);
			case 4: return vec2i(-x, -y);
			case 5: return vec2i(-y, -x);
			case 6: return vec2i(-y, x);
			case 7: return vec2i(x, -y);
		default: return vec2i(x, y);
	}
}
vec2i canvashdl::afterBreseham(int octant, int x, int y){
	switch(octant){
		case 0: return vec2i(x, y);
		case 1: return vec2i(y, x);
		case 2: return vec2i(-y, x);
		case 3: return vec2i(-x, y);
		case 4: return vec2i(-x, -y);
		case 5: return vec2i(-y, -x);
		case 6: return vec2i(y, -x);
		case 7: return vec2i(x, -y);

		default: return vec2i(x, y);
	}
}
void canvashdl::sort3vertex(vec3i &v1, vector<float> &v1_varying, vec3i &v2, vector<float> &v2_varying, vec3i &v3, vector<float> &v3_varying){
	int a=0,b=1,c=2;
	vec3i tmp;
	for(int i =0; i < 2; i++){
	if(v1.data[1] > v2.data[1]){
		swap(v1,v2);
		swap(a,b);
		v1_varying.swap(v2_varying);
	}
	if(v2.data[1] > v3.data[1]){
		swap(v2,v3);
		swap(b,c);
		v2_varying.swap(v3_varying);
	}
	}
/*	if(v1.data[1] > v3.data[1]){
		swap(v1,v3);
		swap(a,c);
		v1_varying.swap(v3_varying);
	}*/
}
/* plot_half_triangle
 *
 * Plot half of a triangle defined by three points in window coordinates (v1, v2, v3).
 * The remaining inputs are as follows (s1, s2, s3) are the pixel coordinates of (v1, v2, v3),
 * and (ave) is the average value of the normal and texture coordinates for flat shading.
 * Use Bresenham's algorithm for this. You may plot the horizontal half or the vertical half.
 */
void canvashdl::plot_half_triangle(vec3i s1, vector<float> v1_varying, vec3i s2, vector<float> v2_varying, vec3i s3, vector<float> v3_varying, vector<float> ave_varying)
{
	// Done Assignment 2: Implement Bresenham's half triangle fill algorithm
	// vec3i order = sort3(s1.data[1], s2.data[1], s3.data[1]);


	vec5i xyminmaxoctant1 = pre_plot(s1, s2);
	int x1 = xyminmaxoctant1.data[0];
    int y1 = xyminmaxoctant1.data[2];
    int xmax1 = xyminmaxoctant1.data[1];
    int ymax1 = xyminmaxoctant1.data[3];
    int octant1 = xyminmaxoctant1.data[4];
	int delta_x1 = xmax1 - x1;
	int delta_y1 = ymax1 - y1;
	int d1 = 2*delta_y1 - delta_x1;

	vec5i xyminmaxoctant2 = pre_plot(s1, s3);
	int x2 = xyminmaxoctant2.data[0];
	int y2 = xyminmaxoctant2.data[2];
    int xmax2 = xyminmaxoctant2.data[1];
    int ymax2 = xyminmaxoctant2.data[3];
    int octant2 = xyminmaxoctant2.data[4];
	int delta_x2 = xmax2 - x2;
	int delta_y2 = ymax2 - y2;
	int d2 = 2*delta_y2 - delta_x2;

	vec2i xy_plot1;
	vec2i xy_plot2;

	do{
		// plot one edge return if y++
		bresenham_halftri(x1, y1, xmax1, ymax1, octant1, delta_x1, delta_y1, ave_varying, d1);
		// plot one edge return if y++
		bresenham_halftri(x2, y2, xmax2, ymax2, octant2, delta_x2, delta_y2, ave_varying, d2);
		// fill the scanline
		xy_plot1 = afterBreseham(octant1, x1, y1);
		xy_plot2 = afterBreseham(octant2, x2, y2);
		int xstart = (xy_plot1.data[0] < xy_plot2.data[0])? xy_plot1.data[0]: xy_plot2.data[0];
		int xend = (xy_plot1.data[0] < xy_plot2.data[0])? xy_plot2.data[0]: xy_plot1.data[0];
		for(int x = xstart; x <= xend; x++)
			plot(vec3i(x,xy_plot1.data[1], 0), v1_varying);
	//check if we finish half
	}while(xy_plot1.data[1] != s2.data[1]);

	// TODO Assignment 3: Interpolate the varying values before passing them into plot.
}

/* plot_triangle
 *
 * Use the above functions to plot a whole triangle. Don't forget to
 * take into account the polygon mode. You should be able to render the
 * triangle as 3 points, 3 lines, or a filled in triangle. (v1, v2, v3)
 * are given in window coordinates.
 */
void canvashdl::plot_triangle(vec3f v1, vector<float> v1_varying, vec3f v2, vector<float> v2_varying, vec3f v3, vector<float> v3_varying)
{
	/* DONE Assignment 1: Use the above functions to plot a whole triangle. Don't forget to
	 * take into account the polygon mode. You should be able to render the
	 * triangle as 3 points or 3 lines.
	 */
	
	if(polygon_mode == canvashdl::point){
		plot_point(v1, v1_varying);
		plot_point(v2, v2_varying);
		plot_point(v3, v3_varying);
	}
	else if(polygon_mode == canvashdl::line){
		plot_line(v1, v1_varying, v2, v2_varying);
		plot_line(v2, v2_varying, v3, v3_varying);
		plot_line(v3, v3_varying, v1, v1_varying);
	}
	else if(polygon_mode == canvashdl::fill){
		plot_line(v1, v1_varying, v2, v2_varying);
		plot_line(v2, v2_varying, v3, v3_varying);
		plot_line(v3, v3_varying, v1, v1_varying);
		vec3i v1_pixel = to_pixel(v1);
		vec3i v2_pixel = to_pixel(v2);
		vec3i v3_pixel = to_pixel(v3);

		vector<float> ave_varying = vector<float>();
		for(int i = 0; i < v1_varying.size(); i++){
			ave_varying.push_back((v1_varying[i] + v2_varying[i] + v3_varying[i]) / 3);
		}


		sort3vertex(v1_pixel, v1_varying, v2_pixel, v2_varying, v3_pixel, v3_varying);
		plot_half_triangle(v1_pixel, v1_varying, v2_pixel, v2_varying, v3_pixel, v3_varying, ave_varying);
		if (v2_pixel.data[1] != v3_pixel.data[1]){
			plot_half_triangle(v3_pixel, v3_varying, v2_pixel, v2_varying, v1_pixel, v1_varying, ave_varying);
		}


	}


	// TODO Assignment 2: Calculate the average varying vector for flat shading and call plot_half_triangle as needed.
}



bool canvashdl::is_inside(vec8f point, vec6f plane){
	vec3f n0 = vec3f(plane.data[0], plane.data[1], plane.data[2]);
	vec3f n = vec3f(plane.data[3], plane.data[4], plane.data[5]);

	return dot(vec3f(point) - n0, n) >= 0.0;
}

vec3f canvashdl::intersect_point(vec8f point1, vec8f point2, vec6f plane){
	vec3f n0 = vec3f(plane.data[0], plane.data[1], plane.data[2]);
	vec3f n = vec3f(plane.data[3], plane.data[4], plane.data[5]);
	vec8f vec12 = point2 - point1;

	float d = dot(n0-vec3f(point1), n) / dot(vec3f(vec12), n);

	return d * vec12 + point1;
}

void canvashdl::update_clipping_planes_helper(vec4f plane, int i){
	float n0 = -plane.data[3] / (plane.data[0] + plane.data[1] + plane.data[2]);
	vec3f n = norm(vec3f(plane.data[0], plane.data[1], plane.data[2]));

	clipping_planes[i].data[0] = n0;
	clipping_planes[i].data[1] = n0;
	clipping_planes[i].data[2] = n0;
	clipping_planes[i].data[3] = n.data[0];
	clipping_planes[i].data[4] = n.data[1];
	clipping_planes[i].data[5] = n.data[2];
}

void canvashdl::update_clipping_planes(){
	mat4f mp = matrices[projection_matrix] * matrices[modelview_matrix];

	vec4f col1 = mp.data[0];
	vec4f col2 = mp.data[1];
	vec4f col3 = mp.data[2];
	vec4f col4 = mp.data[3];
	vec4f plane;

	// Left
	plane = col1 + col4;
	update_clipping_planes_helper(plane, 0);

	// Right
	plane = -col1 + col4;
	update_clipping_planes_helper(plane, 1);

	// Bottom
	plane = col2 + col4;
	update_clipping_planes_helper(plane, 2);

	// Top
	plane = -col2 + col4;
	update_clipping_planes_helper(plane, 3);

	// Near
	plane = col3 + col4;
	update_clipping_planes_helper(plane, 4);

	// Far
	plane = -col3 + col4;
	update_clipping_planes_helper(plane, 5);

//	for(int p=0; p<6; p++){
//		cout << clipping_planes[p] << endl;
//	}
}


/* draw_points
 *
 * Draw a set of 3D points on the canvas. Each point in geometry is
 * formatted (vx, vy, vz, nx, ny, nz, s, t). Don't forget to test the
 * points against the clipping plains of the projection. If you don't
 * you'll get weird behavior (especially when objects behind the camera
 * are being rendered).
 */
void canvashdl::draw_points(const vector<vec8f> &geometry)
{
	// DONE Assignment 3: Update the normal matrix.
	update_normal_matrix();
	update_clipping_planes();
	// Done Assignment 1: call the vertex shader on the geometry, then pass it to plot_point
	vector<float> varying = vector<float>();

	for(int i=0; i<geometry.size(); i++){
		vec8f point = geometry[i];

		// Done Assignment 2: Implement frustum clipping
		for(int p=0; p<6; p++){
			vec6f plane = clipping_planes[p];

			if(is_inside(point, plane)){
				plot_point(shade_vertex( geometry[i], varying ), varying);
			}
		}
	}



}

/* draw_lines
 *
 * Draw a set of 3D lines on the canvas. Each point in geometry
 * is formatted (vx, vy, vz, nx, ny, nz, s, t). Don't forget to clip
 * the lines against the clipping planes of the projection. You can't
 * just not render them because you'll get some weird popping at the
 * edge of the view.
 */
void canvashdl::draw_lines(const vector<vec8f> &geometry, const vector<int> &indices)
{
	// DONE Assignment 3: Update the normal matrix.
	update_normal_matrix();
	update_clipping_planes();
	// Done Assignment 1: call the vertex shader on the geometry, then pass it to plot_line
	assert((indices.size()%2 == 0) && "canvas.draw_lines: indices size cannot be divided by 2");


	for(int i=0; i<indices.size()/2; i++){
		vector<float> varying1 = vector<float>();
		vector<float> varying2 = vector<float>();
		vec8f point1 = geometry[indices[2*i]];
		vec8f point2 = geometry[indices[2*i+1]];

		// Done Assignment 2: Implement frustum clipping
		bool both_outside = false;
		for(int p=0; p<6; p++){
			vec6f plane = clipping_planes[p];

			if(!is_inside(point1, plane) && !is_inside(point2, plane)){
				both_outside = true;
				break;
			} else if(is_inside(point1, plane) && !is_inside(point2, plane)){
				point2 = intersect_point(point1, point2, plane);
			} else if(!is_inside(point1, plane) && is_inside(point2, plane)){
				point1 = intersect_point(point1, point2, plane);
			}
		}
		if(!both_outside){
			vec3f vertex1 = shade_vertex(point1, varying1);
			vec3f vertex2 = shade_vertex(point2, varying2);
//			cout<<varying1.size()<<endl;
			plot_line(vertex1, varying1, vertex2, varying2);
		}

	}
}



/* draw_triangles
 * 
 * Draw a set of 3D triangles on the canvas. Each point in geometry is
 * formatted (vx, vy, vz, nx, ny, nz, s, t). Don't forget to clip the
 * triangles against the clipping planes of the projection. You can't
 * just not render them because you'll get some weird popping at the
 * edge of the view. Also, this is where font/back face culling is implemented.
 */
void canvashdl::draw_triangles(const vector<vec8f> &geometry, const vector<int> &indices)
{
	// DONE Assignment 3: Update the normal matrix.
	update_normal_matrix();
	update_clipping_planes();
	// Done Assignment 1: call the vertex shader on the geometry, then pass it to plot_triangle
	assert((indices.size()%3 == 0) && "canvas.draw_triangles: indices size cannot be divided by 3");

	vector<float> varying1 = vector<float>();
	vector<float> varying2 = vector<float>();
	vector<float> varying3 = vector<float>();


	for (int i = 2; i < indices.size(); i+=3){
		vec8f point1 = geometry[indices[i-2]];
		vec8f point2 = geometry[indices[i-1]];
		vec8f point3 = geometry[indices[i]];



		// Done Assignment 2: Implement back-face culling
		if(culling != disable){
			vec3f vec12 = shade_vertex(point2, varying2) - shade_vertex(point1, varying1);
			vec3f vec13 = shade_vertex(point3, varying3) - shade_vertex(point1, varying1);
			vec3f direction = cross(vec12, vec13);

			if( (direction.data[2] <= 0.0 && culling == frontface) ||
				(direction.data[2] >= 0.0 && culling == backface)	){
				continue;
			}
		}


		// Done Assignment 2: Implement frustum clipping
		vector<vec8f> points = vector<vec8f>();
		vector<vec8f> clipped_points = vector<vec8f>();
		points.push_back(point1);
		points.push_back(point2);
		points.push_back(point3);
		points.push_back(point1);

		for(int p=0; p<6; p++){

			vec6f plane = clipping_planes[p];
			clipped_points.clear();
			for(int j=0; j < (int)points.size()-1; j++){
				vec8f p1 = points[j];
				vec8f p2 = points[j+1];

				if(is_inside(p1, plane)){
					if(is_inside(p2, plane)){
						clipped_points.push_back(p2);
					} else{
						clipped_points.push_back(intersect_point(p1, p2, plane));
					}

				} else {
					if(is_inside(p2, plane)){
						clipped_points.push_back(intersect_point(p1, p2, plane));
						clipped_points.push_back(p2);
					}

				}
			}
			if(clipped_points.size() > 0)
				clipped_points.push_back(clipped_points[0]);

			points = clipped_points;

		}

		for(int j=0; j < (int)points.size() - 3; j++){
			vector<float> varying1 = vector<float>();
			vector<float> varying2 = vector<float>();
			vector<float> varying3 = vector<float>();

			vec3f p1 = shade_vertex(points[0], varying1);
			vec3f p2 = shade_vertex(points[j+1], varying2);
			vec3f p3 = shade_vertex(points[j+2], varying3);
			plot_triangle(p1, varying1, p2, varying2, p3, varying3);
		}


	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Do not edit anything below here, that code just sets up OpenGL to render a single
 * quad that covers the whole screen, applies the color_buffer as a texture to it, and
 * keeps the buffer size and texture up to date.
 */
void canvashdl::load_texture()
{
	glGenTextures(1, &screen_texture);
	check_error(__FILE__, __LINE__);
	glActiveTexture(GL_TEXTURE0);
	check_error(__FILE__, __LINE__);
	glBindTexture(GL_TEXTURE_2D, screen_texture);
	check_error(__FILE__, __LINE__);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	check_error(__FILE__, __LINE__);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	check_error(__FILE__, __LINE__);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	check_error(__FILE__, __LINE__);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	check_error(__FILE__, __LINE__);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	check_error(__FILE__, __LINE__);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, color_buffer);
	check_error(__FILE__, __LINE__);
}

void canvashdl::load_geometry()
{
	// x, y, s, t
	const GLfloat geometry[] = {
			-1.0, -1.0, 0.0, 0.0,
			 1.0, -1.0, 1.0, 0.0,
			-1.0,  1.0, 0.0, 1.0,
			-1.0,  1.0, 0.0, 1.0,
			 1.0, -1.0, 1.0, 0.0,
			 1.0,  1.0, 1.0, 1.0
	};

	glGenBuffers(1, &screen_geometry);
	glBindBuffer(GL_ARRAY_BUFFER, screen_geometry);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*6, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*4*6, geometry);
}

void canvashdl::load_shader()
{
	GLuint vertex = load_shader_file(working_directory + "res/canvas.vx", GL_VERTEX_SHADER);
	GLuint fragment = load_shader_file(working_directory + "res/canvas.ft", GL_FRAGMENT_SHADER);

	screen_shader = glCreateProgram();
	glAttachShader(screen_shader, vertex);
	glAttachShader(screen_shader, fragment);
	glLinkProgram(screen_shader);
}

void canvashdl::init_opengl()
{
	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, width, height);

	load_texture();
	load_geometry();
	load_shader();
	initialized = true;
}

void canvashdl::check_error(const char *file, int line)
{
	GLenum error_code;
	error_code = glGetError();
	if (error_code != GL_NO_ERROR)
		cerr << "error: " << file << ":" << line << ": " << gluErrorString(error_code) << endl;
}

double canvashdl::get_time()
{
	timeval gtime;
	gettimeofday(&gtime, NULL);
	return gtime.tv_sec + gtime.tv_usec*1.0E-6;
}

void canvashdl::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	last_reshape_time = get_time();
	reshape_width = w;
	reshape_height = h;
}

void canvashdl::swap_buffers()
{
	if (!initialized)
		init_opengl();

	if (last_reshape_time > 0.0 && get_time() - last_reshape_time > 0.125)
		resize(reshape_width, reshape_height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(screen_shader);
	check_error(__FILE__, __LINE__);

	glActiveTexture(GL_TEXTURE0);
	check_error(__FILE__, __LINE__);
	glBindTexture(GL_TEXTURE_2D, screen_texture);
	check_error(__FILE__, __LINE__);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, color_buffer);
	check_error(__FILE__, __LINE__);
	glUniform1i(glGetUniformLocation(screen_shader, "tex"), 0);
	check_error(__FILE__, __LINE__);

	glBindBuffer(GL_ARRAY_BUFFER, screen_geometry);
	check_error(__FILE__, __LINE__);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	check_error(__FILE__, __LINE__);
	glEnableClientState(GL_VERTEX_ARRAY);
	check_error(__FILE__, __LINE__);

	glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat)*4, (float*)(sizeof(GLfloat)*2));
	check_error(__FILE__, __LINE__);
	glVertexPointer(2, GL_FLOAT, sizeof(GLfloat)*4, NULL);
	check_error(__FILE__, __LINE__);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	check_error(__FILE__, __LINE__);

	glDisableClientState(GL_VERTEX_ARRAY);
	check_error(__FILE__, __LINE__);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	check_error(__FILE__, __LINE__);

	glutSwapBuffers();
	check_error(__FILE__, __LINE__);
}

int canvashdl::get_width()
{
	return width;
}

int canvashdl::get_height()
{
	return height;
}
