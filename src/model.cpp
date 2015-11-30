/*
 * model.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: nbingham
 */

#include "model.h"
#include "common/standard.h"
#include "vrml/scene.h"
#include "common/hash.h"
#include "primitive.h"
#include "core/geometry.h"

modelhdl::modelhdl()
{

}

modelhdl::modelhdl(string filename)
{
	int dot = filename.find_last_of(".");
	if (dot >= 0 && dot < (int)filename.size())
	{
		if (filename.substr(dot) == ".obj")
			load_obj(filename);
		else if (filename.substr(dot) == ".wrl")
			load_wrl(filename);
	}
	else
		cout << "unrecognized file format" << endl;
}

modelhdl::~modelhdl()
{

}

/* load_obj
 *
 * Load the .obj file located at 'filename'. You only
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
void modelhdl::load_obj(string filename)
{
	int s = 4;
	float x, y, z;
	int v, n, t;
	char name[32];

	ifstream fin(filename.c_str());
	if (!fin.is_open())
	{
		cerr << "Error: file not found: " << filename << endl;
		return;
	}

	vector<vec3f> vertices;
	vector<vec3f> normals;
	vector<vec2f> texcoords;
	vec3f ave(0.0, 0.0, 0.0);
	float num = 0;

	string line(256, '\0');
	string command;
	while (getline(fin, line))
	{
		istringstream iss(line);

		if (iss >> command)
		{
			if (command == "mtllib")
			{
				string mtlname;
				iss >> mtlname;

				if (mtlname.size() > 0 && mtlname[0] != '/')
				{
					int idx = filename.find_last_of("/");
					if (idx == string::npos)
						idx = filename.find_last_of("\\");
					mtlname = filename.substr(0, idx) + "/" + mtlname;
				}

				load_mtl(mtlname);
			}
			else if (command == "g")
			{
				rigid.push_back(rigidhdl());
				if (rigid.size() > 1)
					rigid[rigid.size()-1].material = rigid[rigid.size()-2].material;
			}
			else if (command == "usemtl")
			{
				if (rigid.size() == 0)
					rigid.push_back(rigidhdl());

				iss >> rigid.back().material;
			}
			else if (command == "v" || command == "vn" || command == "vt" || command == "f")
			{
				if (rigid.size() == 0)
					rigid.push_back(rigidhdl());

				if (command == "v" && iss >> x >> y >> z)
				{
					vertices.push_back(vec3f(x, y, z));
					ave += vec3f(x, y, z);
					num += 1.0;
				}
				else if (command == "vn" && iss >> x >> y >> z)
					normals.push_back(vec3f(x, y, z));
				else if (command == "vt" && iss >> x >> y)
					texcoords.push_back(vec2f(x, y));
				else if (command == "f")
				{
					int first = rigid.back().geometry.size();
					string part;
					int i = 0;
					while (iss >> part)
					{
						vec8f point(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
						v = -1;
						n = -1;
						t = -1;

						if (sscanf(part.c_str(), "%d/%d/%d", &v, &t, &n) == 3)
						{
							point.set(0,3, vertices[v-1]);
							point.set(6,8, texcoords[t-1]);
							point.set(3,6, normals[n-1]);
						}
						else if (sscanf(part.c_str(), "%d//%d", &v, &n) == 2)
						{
							point.set(0,3, vertices[v-1]);
							point.set(3,6, normals[n-1]);
						}
						else if (sscanf(part.c_str(), "%d/%d", &v, &t) == 2)
						{
							point.set(0,3, vertices[v-1]);
							point.set(6,8, texcoords[t-1]);
						}
						else if (sscanf(part.c_str(), "%d", &v) == 1)
							point.set(0,3, vertices[v-1]);

						bound[0] = min(bound[0], point[0]);
						bound[1] = max(bound[1], point[0]);
						bound[2] = min(bound[2], point[1]);
						bound[3] = max(bound[3], point[1]);
						bound[4] = min(bound[4], point[2]);
						bound[5] = max(bound[5], point[2]);

						rigid.back().geometry.push_back(point);

						if (i >= 2)
						{
							rigid.back().indices.push_back(first);
							rigid.back().indices.push_back(rigid.back().geometry.size()-1);
							rigid.back().indices.push_back(rigid.back().geometry.size()-2);
						}
						i++;
					}
				}
			}
		}
	}

	ave /= num;

	for (int i = 0; i < 6; i++)
		bound[i] -= ave[i/2];

	for (int k = 0; k < rigid.size(); k++)
		for (int i = 0; i < rigid[k].geometry.size(); i++)
			for (int j = 0; j < 3; j++)
				rigid[k].geometry[i][j] -= ave[j];

	fin.close();
}

/* load_mtl
 *
 * Load the .mtl file located at 'filename'.
 */
void modelhdl::load_mtl(string filename)
{
	ifstream fin(filename.c_str());
	if (!fin.is_open())
	{
		cerr << "Error: file not found: " << filename << endl;
		return;
	}

	string current_material = "";
	string type = "";
	string line(256, '\0');
	string command;
	while (getline(fin, line))
	{
		istringstream iss(line);

		if (iss >> command)
		{
			if (command == "newmtl")
			{
				iss >> type;
				iss >> current_material;
				if (type == "white")
					material[current_material] = new whitehdl();
				else if (type == "gouraud")
					material[current_material] = new gouraudhdl();
				else if (type == "phong")
					material[current_material] = new phonghdl();
				else if (type == "custom")
					material[current_material] = new customhdl();
				else if (type == "gouraud")
					material[current_material] = new texturehdl();
			}
			else if (command == "Ke" && type == "gouraud")
				iss >> ((gouraudhdl*)material[current_material])->emission[0] >> ((gouraudhdl*)material[current_material])->emission[1] >> ((gouraudhdl*)material[current_material])->emission[2];
			else if (command == "Ka" && type == "gouraud")
				iss >> ((gouraudhdl*)material[current_material])->ambient[0] >> ((gouraudhdl*)material[current_material])->ambient[1] >> ((gouraudhdl*)material[current_material])->ambient[2];
			else if (command == "Kd" && type == "gouraud")
				iss >> ((gouraudhdl*)material[current_material])->diffuse[0] >> ((gouraudhdl*)material[current_material])->diffuse[1] >> ((gouraudhdl*)material[current_material])->diffuse[2];
			else if (command == "Ks" && type == "gouraud")
				iss >> ((gouraudhdl*)material[current_material])->specular[0] >> ((gouraudhdl*)material[current_material])->specular[1] >> ((gouraudhdl*)material[current_material])->specular[2];
			else if (command == "Ns" && type == "gouraud")
				iss >> ((gouraudhdl*)material[current_material])->shininess;
			else if (command == "Ke" && type == "phong")
				iss >> ((phonghdl*)material[current_material])->emission[0] >> ((phonghdl*)material[current_material])->emission[1] >> ((phonghdl*)material[current_material])->emission[2];
			else if (command == "Ka" && type == "phong")
				iss >> ((phonghdl*)material[current_material])->ambient[0] >> ((phonghdl*)material[current_material])->ambient[1] >> ((phonghdl*)material[current_material])->ambient[2];
			else if (command == "Kd" && type == "phong")
				iss >> ((phonghdl*)material[current_material])->diffuse[0] >> ((phonghdl*)material[current_material])->diffuse[1] >> ((phonghdl*)material[current_material])->diffuse[2];
			else if (command == "Ks" && type == "phong")
				iss >> ((phonghdl*)material[current_material])->specular[0] >> ((phonghdl*)material[current_material])->specular[1] >> ((phonghdl*)material[current_material])->specular[2];
			else if (command == "Ns" && type == "phong")
				iss >> ((phonghdl*)material[current_material])->shininess;

		}
	}
}



