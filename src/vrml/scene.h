/*
 * scene.h
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "declaration.h"
#include "globals.h"

#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef parse_vrml_scene_h
#define parse_vrml_scene_h

namespace parse_vrml
{

struct scene : parse::syntax
{
	scene();
	scene(const scene &cpy);
	scene(tokenizer &tokens, void *data = NULL);
	~scene();

	vector<declaration> declarations;
	globals *variables;
	bool owner;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#endif
