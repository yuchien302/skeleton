/*
 * opengl.cpp
 *
 *  Created on: Jan 5, 2015
 *      Author: nbingham
 */

#include "opengl.h"

// trim from start
string ltrim(string s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
        return s;
}

// trim from end
string rtrim(string s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

string trim(string s)
{
        return ltrim(rtrim(s));
}

string get_source(string filename, vector<string> loaded = vector<string>())
{
	loaded.push_back(filename);
	ifstream fin(filename.c_str());
	if (!fin.is_open())
	{
		cerr << "Error: Could not find file: " << filename << endl;
		return "";
	}
	int slash = string(filename).find_last_of("/\\");
	string path = "";
	if (slash != -1)
		path = filename.substr(0, slash) + "/";

	string source = "";
	string line;
	string command;
	while (getline(fin, line))
	{
		istringstream iss(line);

		if (iss >> command && command == "#include")
		{
			filename = trim(line.substr(8));
			if (filename.size() > 2 && filename[0] == '\"' && filename[filename.size()-1] == '\"')
				filename = filename.substr(1, filename.size()-2);

			filename = path + filename;
			if (find(loaded.begin(), loaded.end(), filename) == loaded.end())
				source += get_source(filename, loaded);
		}
		else
			source += line + "\n";
	}
	fin.close();

	return source;
}

GLuint load_shader_source(string source, GLuint type)
{
	GLuint handle = 0;
	const char* data = source.c_str();
	GLint length = source.length();

	if (length == 0)
		return 0;

	handle = glCreateShader(type);
	glShaderSource(handle, 1, &data, &length);
	glCompileShader(handle);

	char temp[1024];
	length = 1023;
	glGetShaderInfoLog(handle, 1023, &length, temp);
	if (length > 0)
		cout << temp;

	return handle;
}

GLuint load_shader_file(string filename, GLuint type)
{
	return load_shader_source(get_source(filename), type);
}
