/*
 * nodeDeclaration.h
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "node.h"
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef parse_vrml_nodeDeclaration_h
#define parse_vrml_nodeDeclaration_h

namespace parse_vrml
{

struct nodeDeclaration : parse::syntax
{
	nodeDeclaration();
	nodeDeclaration(tokenizer &tokens, void *data = NULL);
	~nodeDeclaration();

	string cmd;
	string name;
	node content;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#endif