vector<vec3f> interpret_properties(mat4f model, mat3f norm, const parse_vrml::node &syntax, map<string, definition> &names, vector<string> parent)
{
	vector<vec3f> result;
	if (syntax.nodeType == "Coordinate")
	{
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "point" && syntax.nodeGuts[i].value.type == "MFVec3f")
			{
				for (int j = 0; j < (int)syntax.nodeGuts[i].value.values.size(); j++)
				{
					vec4f coord(0.0f, 0.0f, 0.0f, 1.0f);
					for (int k = 0; k < (int)syntax.nodeGuts[i].value.values[j].values.size() && k < 3; k++)
						coord[k] = atof(syntax.nodeGuts[i].value.values[j].values[k].c_str());
					//coord = model*coord;
					result.push_back((vec3f)coord/coord[3]);
				}
			}
		}
	}
	else if (syntax.nodeType == "Normal")
	{
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "vector" && syntax.nodeGuts[i].value.type == "MFVec3f")
			{
				for (int j = 0; j < (int)syntax.nodeGuts[i].value.values.size(); j++)
				{
					vec3f coord(0.0f, 0.0f, 0.0f);
					for (int k = 0; k < (int)syntax.nodeGuts[i].value.values[j].values.size() && k < 3; k++)
						coord[k] = atof(syntax.nodeGuts[i].value.values[j].values[k].c_str());
					//coord = norm*coord;
					result.push_back(coord);
				}
			}
		}
	}
	else if (syntax.nodeType == "TextureCoordinate")
	{
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "point" && syntax.nodeGuts[i].value.type == "MFVec2f")
			{
				for (int j = 0; j < (int)syntax.nodeGuts[i].value.values.size(); j++)
				{
					vec3f coord(0.0f, 0.0f, 0.0f);
					for (int k = 0; k < (int)syntax.nodeGuts[i].value.values[j].values.size() && k < 2; k++)
						coord[k] = atof(syntax.nodeGuts[i].value.values[j].values[k].c_str());
					result.push_back(coord);
				}
			}
		}
	}

	return result;
}

vector<vec3f> interpret_properties(mat4f model, mat3f norm, const parse_vrml::nodeDeclaration &syntax, map<string, definition> &names, vector<string> parent)
{
	if (syntax.cmd == "USE" || syntax.content.nodeType == "Coordinate" || syntax.content.nodeType == "Normal" || syntax.content.nodeType == "TextureCoordinate")
	{
		if (syntax.cmd == "DEF")
		{
			names[syntax.name].model = model;
			names[syntax.name].norm = norm;
			names[syntax.name].syntax = syntax.content;
			parent.push_back(syntax.name);
			return interpret_properties(model, norm, syntax.content, names, parent);
		}
		else if (syntax.cmd == "USE")
		{
			map<string, definition>::iterator loc = names.find(syntax.name);
			parent.push_back(syntax.name);
			if (loc != names.end())
				return interpret_properties(model*loc->second.model, norm*loc->second.norm, loc->second.syntax, names, parent);
			else
				error("", "undefined node \"" + syntax.name + "\"", __FILE__, __LINE__);
		}
		else
		{
			return interpret_properties(model, norm, syntax.content, names, parent);
		}
	}
	return vector<vec3f>();
}

