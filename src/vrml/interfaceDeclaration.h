/*
 * interfaceDeclaration.h
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "fieldValue.h"

#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef parse_vrml_interfaceDeclaration_h
#define parse_vrml_interfaceDeclaration_h

namespace parse_vrml
{

struct interfaceDeclaration : parse::syntax
{
	interfaceDeclaration();
	interfaceDeclaration(tokenizer &tokens, bool external, void *data = NULL);
	~interfaceDeclaration();

	bool external;
	string kind;
	string type;
	string name;
	fieldValue value;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#endif
