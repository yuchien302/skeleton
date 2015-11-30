/*
 * configuration.cpp
 *
 *  Created on: Oct 26, 2014
 *      Author: nbingham
 */

#include "configuration.h"
#include "tokenizer.h"

configuration::configuration()
{
}

configuration::~configuration()
{

}

void configuration::load(tokenizer &tokens, string filename, string base)
{
	if (base.find_last_of("\\/") != string::npos)
		filename = base.substr(0, base.find_last_of("\\/")+1) + filename;

	if (tokens.segment_loading(filename))
		tokens.error("cycle found in include graph", __FILE__, __LINE__);
	else if (!tokens.segment_loaded(filename))
	{
		ifstream fin;
		fin.open(filename.c_str(), ios::binary | ios::in);
		for (int i = 0; !fin.is_open() && i < (int)import_directories.size(); i++)
			fin.open((import_directories[i] + filename).c_str(), ios::binary | ios::in);

		if (!fin.is_open())
			tokens.error("file not found '" + filename + "'", __FILE__, __LINE__);
		else
		{
			fin.seekg(0, ios::end);
			int size = (int)fin.tellg();
			string buffer(size, ' ');
			fin.seekg(0, ios::beg);
			fin.read(&buffer[0], size);
			fin.clear();
			tokens.insert(filename, buffer, this);
		}
	}
}

void configuration::add_import_directory(string filename)
{
	if (filename.size() > 0 && filename[filename.size()-1] != '/' && filename[filename.size()-1] != '\\')
		filename.push_back('/');

	if ((filename.size() < 3 || filename.substr(1, 2) != ":\\") && (filename.size() < 1 || filename[0] != '/'))
		filename = working_directory + filename;

	import_directories.push_back(filename);
}

void configuration::set_working_directory(string filename)
{
	working_directory = filename.substr(0, filename.find_last_of("/\\")+1);
}
