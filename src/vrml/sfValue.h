/*
 * sfValue.h
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

#ifndef parse_vrml_sfValue_h
#define parse_vrml_sfValue_h

namespace parse_vrml
{

struct nodeDeclaration;

struct sfValue : parse::syntax
{
	sfValue();
	sfValue(const sfValue &cpy);
	sfValue(tokenizer &tokens, string type, void *data = NULL);
	~sfValue();

	string type;
	vector<string> values;
	nodeDeclaration *node;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;

	sfValue &operator=(const sfValue &value);
};

}

#endif
