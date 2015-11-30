/*
 * scriptGut.h
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "interfaceDeclaration.h"
#include "nodeGut.h"

#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef parse_vrml_scriptGut_h
#define parse_vrml_scriptGut_h

namespace parse_vrml
{

struct scriptGut : parse::syntax
{
	scriptGut();
	scriptGut(tokenizer &tokens, void *data = NULL);
	~scriptGut();

	interfaceDeclaration interface;
	string stmtType;
	string fieldType;
	string first;
	string second;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#endif
