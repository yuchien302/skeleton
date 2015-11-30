/*
 * scriptGut.cpp
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "scriptGut.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>

namespace parse_vrml
{
scriptGut::scriptGut()
{
	debug_name = "scriptGut";
}

scriptGut::scriptGut(tokenizer &tokens, void *data)
{
	debug_name = "scriptGut";
	parse(tokens, data);
}

scriptGut::~scriptGut()
{

}

void scriptGut::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect<interfaceDeclaration>();
	tokens.expect("eventIn");
	tokens.expect("eventOut");
	tokens.expect("field");

	if (tokens.decrement(__FILE__, __LINE__, data))
	{
		if (tokens.found<interfaceDeclaration>())
			interface = interfaceDeclaration(tokens, data);
		else
		{
			stmtType = tokens.next();

			tokens.increment(true);
			tokens.expect<parse::instance>();

			tokens.increment(true);
			tokens.expect("IS");

			tokens.increment(true);
			tokens.expect<parse::instance>();

			tokens.increment(true);
			tokens.expect<parse::instance>();

			if (tokens.decrement(__FILE__, __LINE__, data))
				fieldType = tokens.next();

			if (tokens.decrement(__FILE__, __LINE__, data))
				first = tokens.next();

			if (tokens.decrement(__FILE__, __LINE__, data))
				tokens.next();

			if (tokens.decrement(__FILE__, __LINE__, data))
				second = tokens.next();
		}
	}

	tokens.syntax_end(this);
}

bool scriptGut::is_next(tokenizer &tokens, int i, void *data)
{
	return interfaceDeclaration::is_next(tokens, i, data) ||
		   tokens.is_next("eventIn") ||
		   tokens.is_next("eventOut") ||
		   tokens.is_next("field");
}

void scriptGut::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<scriptGut>())
	{
		tokens.register_syntax<scriptGut>();
		tokens.register_token<parse::instance>();
		interfaceDeclaration::register_syntax(tokens);
	}
}

string scriptGut::to_string(string tab) const
{
	if (interface.valid)
		return interface.to_string(tab);
	else
		return stmtType + " " + fieldType + " " + first + " IS " + second;
}

parse::syntax *scriptGut::clone() const
{
	return new scriptGut(*this);
}

}
