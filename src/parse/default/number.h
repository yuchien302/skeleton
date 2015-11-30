/*
 * number.h
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "../parse.h"

#ifndef parse_default_number_h
#define parse_default_number_h

namespace parse
{
struct number
{
	number();
	~number();

	string debug_name;

	static token consume(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
};
}

#endif
