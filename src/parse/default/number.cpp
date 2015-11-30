/*
 * number.cpp
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "number.h"

namespace parse
{
number::number()
{
	debug_name = "number";
}

number::~number()
{

}

token number::consume(tokenizer &tokens, void *data)
{
	token result;
	result.type = tokens.token_type<number>();
	result.start = tokens.offset+1;

	char character = tokens.peek_char(1);

	while (character == '-' || character == '+')
	{
		tokens.next_char();
		character = tokens.peek_char(1);
	}

	enum
	{
		hexidecimal = 3,
		decimal = 2,
		octal = 1,
		binary = 0,
	} type = decimal;

	if (tokens.peek_char(1) == '0' && tokens.peek_char(2) == 'x')
	{
		type = hexidecimal;
		tokens.next_char();
		tokens.next_char();
	}
	else if (tokens.peek_char(1) == '0' && tokens.peek_char(2) == 'o')
	{
		type = octal;
		tokens.next_char();
		tokens.next_char();
	}
	else if (tokens.peek_char(1) == '0' && tokens.peek_char(2) == 'b')
	{
		type = binary;
		tokens.next_char();
		tokens.next_char();
	}

	while ((type == hexidecimal && ((character >= '0' && character <= '9') || (character >= 'a' && character <= 'f') || (character >= 'A' && character <= 'F'))) ||
		   (type == decimal && (character >= '0' && character <= '9')) ||
		   (type == octal && (character >= '0' && character <= '7')) ||
		   (type == binary && (character >= '0' && character <= '1')))
	{
		tokens.next_char();
		character = tokens.peek_char(1);
	}

	if (type == decimal && character == '.' && tokens.peek_char(2) >= '0' && tokens.peek_char(2) <= '9')
	{
		tokens.next_char();
		character = tokens.peek_char(1);
		while (character >= '0' && character <= '9')
		{
			tokens.next_char();
			character = tokens.peek_char(1);
		}
	}

	if (type == decimal && (character == 'e' || character == 'E') && (tokens.peek_char(2) == '-' || (tokens.peek_char(2) >= '0' && tokens.peek_char(2) <= '9')))
	{
		tokens.next_char();
		character = tokens.peek_char(1);
		if (character == '-')
		{
			tokens.next_char();
			character = tokens.peek_char(1);
		}

		if (character < '0' || character > '9')
			tokens.token_error((string)"exponent has no digits", __FILE__, __LINE__);

		while (character >= '0' && character <= '9')
		{
			tokens.next_char();
			character = tokens.peek_char(1);
		}
	}

	result.end = tokens.offset+1;
	return result;
}

bool number::is_next(tokenizer &tokens, int i, void *data)
{
	while (tokens.peek_char(i) == '-' || tokens.peek_char(i) == '+' || tokens.peek_char(i) == '.')
		i++;

	return (tokens.peek_char(i) >= '0' && tokens.peek_char(i) <= '9');
}

}
