/*
 * configuration.h
 *
 *  Created on: Oct 22, 2014
 *      Author: nbingham
 */

#include <common/standard.h>
#include "segment.h"

#ifndef parse_configuration_h
#define parse_configuration_h

struct tokenizer;

struct configuration
{
	configuration();
	~configuration();

	vector<string> import_directories;
	vector<string> input_files;
	string working_directory;

	void load(tokenizer &tokens, string filename, string base = "");
	void add_import_directory(string directory);
	void set_working_directory(string filename);
};

#endif
