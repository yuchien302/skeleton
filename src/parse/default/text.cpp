/*
 * text.cpp
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "text.h"

namespace parse
{
text::text()
{
	debug_name = "text";
}

text::~text()
{

}

token text::consume(tokenizer &tokens, void *data)
{
	token result;
	result.type = tokens.token_type<text>();
	result.start = tokens.offset+1;

	char base = tokens.next_char();

	bool escaped = false;
	bool done = false;
	char character;

	while (!done)
	{
		character = tokens.next_char();

		if (escaped)
			escaped = false;
		else if (character == '\\')
			escaped = true;
		else if (character == base)
			done = true;
		else if (character == '\0')
		{
			tokens.token_error((string)"expected \'" + base + "\'", __FILE__, __LINE__);
			result.end = tokens.offset+1;
			return result;
		}
	}

	result.end = tokens.offset+1;
	return result;
}

bool text::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.peek_char(i) == '\"';
}

}
