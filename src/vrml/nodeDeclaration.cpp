/*
 * nodeDeclaration.cpp
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "nodeDeclaration.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>

namespace parse_vrml
{
nodeDeclaration::nodeDeclaration()
{
	debug_name = "nodeDeclaration";
}

nodeDeclaration::nodeDeclaration(tokenizer &tokens, void *data)
{
	debug_name = "nodeDeclaration";
	parse(tokens, data);
}

nodeDeclaration::~nodeDeclaration()
{

}

void nodeDeclaration::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("DEF");
	tokens.expect("USE");
	tokens.expect<node>();

	if (tokens.decrement(__FILE__, __LINE__, data))
	{
		if (tokens.found("DEF"))
		{
			cmd = tokens.next();

			tokens.increment(true);
			tokens.expect<node>();

			tokens.increment(true);
			tokens.expect<parse::instance>();

			if (tokens.decrement(__FILE__, __LINE__, data))
				name = tokens.next();

			if (tokens.decrement(__FILE__, __LINE__, data))
				content = node(tokens, data);
		}
		else if (tokens.found("USE"))
		{
			cmd = tokens.next();

			tokens.increment(true);
			tokens.expect<parse::instance>();

			if (tokens.decrement(__FILE__, __LINE__, data))
				name = tokens.next();
		}
		else if (tokens.found<node>())
			content = node(tokens, data);
	}

	tokens.syntax_end(this);
}

bool nodeDeclaration::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next("USE", i) || tokens.is_next("DEF", i) || node::is_next(tokens, i, data);
}

void nodeDeclaration::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<nodeDeclaration>())
	{
		tokens.register_syntax<nodeDeclaration>();
		tokens.register_token<parse::instance>();
		node::register_syntax(tokens);
	}
}

string nodeDeclaration::to_string(string tab) const
{
	if (cmd == "USE")
		return cmd + " " + name;
	else if (cmd == "DEF")
		return cmd + " " + name + " " + content.to_string(tab);
	else
		return content.to_string(tab);
}

parse::syntax *nodeDeclaration::clone() const
{
	return new nodeDeclaration(*this);
}

}
