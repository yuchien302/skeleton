#include "object.h"
#include "canvas.h"

rigidhdl::rigidhdl()
{

}

rigidhdl::~rigidhdl()
{

}

/* draw
 *
 * Draw a rigid body.
 */
void rigidhdl::draw(canvashdl *canvas)
{
	// TODO Assignment 1: Send the rigid body geometry to the renderer
}

objecthdl::objecthdl()
{
	position = vec3f(0.0, 0.0, 0.0);
	orientation = vec3f(0.0, 0.0, 0.0);
	bound = vec6f(1.0e6, -1.0e6, 1.0e6, -1.0e6, 1.0e6, -1.0e6);
	scale = 1.0;
}

objecthdl::~objecthdl()
{

}

/* draw
 *
 * Draw the model. Don't forget to apply the transformations necessary
 * for position, orientation, and scale.
 */
void objecthdl::draw(canvashdl *canvas)
{
	// TODO Assignment 1: Send transformations and geometry to the renderer to draw the object
}

/* draw_bound
 *
 * Create a representation for the bounding box and
 * render it.
 */
void objecthdl::draw_bound(canvashdl *canvas)
{
	/* TODO Assignment 1: Generate the geometry for the bounding box and send the necessary
	 * transformations and geometry to the renderer
	 */
}

/* draw_normals
 *
 * create a representation of the normals for this object.
 * If face is false, render the vertex normals. Otherwise,
 * calculate the normals for each face and render those.
 */
void objecthdl::draw_normals(canvashdl *canvas, bool face)
{
	/* TODO Assignment 1: Generate the geometry to display the normals and send the necessary
	 * transformations and geometry to the renderer
	 */
}
