/*
 * declaration.cpp
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "declaration.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>

namespace parse_vrml
{
declaration::declaration()
{
	debug_name = "declaration";
}

declaration::declaration(tokenizer &tokens, void *data)
{
	debug_name = "declaration";
	parse(tokens, data);
}

declaration::~declaration()
{

}

void declaration::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect<routeDeclaration>();
	tokens.expect<protoDeclaration>();
	tokens.expect<nodeDeclaration>();
	tokens.expect("NULL");

	if (tokens.decrement(__FILE__, __LINE__, data))
	{
		if (tokens.found<routeDeclaration>())
			route = routeDeclaration(tokens, data);
		else if (tokens.found<protoDeclaration>())
			proto = protoDeclaration(tokens, data);
		else if (tokens.found<nodeDeclaration>())
			node = nodeDeclaration(tokens, data);
		else if (tokens.found("NULL"))
			tokens.next();
	}

	tokens.syntax_end(this);
}

bool declaration::is_next(tokenizer &tokens, int i, void *data)
{
	return routeDeclaration::is_next(tokens, i, data) ||
		   protoDeclaration::is_next(tokens, i, data) ||
		   nodeDeclaration::is_next(tokens, i, data) ||
		   tokens.is_next("NULL", i);
}

void declaration::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<declaration>())
	{
		tokens.register_syntax<declaration>();
		tokens.register_token<parse::instance>();
		routeDeclaration::register_syntax(tokens);
		protoDeclaration::register_syntax(tokens);
		nodeDeclaration::register_syntax(tokens);
	}
}

string declaration::to_string(string tab) const
{
	if (route.valid)
		return route.to_string(tab);
	else if (proto.valid)
		return proto.to_string(tab);
	else if (node.valid)
		return node.to_string(tab);
	else
		return "NULL";
}

parse::syntax *declaration::clone() const
{
	return new declaration(*this);
}

}
