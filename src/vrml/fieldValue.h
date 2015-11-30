/*
 * fieldValue.h
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "sfValue.h"

#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef parse_vrml_fieldValue_h
#define parse_vrml_fieldValue_h

namespace parse_vrml
{

struct fieldValue : parse::syntax
{
	fieldValue();
	fieldValue(tokenizer &tokens, string type, void *data = NULL);
	~fieldValue();

	string type;
	vector<sfValue> values;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#endif
