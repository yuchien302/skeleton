/*
 * nodeGut.h
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "routeDeclaration.h"
#include "protoDeclaration.h"
#include "fieldValue.h"

#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef parse_vrml_nodeGut_h
#define parse_vrml_nodeGut_h

namespace parse_vrml
{

struct nodeGut : parse::syntax
{
	nodeGut();
	nodeGut(tokenizer &tokens, string nodeType, void *data = NULL);
	~nodeGut();

	string nodeType;
	string first;
	string second;
	fieldValue value;
	routeDeclaration route;
	protoDeclaration proto;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#endif