rigidhdl interpret_geometry(mat4f model, mat3f norm, const parse_vrml::node &syntax, map<string, definition> &names, vector<string> parent)
{
	rigidhdl result;
	if (syntax.nodeType == "IndexedFaceSet")
	{
		vector<vec3f> coord;
		vector<vec3f> normal;
		vector<vec3f> texCoord;
		vector<int> coordIndex;
		vector<int> normalIndex;
		vector<int> texCoordIndex;
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "coord" && syntax.nodeGuts[i].value.type == "SFNode" &&
				syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].node != NULL)
				coord = interpret_properties(model, norm, *syntax.nodeGuts[i].value.values[0].node, names, parent);
			else if (syntax.nodeGuts[i].first == "normal" && syntax.nodeGuts[i].value.type == "SFNode" &&
				syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].node != NULL)
				normal = interpret_properties(model, norm, *syntax.nodeGuts[i].value.values[0].node, names, parent);
			else if (syntax.nodeGuts[i].first == "texCoord" && syntax.nodeGuts[i].value.type == "SFNode" &&
				syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].node != NULL)
				texCoord = interpret_properties(model, norm, *syntax.nodeGuts[i].value.values[0].node, names, parent);
			else if (syntax.nodeGuts[i].first == "coordIndex" && syntax.nodeGuts[i].value.type == "MFInt32")
				for (int j = 0; j < (int)syntax.nodeGuts[i].value.values.size();)
				{
					vector<int> indices;

					bool done = false;
					while (!done)
					{
						if (syntax.nodeGuts[i].value.values[j].values.size() > 0)
						{
							int index = atoi(syntax.nodeGuts[i].value.values[j].values[0].c_str());
							if (index >= 0)
								indices.push_back(index);
							else
								done = true;
						}

						j++;
					}

					for (int l = 0; l < (int)indices.size()-2; l++)
					{
						coordIndex.push_back(indices[0]);
						coordIndex.push_back(indices[l+1]);
						coordIndex.push_back(indices[l+2]);
					}
				}
			else if (syntax.nodeGuts[i].first == "normalIndex" && syntax.nodeGuts[i].value.type == "MFInt32")
				for (int j = 0; j < (int)syntax.nodeGuts[i].value.values.size();)
				{
					vector<int> indices;

					bool done = false;
					while (!done)
					{
						if (syntax.nodeGuts[i].value.values[j].values.size() > 0)
						{
							int index = atoi(syntax.nodeGuts[i].value.values[j].values[0].c_str());
							if (index >= 0)
								indices.push_back(index);
							else
								done = true;
						}

						j++;
					}

					for (int l = 0; l < (int)indices.size()-2; l++)
					{
						normalIndex.push_back(indices[0]);
						normalIndex.push_back(indices[l+1]);
						normalIndex.push_back(indices[l+2]);
					}
				}
			else if (syntax.nodeGuts[i].first == "texCoordIndex" && syntax.nodeGuts[i].value.type == "MFInt32")
				for (int j = 0; j < (int)syntax.nodeGuts[i].value.values.size();)
				{
					vector<int> indices;

					bool done = false;
					while (!done)
					{
						if (syntax.nodeGuts[i].value.values[j].values.size() > 0)
						{
							int index = atoi(syntax.nodeGuts[i].value.values[j].values[0].c_str());
							if (index >= 0)
								indices.push_back(index);
							else
								done = true;
						}

						j++;
					}

					for (int l = 0; l < (int)indices.size()-2; l++)
					{
						texCoordIndex.push_back(indices[0]);
						texCoordIndex.push_back(indices[l+1]);
						texCoordIndex.push_back(indices[l+2]);
					}
				}
		}

		map<vec3f, int> geomap;
		for (int i = 0; i < (int)coordIndex.size(); i++)
		{
			vec8f point(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			point[0] = coord[coordIndex[i]][0];
			point[1] = coord[coordIndex[i]][1];
			point[2] = coord[coordIndex[i]][2];

			if (coordIndex.size() == normalIndex.size() && normal.size() > 0)
			{
				point[3] = normal[normalIndex[i]][0];
				point[4] = normal[normalIndex[i]][1];
				point[5] = normal[normalIndex[i]][2];
			}

			if (i < (int)texCoordIndex.size())
			{
				point[6] = texCoord[texCoordIndex[i]][0];
				point[7] = texCoord[texCoordIndex[i]][1];
			}

			map<vec3f, int>::iterator loc = geomap.find(vec3f(point));
			if (loc != geomap.end())
				result.indices.push_back(loc->second);
			else
			{
				result.indices.push_back(result.geometry.size());
				result.geometry.push_back(point);
			}
		}

		if (normalIndex.size() < coordIndex.size() || normal.size() == 0)
		{
			for (int i = 0; i < (int)result.geometry.size(); i++)
			{
				result.geometry[i][3] = 0.0f;
				result.geometry[i][4] = 0.0f;
				result.geometry[i][5] = 0.0f;
			}

			for (int i = 0; i < (int)result.indices.size(); i+=3)
			{
				vec3f n0 = ::norm((vec3f)result.geometry[result.indices[i+1]] - (vec3f)result.geometry[result.indices[i+0]]);
				vec3f n1 = ::norm((vec3f)result.geometry[result.indices[i+2]] - (vec3f)result.geometry[result.indices[i+0]]);
				vec3f n = cross(n0, n1);
				for (int j = 0; j < 3; j++)
				{
					result.geometry[result.indices[i+j]][3] += n[0];
					result.geometry[result.indices[i+j]][4] += n[1];
					result.geometry[result.indices[i+j]][5] += n[2];
				}
			}

			for (int i = 0; i < (int)result.geometry.size(); i++)
			{
				vec3f n = ::norm((vec3f)result.geometry[i](3, 6));
				result.geometry[i][3] = n[0];
				result.geometry[i][4] = n[1];
				result.geometry[i][5] = n[2];
			}
		}
	}
	else if (syntax.nodeType == "Box")
	{
		vec3f dimensions(2.0f, 2.0f, 2.0f);
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
			if (syntax.nodeGuts[i].first == "size" && syntax.nodeGuts[i].value.type == "SFVec3f")
				if (syntax.nodeGuts[i].value.values.size() > 0)
					for (int j = 0; j < (int)syntax.nodeGuts[i].value.values[0].values.size() && j < 3; j++)
						dimensions[j] = atof(syntax.nodeGuts[i].value.values[0].values[j].c_str());

		boxhdl b(dimensions[0], dimensions[1], dimensions[2]);
		for (int i = 0; i < b.rigid.size(); i++)
		{
			for (int j = 0; j < (int)b.rigid.back().geometry.size(); j++)
			{
				b.rigid.back().geometry[j].set(0,3, model*homogenize(b.rigid.back().geometry[j]));
				b.rigid.back().geometry[j].set(3,6, norm*(vec3f)b.rigid.back().geometry[j](3,6));
			}
		}
		result = b.rigid[0];
	}
	else if (syntax.nodeType == "Cone")
	{
		float radius = 1.0f, height = 2.0f;
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "bottomRadius" && syntax.nodeGuts[i].value.type == "SFFloat")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].values.size() > 0)
					radius = atof(syntax.nodeGuts[i].value.values[0].values[0].c_str());
			}
			else if (syntax.nodeGuts[i].first == "height" && syntax.nodeGuts[i].value.type == "SFFloat")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].values.size() > 0)
					height = atof(syntax.nodeGuts[i].value.values[0].values[0].c_str());
			}
		}

		pyramidhdl b(radius, height, 8);
		for (int i = 0; i < b.rigid.size(); i++)
		{
			for (int j = 0; j < (int)b.rigid.back().geometry.size(); j++)
			{
				b.rigid.back().geometry[j].set(0,3, model*homogenize(b.rigid.back().geometry[j]));
				b.rigid.back().geometry[j].set(3,6, norm*(vec3f)b.rigid.back().geometry[j](3,6));
			}
		}
		result = b.rigid[0];
	}
	else if (syntax.nodeType == "Cylinder")
	{
		float radius = 1.0f, height = 2.0f;
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "radius" && syntax.nodeGuts[i].value.type == "SFFloat")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].values.size() > 0)
					radius = atof(syntax.nodeGuts[i].value.values[0].values[0].c_str());
			}
			else if (syntax.nodeGuts[i].first == "height" && syntax.nodeGuts[i].value.type == "SFFloat")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].values.size() > 0)
					height = atof(syntax.nodeGuts[i].value.values[0].values[0].c_str());
			}
		}

		cylinderhdl b(radius, height, 8);
		for (int i = 0; i < b.rigid.size(); i++)
		{
			for (int j = 0; j < (int)b.rigid.back().geometry.size(); j++)
			{
				b.rigid.back().geometry[j].set(0,3, model*homogenize(b.rigid.back().geometry[j]));
				b.rigid.back().geometry[j].set(3,6, norm*vec3f(b.rigid.back().geometry[j][3], b.rigid.back().geometry[j][4], b.rigid.back().geometry[j][5]));
			}
		}
		result = b.rigid[0];
	}
	else if (syntax.nodeType == "Sphere")
	{
		float radius = 1.0f;
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "radius" && syntax.nodeGuts[i].value.type == "SFFloat")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].values.size() > 0)
					radius = atof(syntax.nodeGuts[i].value.values[0].values[0].c_str());
			}
		}

		spherehdl b(radius, 8, 8);
		for (int i = 0; i < b.rigid.size(); i++)
		{
			for (int j = 0; j < (int)b.rigid.back().geometry.size(); j++)
			{
				b.rigid.back().geometry[j].set(0,3, model*homogenize(b.rigid.back().geometry[j]));
				b.rigid.back().geometry[j].set(3,6, norm*(vec3f)b.rigid.back().geometry[j](3,6));
			}
		}
		result = b.rigid[0];
	}

	return result;
}

