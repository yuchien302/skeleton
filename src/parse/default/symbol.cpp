/*
 * symbol.cpp
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "symbol.h"

namespace parse
{
symbol::symbol()
{
	debug_name = "symbol";
}

symbol::~symbol()
{

}

token symbol::consume(tokenizer &tokens, void *data)
{
	token result;
	result.type = tokens.token_type<symbol>();
	result.start = tokens.offset+1;

	string one(1, tokens.next_char());

	result.end = tokens.offset+1;
	return result;
}

bool symbol::is_next(tokenizer &tokens, int i, void *data)
{
	char character = tokens.peek_char(i);
	char character2 = tokens.peek_char(i+1);

	return ( character == '(' || character == ')' || character == '=' || character == '[' || character == '{' ||
			 character == ']' || character == '}' || character == ';' || character == ':' || character == ',' || (character == '.' && (character2 < '0' || character2 > '9')));
}

}
