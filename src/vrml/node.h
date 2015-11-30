/*
 * node.h
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "nodeGut.h"
#include "scriptGut.h"

#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#include <parse/parse.h>
#include <parse/syntax.h>

#ifndef parse_vrml_node_h
#define parse_vrml_node_h

namespace parse_vrml
{

struct node : parse::syntax
{
	node();
	node(tokenizer &tokens, void *data = NULL);
	~node();

	string nodeType;
	vector<nodeGut> nodeGuts;
	vector<scriptGut> scriptGuts;

	void parse(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
	static void register_syntax(tokenizer &tokens);

	string to_string(string tab = "") const;
	parse::syntax *clone() const;
};

}

#endif
