/*
 * routeDeclaration.h
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef parse_vrml_routeDeclaration_h
#define parse_vrml_routeDeclaration_h

namespace parse_vrml
{

struct routeDeclaration : parse::syntax
{
	routeDeclaration();
	routeDeclaration(tokenizer &tokens, void *data = NULL);
	~routeDeclaration();

	string nodeOut;
	string eventOut;

	string nodeIn;
	string eventIn;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#endif
