/*
 * text.h
 *
 *  Created on: Oct 16, 2014
 *      Author: nbingham
 */

#include "../parse.h"

#ifndef parse_default_text_h
#define parse_default_text_h

namespace parse
{
struct text
{
	text();
	~text();

	string debug_name;

	static token consume(tokenizer &tokens, void *data = NULL);
	static bool is_next(tokenizer &tokens, int i = 1, void *data = NULL);
};
}

#endif
