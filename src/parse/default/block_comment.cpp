/*
 * block_comment.cpp
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "block_comment.h"

namespace parse
{
block_comment::block_comment()
{
}

block_comment::~block_comment()
{

}

token block_comment::consume(tokenizer &tokens, void *data)
{
	token result;
	result.type = tokens.comment_type<block_comment>();
	result.start = tokens.offset+1;

	tokens.next_char();
	tokens.next_char();

	bool star = false;
	bool done = false;
	char character;

	while (!done)
	{
		character = tokens.next_char();

		if (character == '\0')
		{
			tokens.token_error("expected '*/'", __FILE__, __LINE__);
			result.end = tokens.offset;
			return result;
		}
		else if (character == '*')
			star = true;
		else if (star && character == '/')
			done = true;
		else
			star = false;
	}

	result.end = tokens.offset;
	return result;
}

bool block_comment::is_next(tokenizer &tokens, int i, void *data)
{
	return (tokens.peek_char(i) == '/' && tokens.peek_char(i+1) == '*');
}
}