rigidhdl interpret_geometry(mat4f model, mat3f norm, const parse_vrml::nodeDeclaration &syntax, map<string, definition> &names, vector<string> parent)
{
	if (syntax.cmd == "USE" || syntax.content.nodeType == "IndexedFaceSet" || syntax.content.nodeType == "Box" || syntax.content.nodeType == "Cone" ||
		syntax.content.nodeType == "Cylinder" || syntax.content.nodeType == "Sphere")
	{
		if (syntax.cmd == "DEF")
		{
			names[syntax.name].model = model;
			names[syntax.name].norm = norm;
			names[syntax.name].syntax = syntax.content;
			parent.push_back(syntax.name);
			return interpret_geometry(model, norm, syntax.content, names, parent);
		}
		else if (syntax.cmd == "USE")
		{
			map<string, definition>::iterator loc = names.find(syntax.name);
			parent.push_back(syntax.name);
			if (loc != names.end())
				return interpret_geometry(model*loc->second.model, norm*loc->second.norm, loc->second.syntax, names, parent);
			else
				error("", "undefined node \"" + syntax.name + "\"", __FILE__, __LINE__);
		}
		else
			return interpret_geometry(model, norm, syntax.content, names, parent);
	}
	return rigidhdl();
}

pair<string, phonghdl> interpret_appearance(const parse_vrml::node &syntax, map<string, definition> &names, vector<string> parent)
{
	pair<string, phonghdl> result("", phonghdl());
	for (int i = 0; i < (int)parent.size(); i++)
	{
		if (i != 0)
			result.first += ".";
		result.first += parent[i];
	}
	if (syntax.nodeType == "Appearance")
	{
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "material" && syntax.nodeGuts[i].value.type == "SFNode")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].node != NULL)
				{
					vector<string> temp = parent;
					temp.push_back(syntax.nodeGuts[i].first);
					result = interpret_appearance(*syntax.nodeGuts[i].value.values[0].node, names, temp);
				}
			}
			/*else if (syntax.nodeGuts[i].first == "texture" && syntax.nodeGuts[i].value.type == "SFNode")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].node != NULL)
				{
					vector<phonghdl> temp = interpret_appearance(*syntax.nodeGuts[i].value.values[0].node, names, parent + "." + syntax.nodeGuts[i].first);
					result.insert(result.end(), temp.begin(), temp.end());
				}
			}*/
		}
	}
	else if (syntax.nodeType == "Material")
	{
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "ambientIntensity" && syntax.nodeGuts[i].value.type == "SFFloat")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].values.size() > 0)
					result.second.ambient = vec3f(1.0f, 1.0f, 1.0f)*(float)atof(syntax.nodeGuts[i].value.values[0].values[0].c_str());
			}
			else if (syntax.nodeGuts[i].first == "diffuseColor" && syntax.nodeGuts[i].value.type == "SFColor")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0)
					for (int j = 0; j < (int)syntax.nodeGuts[i].value.values[0].values.size() && j < 3; j++)
						result.second.diffuse[j] = atof(syntax.nodeGuts[i].value.values[0].values[j].c_str());
			}
			else if (syntax.nodeGuts[i].first == "emissiveColor" && syntax.nodeGuts[i].value.type == "SFColor")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0)
					for (int j = 0; j < (int)syntax.nodeGuts[i].value.values[0].values.size() && j < 3; j++)
						result.second.emission[j] = atof(syntax.nodeGuts[i].value.values[0].values[j].c_str());
			}
			else if (syntax.nodeGuts[i].first == "shininess" && syntax.nodeGuts[i].value.type == "SFFloat")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].values.size() > 0)
					result.second.shininess = atof(syntax.nodeGuts[i].value.values[0].values[0].c_str());
			}
			else if (syntax.nodeGuts[i].first == "specularColor" && syntax.nodeGuts[i].value.type == "SFColor")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0)
					for (int j = 0; j < (int)syntax.nodeGuts[i].value.values[0].values.size() && j < 3; j++)
						result.second.specular[j] = atof(syntax.nodeGuts[i].value.values[0].values[j].c_str());
			}
			else if (syntax.nodeGuts[i].first == "transparency" && syntax.nodeGuts[i].value.type == "SFFloat")
			{
			}
		}
	}

	return result;
}

