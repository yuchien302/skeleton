/*
 * routeDeclaration.cpp
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "routeDeclaration.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>

namespace parse_vrml
{
routeDeclaration::routeDeclaration()
{
	debug_name = "routeDeclaration";
}

routeDeclaration::routeDeclaration(tokenizer &tokens, void *data)
{
	debug_name = "routeDeclaration";
	parse(tokens, data);
}

routeDeclaration::~routeDeclaration()
{

}

void routeDeclaration::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("ROUTE");

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	tokens.increment(true);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	tokens.expect(".");

	tokens.increment(true);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	tokens.expect("TO");

	tokens.increment(true);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	tokens.expect(".");

	tokens.increment(true);
	tokens.expect<parse::instance>();

	if (tokens.decrement(__FILE__, __LINE__, data))
		nodeOut = tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		eventOut = tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		nodeIn = tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		eventIn = tokens.next();

	tokens.syntax_end(this);
}

bool routeDeclaration::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next("ROUTE", i);
}

void routeDeclaration::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<routeDeclaration>())
	{
		tokens.register_syntax<routeDeclaration>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::symbol>();
	}
}

string routeDeclaration::to_string(string tab) const
{
	return "ROUTE " + nodeOut + "." + eventOut + " TO " + nodeIn + "." + eventIn;
}

parse::syntax *routeDeclaration::clone() const
{
	return new routeDeclaration(*this);
}

}
