/*
 * line_comment.cpp
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "line_comment.h"

namespace parse
{
line_comment::line_comment()
{
}

line_comment::~line_comment()
{

}

token line_comment::consume(tokenizer &tokens, void *data)
{
	token result;
	result.type = tokens.comment_type<line_comment>();
	result.start = tokens.offset+1;

	tokens.next_char();

	char character;

	do
	{
		character = tokens.next_char();
	} while (character != '\0' && character != '\n');

	result.end = tokens.offset;
	return result;
}

bool line_comment::is_next(tokenizer &tokens, int i, void *data)
{
	return (tokens.peek_char(i) == '#');
}
}