pair<string, phonghdl> interpret_appearance(const parse_vrml::nodeDeclaration &syntax, map<string, definition> &names, vector<string> parent)
{
	if (syntax.cmd == "USE" || syntax.content.nodeType == "Appearance" || syntax.content.nodeType == "Material")
	{
		if (syntax.cmd == "DEF")
		{
			names[syntax.name].syntax = syntax.content;
			parent.push_back(syntax.name);
			return interpret_appearance(syntax.content, names, parent);
		}
		else if (syntax.cmd == "USE")
		{
			map<string, definition>::iterator loc = names.find(syntax.name);
			parent.push_back(syntax.name);
			if (loc != names.end())
				return interpret_appearance(loc->second.syntax, names, parent);
			else
				error("", "undefined node \"" + syntax.name + "\"", __FILE__, __LINE__);
		}
		else
			return interpret_appearance(syntax.content, names, parent);
	}
	return pair<string, phonghdl>("", phonghdl());
}

objecthdl interpret_common(mat4f model, mat3f norm, const parse_vrml::node &syntax, map<string, definition> &names, vector<string> parent)
{
	objecthdl result;
	if (syntax.nodeType == "Shape")
	{
		string matname = "";
		for (int i = 0; i < (int)parent.size(); i++)
			matname += parent[i] + ".";
		matname += "material";
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			vector<string> name = parent;
			name.push_back(syntax.nodeGuts[i].first);

			if (syntax.nodeGuts[i].first == "appearance" && syntax.nodeGuts[i].value.type == "SFNode")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].node != NULL)
				{

					pair<string, phonghdl> temp = interpret_appearance(*syntax.nodeGuts[i].value.values[0].node, names, name);
					matname = temp.first;
					result.material.insert(pair<string, materialhdl*>(matname, new phonghdl(temp.second)));
				}
			}
			else if (syntax.nodeGuts[i].first == "geometry" && syntax.nodeGuts[i].value.type == "SFNode")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0 && syntax.nodeGuts[i].value.values[0].node != NULL)
				{
					result.rigid.push_back(interpret_geometry(model, norm, *syntax.nodeGuts[i].value.values[0].node, names, name));
					result.rigid.back().material = matname;
					result.rigid.back().name = parent;

					result.rigid.back().positions.resize(result.rigid.back().name.size(), map<double, vec3f>());
					result.rigid.back().orientations.resize(result.rigid.back().name.size(), map<double, vec4d>());
					result.rigid.back().center.resize(result.rigid.back().name.size(), vec3f(0.0f, 0.0f, 0.0f));
					result.rigid.back().scale.resize(result.rigid.back().name.size(), vec3f(1.0f, 1.0f, 1.0f));
					result.rigid.back().scale_orientation.resize(result.rigid.back().name.size(), vec4d(0.0f, 0.0f, 1.0f, 0.0f));
				}
			}
		}
	}

	for (int i = 0; i < result.rigid.size(); i++)
		for (int j = 0; j < result.rigid[i].geometry.size(); j++)
		{
			if (result.bound[0] > result.rigid[i].geometry[j][0])
				result.bound[0] = result.rigid[i].geometry[j][0];
			if (result.bound[1] < result.rigid[i].geometry[j][0])
				result.bound[1] = result.rigid[i].geometry[j][0];

			if (result.bound[2] > result.rigid[i].geometry[j][1])
				result.bound[2] = result.rigid[i].geometry[j][1];
			if (result.bound[3] < result.rigid[i].geometry[j][1])
				result.bound[3] = result.rigid[i].geometry[j][1];

			if (result.bound[4] > result.rigid[i].geometry[j][2])
				result.bound[4] = result.rigid[i].geometry[j][2];
			if (result.bound[5] < result.rigid[i].geometry[j][2])
				result.bound[5] = result.rigid[i].geometry[j][2];
		}

	return result;
}

objecthdl interpret_common(mat4f model, mat3f norm, const parse_vrml::nodeDeclaration &syntax, map<string, definition> &names, vector<string> parent)
{
	if (syntax.cmd == "USE" || syntax.content.nodeType == "DirectionalLight" || syntax.content.nodeType == "PointLight" || syntax.content.nodeType == "SpotLight" ||
		syntax.content.nodeType == "Shape")
	{
		if (syntax.cmd == "DEF")
		{
			names[syntax.name].model = model;
			names[syntax.name].norm = norm;
			names[syntax.name].syntax = syntax.content;
			parent.push_back(syntax.name);

			return interpret_common(model, norm, syntax.content, names, parent);
		}
		else if (syntax.cmd == "USE")
		{
			map<string, definition>::iterator loc = names.find(syntax.name);
			parent.push_back(syntax.name);

			if (loc != names.end())
				return interpret_common(model*loc->second.model, norm*loc->second.norm, loc->second.syntax, names, parent);
			else
				error("", "undefined node \"" + syntax.name + "\"", __FILE__, __LINE__);
		}
		else
			return interpret_common(model, norm, syntax.content, names, parent);
	}

	return objecthdl();
}

objecthdl interpret_interpolator(mat4f model, mat3f norm, const parse_vrml::node &syntax, map<string, definition> &names, vector<string> parent)
{
	objecthdl result;
	if (syntax.nodeType == "PositionInterpolator")
	{
		vector<double> keys;
		vector<vec3f> values;
		result.rigid.push_back(rigidhdl());
		result.rigid.back().name = parent;
		result.rigid.back().positions.push_back(map<double, vec3f>());
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "key" && syntax.nodeGuts[i].value.type == "MFFloat")
			{
				for (int j = 0; j < (int)syntax.nodeGuts[i].value.values.size(); j++)
					if ((int)syntax.nodeGuts[i].value.values[j].values.size() > 0)
						keys.push_back(atof(syntax.nodeGuts[i].value.values[j].values[0].c_str()));
			}
			else if (syntax.nodeGuts[i].first == "keyValue" && syntax.nodeGuts[i].value.type == "MFVec3f")
			{
				for (int j = 0; j < (int)syntax.nodeGuts[i].value.values.size(); j++)
				{
					values.push_back(vec3f());
					for (int k = 0; k < (int)syntax.nodeGuts[i].value.values[j].values.size() && k < 3; k++)
						values.back()[k] = atof(syntax.nodeGuts[i].value.values[j].values[k].c_str());
				}
			}
		}

		for (int i = 0; i < (int)min(keys.size(), values.size()); i++)
			result.rigid.back().positions.back().insert(pair<double, vec3f>(keys[i], values[i]));
	}
	else if (syntax.nodeType == "OrientationInterpolator")
	{
		vector<double> keys;
		vector<vec4d> values;
		result.rigid.push_back(rigidhdl());
		result.rigid.back().name = parent;
		result.rigid.back().orientations.push_back(map<double, vec4d>());
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "key" && syntax.nodeGuts[i].value.type == "MFFloat")
			{
				for (int j = 0; j < (int)syntax.nodeGuts[i].value.values.size(); j++)
					if ((int)syntax.nodeGuts[i].value.values[j].values.size() > 0)
						keys.push_back(atof(syntax.nodeGuts[i].value.values[j].values[0].c_str()));
			}
			else if (syntax.nodeGuts[i].first == "keyValue" && syntax.nodeGuts[i].value.type == "MFRotation")
			{
				for (int j = 0; j < (int)syntax.nodeGuts[i].value.values.size(); j++)
				{
					values.push_back(vec4d());
					for (int k = 0; k < (int)syntax.nodeGuts[i].value.values[j].values.size() && k < 4; k++)
						values.back()[k] = atof(syntax.nodeGuts[i].value.values[j].values[k].c_str());
					vec3f axis(values.back());
					axis = ::norm(axis);
					values.back().set(0,3,axis);
				}
			}
		}

		for (int i = 0; i < (int)min(keys.size(), values.size()); i++)
			result.rigid.back().orientations.back().insert(pair<double, vec4d>(keys[i], values[i]));
	}

	return result;
}

