/*
 * protoDeclaration.h
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "interfaceDeclaration.h"
#include "fieldValue.h"

#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef parse_vrml_protoDeclaration_h
#define parse_vrml_protoDeclaration_h

namespace parse_vrml
{

struct scene;

struct protoDeclaration : parse::syntax
{
	protoDeclaration();
	protoDeclaration(const protoDeclaration &cpy);
	protoDeclaration(tokenizer &tokens, void *data = NULL);
	~protoDeclaration();

	bool external;
	string nodeType;
	vector<interfaceDeclaration> interfaces;
	fieldValue value;
	scene *sub;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#endif
