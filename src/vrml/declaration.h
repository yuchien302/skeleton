/*
 * declaration.h
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "nodeDeclaration.h"
#include "protoDeclaration.h"
#include "routeDeclaration.h"

#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef parse_vrml_declaration_h
#define parse_vrml_declaration_h

namespace parse_vrml
{

struct declaration : parse::syntax
{
	declaration();
	declaration(tokenizer &tokens, void *data = NULL);
	~declaration();

	nodeDeclaration node;
	protoDeclaration proto;
	routeDeclaration route;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#endif
