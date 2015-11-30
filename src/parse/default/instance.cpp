/*
 * instance.cpp
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "instance.h"

namespace parse
{
instance::instance()
{
	debug_name = "instance";
}

instance::~instance()
{
}

token instance::consume(tokenizer &tokens, void *data)
{
	token result;
	result.type = tokens.token_type<instance>();
	result.start = tokens.offset+1;
	char character;
	do
	{
		tokens.next_char();
		character = tokens.peek_char(1);
	} while ((character >= 'A' && character <= 'Z') || character == '_' || (character >= 'a' && character <= 'z') || (character >= '0' && character <= '9') || character == '%');
	result.end = tokens.offset+1;
	return result;
}

bool instance::is_next(tokenizer &tokens, int i, void *data)
{
	char character = tokens.peek_char(i);

	return ((character >= 'A' && character <= 'Z') || character == '_' || (character >= 'a' && character <= 'z') || character == '%');
}

}
