/*
 * node.cpp
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "node.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>

namespace parse_vrml
{
node::node()
{
	debug_name = "node";
}

node::node(tokenizer &tokens, void *data)
{
	debug_name = "node";

	parse(tokens, data);
}

node::~node()
{

}

void node::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("}");

	tokens.increment(true);
	tokens.expect("{");

	tokens.increment(true);
	tokens.expect<parse::instance>();

	if (tokens.decrement(__FILE__, __LINE__, data))
		nodeType = tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	tokens.increment(false);
	tokens.expect<nodeGut>();
	if (nodeType == "Script")
		tokens.expect<scriptGut>();

	while (tokens.decrement(__FILE__, __LINE__, data))
	{
		if (tokens.found<scriptGut>())
			scriptGuts.push_back(scriptGut(tokens, data));
		else if (tokens.found<nodeGut>())
			nodeGuts.push_back(nodeGut(tokens, nodeType, data));

		tokens.increment(false);
		tokens.expect<nodeGut>();
		if (nodeType == "Script")
			tokens.expect<scriptGut>();
	}

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	tokens.syntax_end(this);
}

bool node::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next<parse::instance>(i) && tokens.is_next("{", i+1);
}

void node::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<node>())
	{
		tokens.register_syntax<node>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::symbol>();
		scriptGut::register_syntax(tokens);
		nodeGut::register_syntax(tokens);
	}
}

string node::to_string(string tab) const
{
	string result = nodeType + "\n" + tab + "{\n";

	for (int i = 0; i < (int)nodeGuts.size(); i++)
		result += tab + "\t" + nodeGuts[i].to_string(tab + "\t") + "\n";

	for (int i = 0; i < (int)scriptGuts.size(); i++)
		result += tab + "\t" + scriptGuts[i].to_string(tab + "\t") + "\n";

	result += tab + "}";
	return result;
}

parse::syntax *node::clone() const
{
	return new node(*this);
}

}