objecthdl interpret_interpolator(mat4f model, mat3f norm, const parse_vrml::nodeDeclaration &syntax, map<string, definition> &names, vector<string> parent)
{
	if (syntax.cmd == "USE" || syntax.content.nodeType == "PositionInterpolator" || syntax.content.nodeType == "OrientationInterpolator")
	{
		if (syntax.cmd == "DEF")
		{
			names[syntax.name].model = model;
			names[syntax.name].norm = norm;
			names[syntax.name].syntax = syntax.content;
			parent.push_back(syntax.name);

			return interpret_interpolator(model, norm, syntax.content, names, parent);
		}
		else if (syntax.cmd == "USE")
		{
			map<string, definition>::iterator loc = names.find(syntax.name);
			parent.push_back(syntax.name);

			if (loc != names.end())
				return interpret_interpolator(model*loc->second.model, norm*loc->second.norm, loc->second.syntax, names, parent);
			else
				error("", "undefined node \"" + syntax.name + "\"", __FILE__, __LINE__);
		}
		else
			return interpret_interpolator(model, norm, syntax.content, names, parent);
	}

	return objecthdl();
}

objecthdl interpret_sensors(mat4f model, mat3f norm, const parse_vrml::node &syntax, map<string, definition> &names, vector<string> parent)
{
	objecthdl result;
	if (syntax.nodeType == "TimeSensor")
	{
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "cycleInterval" && syntax.nodeGuts[i].value.type == "SFTime")
			{
				if ((int)syntax.nodeGuts[i].value.values.size() > 0 && (int)syntax.nodeGuts[i].value.values[0].values.size() > 0)
					result.animation_length = (float)atof(syntax.nodeGuts[i].value.values[0].values[0].c_str());
			}
		}
	}

	return result;
}

objecthdl interpret_sensors(mat4f model, mat3f norm, const parse_vrml::nodeDeclaration &syntax, map<string, definition> &names, vector<string> parent)
{
	if (syntax.cmd == "USE" || syntax.content.nodeType == "TimeSensor")
	{
		if (syntax.cmd == "DEF")
		{
			names[syntax.name].model = model;
			names[syntax.name].norm = norm;
			names[syntax.name].syntax = syntax.content;
			parent.push_back(syntax.name);

			return interpret_sensors(model, norm, syntax.content, names, parent);
		}
		else if (syntax.cmd == "USE")
		{
			map<string, definition>::iterator loc = names.find(syntax.name);
			parent.push_back(syntax.name);

			if (loc != names.end())
				return interpret_sensors(model*loc->second.model, norm*loc->second.norm, loc->second.syntax, names, parent);
			else
				error("", "undefined node \"" + syntax.name + "\"", __FILE__, __LINE__);
		}
		else
			return interpret_sensors(model, norm, syntax.content, names, parent);
	}

	return objecthdl();
}

