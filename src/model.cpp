#include "model.h"
#include "standard.h"

modelhdl::modelhdl()
{

}

modelhdl::modelhdl(string filename)
{
	load_obj(filename);
}

modelhdl::~modelhdl()
{

}

/* load_obj
 *
 * Load the a .obj file into the model.
 */
void modelhdl::load_obj(string filename)
{
	/* TODO Assignment 1: Load the .obj file located at 'filename'. You only
	 * need to handle the following commands:
	 * mtllib - the location of the material template library
	 * usemtl - use the material named X
	 * g - name this new rigid body X
	 * v - add a vertex to the current rigid body
	 * vn - add a normal to the current rigid body
	 * vt - add a texture coordinate to the current rigid body
	 * f - add a face to the current rigid body
	 *
	 * The underlying storage of data in an object file is slightly
	 * different from the one we are using here. You may change your
	 * representation if you like, but just remember to account for this
	 * in some way.
	 */
}

/* load_mtl
 *
 * Load the .mtl file located at 'filename'.
 */
void modelhdl::load_mtl(string filename)
{
	/* TODO Assignment 2: Load the .mtl file located at 'filename' into a list of materials.
	 * You should be able to at least handle the following commands:
	 * Ke - Set the emissive color
	 * Ka - Set the ambient color
	 * Kd - Set the diffuse color
	 * Ks - Set the specular color
	 * Ns - Set the material shininess
	 * mtlname - Set the name for this material
	 */
}
