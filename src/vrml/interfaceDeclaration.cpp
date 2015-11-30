/*
 * interfaceDeclaration.cpp
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "interfaceDeclaration.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>

namespace parse_vrml
{
interfaceDeclaration::interfaceDeclaration()
{
	debug_name = "interfaceDeclaration";
	external = false;
}

interfaceDeclaration::interfaceDeclaration(tokenizer &tokens, bool external, void *data)
{
	this->external = external;
	debug_name = "interfaceDeclaration";
	parse(tokens, data);
}

interfaceDeclaration::~interfaceDeclaration()
{

}

void interfaceDeclaration::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("eventIn");
	tokens.expect("eventOut");
	tokens.expect("field");
	tokens.expect("exposedField");

	if (tokens.decrement(__FILE__, __LINE__, data))
	{
		kind = tokens.next();
		tokens.increment(true);
		tokens.expect<parse::instance>();

		tokens.increment(true);
		tokens.expect<parse::instance>();

		if (tokens.decrement(__FILE__, __LINE__, data))
			type = tokens.next();

		if (tokens.decrement(__FILE__, __LINE__, data))
			name = tokens.next();

		if ((kind == "field" || kind == "exposedField") && !external)
		{
			tokens.increment(true);
			tokens.expect<fieldValue>();

			if (tokens.decrement(__FILE__, __LINE__, data))
				value = fieldValue(tokens, type, data);
		}
	}

	tokens.syntax_end(this);
}

bool interfaceDeclaration::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next("eventIn", i) ||
		   tokens.is_next("eventOut", i) ||
		   tokens.is_next("field", i) ||
		   tokens.is_next("exposedField", i);
}

void interfaceDeclaration::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<interfaceDeclaration>())
	{
		tokens.register_syntax<interfaceDeclaration>();
		tokens.register_token<parse::instance>();
		fieldValue::register_syntax(tokens);
	}
}

string interfaceDeclaration::to_string(string tab) const
{
	string result = kind + " " + type + " " + name;

	if (value.valid)
		result += " " + value.to_string();

	return result;
}

parse::syntax *interfaceDeclaration::clone() const
{
	return new interfaceDeclaration(*this);
}

}