objecthdl interpret_groups(mat4f model, mat3f norm, const parse_vrml::node &syntax, map<string, definition> &names, vector<string> parent)
{
	objecthdl result;
	vec3f center(0.0f, 0.0f, 0.0f);
	vec4d rotation(0.0f, 0.0f, 1.0f, 0.0f);
	vec3f scale(1.0f, 1.0f, 1.0f);
	vec4d scale_orientation(0.0f, 0.0f, 1.0f, 0.0f);
	vec3f translation(0.0f, 0.0f, 0.0f);
	if (syntax.nodeType == "Transform")
	{
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "center" && syntax.nodeGuts[i].value.type == "SFVec3f")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0)
					for (int j = 0; j < (int)syntax.nodeGuts[i].value.values[0].values.size() && j < 3; j++)
						center[j] = atof(syntax.nodeGuts[i].value.values[0].values[j].c_str());
			}
			else if (syntax.nodeGuts[i].first == "rotation" && syntax.nodeGuts[i].value.type == "SFRotation")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0)
				{
					for (int j = 0; j < (int)syntax.nodeGuts[i].value.values[0].values.size() && j < 3; j++)
						rotation[j] = atof(syntax.nodeGuts[i].value.values[0].values[j].c_str());
					if ((int)syntax.nodeGuts[i].value.values[0].values.size() >= 4)
						rotation[3] = atof(syntax.nodeGuts[i].value.values[0].values[3].c_str());

					vec3f axis = (vec3f)rotation;
					axis = ::norm(axis);
					rotation.set(0, 3, axis);
				}
			}
			else if (syntax.nodeGuts[i].first == "scale" && syntax.nodeGuts[i].value.type == "SFVec3f")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0)
					for (int j = 0; j < (int)syntax.nodeGuts[i].value.values[0].values.size() && j < 3; j++)
						scale[j] = atof(syntax.nodeGuts[i].value.values[0].values[j].c_str());
			}
			else if (syntax.nodeGuts[i].first == "scaleOrientation" && syntax.nodeGuts[i].value.type == "SFRotation")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0)
				{
					for (int j = 0; j < (int)syntax.nodeGuts[i].value.values[0].values.size() && j < 3; j++)
						scale_orientation[j] = atof(syntax.nodeGuts[i].value.values[0].values[j].c_str());
					if ((int)syntax.nodeGuts[i].value.values[0].values.size() >= 4)
						scale_orientation[3] = atof(syntax.nodeGuts[i].value.values[0].values[3].c_str());

					vec3f axis = (vec3f)scale_orientation;
					axis = ::norm(axis);
					scale_orientation.set(0, 3, axis);
				}
			}
			else if (syntax.nodeGuts[i].first == "translation" && syntax.nodeGuts[i].value.type == "SFVec3f")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0)
					for (int j = 0; j < (int)syntax.nodeGuts[i].value.values[0].values.size() && j < 3; j++)
						translation[j] = atof(syntax.nodeGuts[i].value.values[0].values[j].c_str());
			}
		}

		/*mat4f M = translate_matrix(translation) *
				  translate_matrix(center) *
				  rotate_matrix(rotation[3], (vec3f)rotation) *
				  rotate_matrix(scale_orientation[3], (vec3f)scale_orientation) *
				  scale_matrix(scale) *
				  rotate_matrix(-scale_orientation[3], (vec3f)scale_orientation) *
				  translate_matrix(-center);

		translation *= -1.0f;
		rotation[3] *= -1.0f;

		model *= M;

		norm = transpose(inverse(model));*/
	}

	if (syntax.nodeType == "Group" || syntax.nodeType == "Transform" || syntax.nodeType == "Anchor" || syntax.nodeType == "Collision")
	{
		vector<objecthdl> children;
		vec3f bboxCenter(0.0f, 0.0f, 0.0f);
		vec3f bboxSize(-1.0f, -1.0f, -1.0f);
		for (int i = 0; i < (int)syntax.nodeGuts.size(); i++)
		{
			if (syntax.nodeGuts[i].first == "children" && syntax.nodeGuts[i].value.type == "MFNode")
			{
				for (int j = 0; j < (int)syntax.nodeGuts[i].value.values.size(); j++)
					if (syntax.nodeGuts[i].value.values[j].node != NULL)
						children.push_back(interpret_groups(model, norm, *syntax.nodeGuts[i].value.values[j].node, names, parent));
			}
			else if (syntax.nodeGuts[i].first == "bboxCenter" && syntax.nodeGuts[i].value.type == "SFVec3f")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0)
					for (int j = 0; j < (int)syntax.nodeGuts[i].value.values[0].values.size() && j < 3; j++)
						bboxCenter[j] = atof(syntax.nodeGuts[i].value.values[0].values[j].c_str());
				vec4f temp = model*homogenize(bboxCenter);
				bboxCenter = temp/temp[3];
			}
			else if (syntax.nodeGuts[i].first == "bboxSize" && syntax.nodeGuts[i].value.type == "SFVec3f")
			{
				if (syntax.nodeGuts[i].value.values.size() > 0)
					for (int j = 0; j < (int)syntax.nodeGuts[i].value.values[0].values.size() && j < 3; j++)
						bboxSize[j] = atof(syntax.nodeGuts[i].value.values[0].values[j].c_str());
			}
		}

		for (int i = 0; i < children.size(); i++)
		{
			for (int j = 0; j < children[i].rigid.size(); j++)
			{
				result.rigid.push_back(children[i].rigid[j]);

				int index = (int)parent.size()-1;

				if (syntax.nodeType == "Transform" && translation != vec3f(0.0f, 0.0f, 0.0f))
					result.rigid.back().positions[index].insert(pair<double, vec3f>(0.0f, translation));
				if (syntax.nodeType == "Transform" && rotation[3] != 0.0f)
					result.rigid.back().orientations[index].insert(pair<double, vec4d>(0.0f, rotation));
				if (syntax.nodeType == "Transform")
					result.rigid.back().center[index] = center;
				if (syntax.nodeType == "Transform")
					result.rigid.back().scale[index] = scale;
				if (syntax.nodeType == "Transform")
					result.rigid.back().scale_orientation[index] = scale_orientation;
			}

			result.material.insert(children[i].material.begin(), children[i].material.end());

			children[i].material.clear();

			if (result.bound[0] > children[i].bound[0])
				result.bound[0] = children[i].bound[0];
			if (result.bound[1] < children[i].bound[1])
				result.bound[1] = children[i].bound[1];

			if (result.bound[2] > children[i].bound[2])
				result.bound[2] = children[i].bound[2];
			if (result.bound[3] < children[i].bound[3])
				result.bound[3] = children[i].bound[3];

			if (result.bound[4] > children[i].bound[4])
				result.bound[4] = children[i].bound[4];
			if (result.bound[5] < children[i].bound[5])
				result.bound[5] = children[i].bound[5];
		}

		if (bboxSize[0] > 0.0f)
		{
			result.bound[0] = bboxCenter[0] - bboxSize[0];
			result.bound[1] = bboxCenter[0] + bboxSize[0];
		}

		if (bboxSize[1] > 0.0f)
		{
			result.bound[2] = bboxCenter[1] - bboxSize[1];
			result.bound[3] = bboxCenter[1] + bboxSize[1];
		}

		if (bboxSize[2] > 0.0f)
		{
			result.bound[4] = bboxCenter[2] - bboxSize[2];
			result.bound[5] = bboxCenter[2] + bboxSize[2];
		}
	}

	return result;
}

objecthdl interpret_groups(mat4f model, mat3f norm, const parse_vrml::nodeDeclaration &syntax, map<string, definition> &names, vector<string> parent)
{
	if (syntax.content.nodeType == "Coordinate" || syntax.content.nodeType == "Normal" || syntax.content.nodeType == "TextureCoordinate")
		interpret_properties(model, norm, syntax, names, parent);
	else if (syntax.content.nodeType == "IndexedFaceSet" || syntax.content.nodeType == "Box" || syntax.content.nodeType == "Cone" ||
			syntax.content.nodeType == "Cylinder" || syntax.content.nodeType == "Sphere")
	{
		objecthdl result;
		result.rigid.push_back(interpret_geometry(model, norm, syntax, names, parent));
		return result;
	}
	else if (syntax.cmd == "USE" || syntax.content.nodeType == "Group" || syntax.content.nodeType == "Transform" || syntax.content.nodeType == "Anchor" ||
		syntax.content.nodeType == "Collision")
	{
		if (syntax.cmd == "DEF")
		{
			names[syntax.name].model = model;
			names[syntax.name].norm = norm;
			names[syntax.name].syntax = syntax.content;
			parent.push_back(syntax.name);
			return interpret_groups(model, norm, syntax.content, names, parent);
		}
		else if (syntax.cmd == "USE")
		{
			map<string, definition>::iterator loc = names.find(syntax.name);
			parent.push_back(syntax.name);
			if (loc != names.end())
				return interpret_groups(model*loc->second.model, norm*loc->second.norm, loc->second.syntax, names, parent);
			else
				error("", "undefined node \"" + syntax.name + "\"", __FILE__, __LINE__);
		}
		else
			return interpret_groups(model, norm, syntax.content, names, parent);
	}
	else if (syntax.content.nodeType == "Shape")
	{
		objecthdl result = interpret_common(model, norm, syntax, names, parent);
		return result;
	}
	else if (syntax.content.nodeType == "PositionInterpolator" || syntax.content.nodeType == "OrientationInterpolator")
	{
		objecthdl result = interpret_interpolator(model, norm, syntax, names, parent);
		return result;
	}
	else if (syntax.content.nodeType == "TimeSensor")
	{
		objecthdl result = interpret_sensors(model, norm, syntax, names, parent);
		return result;
	}

	return objecthdl();
}

void interpret_route(const parse_vrml::routeDeclaration &syntax, map<pair<string, string>, pair<string, string> > &routes, vector<string> parent)
{
	routes.insert(pair<pair<string, string>, pair<string, string> >(pair<string, string>(syntax.nodeIn, syntax.eventIn), pair<string, string>(syntax.nodeOut, syntax.eventOut)));
}

void modelhdl::load_wrl(string filename)
{
	configuration config;
	tokenizer tokens;
	parse_vrml::scene::register_syntax(tokens);
	config.load(tokens, filename, "");
	tokens.increment(true);
	tokens.expect<parse_vrml::scene>();

	if (tokens.decrement(__FILE__, __LINE__))
	{
		parse_vrml::scene syntax(tokens);

		map<string, definition> names;
		vector<objecthdl> objects;
		map<pair<string, string>, pair<string, string> > routes;
		for (int i = 0; i < (int)syntax.declarations.size(); i++)
			if (syntax.declarations[i].valid)
			{
				if (syntax.declarations[i].node.valid)
					objects.push_back(interpret_groups(identity<float, 4, 4>(), identity<float, 3, 3>(), syntax.declarations[i].node, names, vector<string>()));
				else if (syntax.declarations[i].route.valid)
					interpret_route(syntax.declarations[i].route, routes, vector<string>());
			}

		for (int i = 0; i < (int)objects.size(); i++)
		{
			for (int j = 0; j < (int)objects[i].rigid.size(); j++)
			{
				for (int k = 0; k < (objects[i].rigid[j].name.size() - objects[i].rigid[j].positions.size()); k++)
					objects[i].rigid[j].positions.insert(objects[i].rigid[j].positions.begin(), map<double, vec3f>());
				for (int k = 0; k < (objects[i].rigid[j].name.size() - objects[i].rigid[j].orientations.size()); k++)
					objects[i].rigid[j].orientations.insert(objects[i].rigid[j].orientations.begin(), map<double, vec4d>());

				if (objects[i].rigid[j].geometry.size() > 0 && objects[i].rigid[j].indices.size() > 0)
				{
					for (int k = 0; k < (int)objects[i].rigid[j].name.size(); k++)
					{
						map<pair<string, string>, pair<string, string> >::iterator tloc = routes.find(pair<string, string>(objects[i].rigid[j].name[k], "translation"));
						if (tloc == routes.end())
							tloc = routes.find(pair<string, string>(objects[i].rigid[j].name[k], "set_translation"));
						map<pair<string, string>, pair<string, string> >::iterator rloc = routes.find(pair<string, string>(objects[i].rigid[j].name[k], "rotation"));
						if (rloc == routes.end())
							rloc = routes.find(pair<string, string>(objects[i].rigid[j].name[k], "set_rotation"));

						if (tloc != routes.end())
						{
							bool found = false;
							for (int i0 = 0; i0 < (int)objects.size() && !found; i0++)
								for (int j0 = 0; j0 < (int)objects[i0].rigid.size() && !found; j0++)
								{
									vector<string>::iterator loc = find(objects[i0].rigid[j0].name.begin(), objects[i0].rigid[j0].name.end(), tloc->second.first);
									if (loc != objects[i0].rigid[j0].name.end() && objects[i0].rigid[j0].positions.size() > 0)
									{
										objects[i].rigid[j].positions[k] = objects[i0].rigid[j0].positions[0];
										found = true;
									}
								}
						}
						if (rloc != routes.end())
						{
							bool found = false;
							for (int i0 = 0; i0 < (int)objects.size() && !found; i0++)
								for (int j0 = 0; j0 < (int)objects[i0].rigid.size() && !found; j0++)
								{
									vector<string>::iterator loc = find(objects[i0].rigid[j0].name.begin(), objects[i0].rigid[j0].name.end(), rloc->second.first);
									if (loc != objects[i0].rigid[j0].name.end() && objects[i0].rigid[j0].orientations.size() > 0)
									{
										objects[i].rigid[j].orientations[k] = objects[i0].rigid[j0].orientations[0];
										found = true;
									}
								}
						}
					}
				}
			}
		}

		for (int i = 0; i < (int)objects.size(); i++)
		{
			bool valid = false;
			for (int j = 0; j < (int)objects[i].rigid.size() && !valid; j++)
				valid = (objects[i].rigid[j].geometry.size() > 0 && objects[i].rigid[j].indices.size() > 0);

			animation_length = max(objects[i].animation_length, animation_length);

			if (valid)
			{
				for (int j = 0; j < (int)objects[i].rigid.size(); j++)
					rigid.push_back(objects[i].rigid[j]);

				material.insert(objects[i].material.begin(), objects[i].material.end());
				objects[i].material.clear();

				if (bound[0] > objects[i].bound[0])
					bound[0] = objects[i].bound[0];
				if (bound[1] < objects[i].bound[1])
					bound[1] = objects[i].bound[1];

				if (bound[2] > objects[i].bound[2])
					bound[2] = objects[i].bound[2];
				if (bound[3] < objects[i].bound[3])
					bound[3] = objects[i].bound[3];

				if (bound[4] > objects[i].bound[4])
					bound[4] = objects[i].bound[4];
				if (bound[5] < objects[i].bound[5])
					bound[5] = objects[i].bound[5];
			}
		}
	}
}
