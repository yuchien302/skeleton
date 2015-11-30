/*
 * syntax.h
 *
 *  Created on: Oct 28, 2014
 *      Author: nbingham
 */

#include <common/standard.h>

#ifndef parse_syntax_h
#define parse_syntax_h

namespace parse
{
struct syntax
{
	syntax()
	{
		debug_name = "syntax";
		valid = false;
		segment_name = "";
		start = -1;
		end = -1;
	}
	virtual ~syntax() {}

	string segment_name;
	int start, end;
	string debug_name;
	bool valid;

	virtual string to_string(string tab = "") const = 0;
	virtual syntax *clone() const = 0;

	template <class type>
	bool is_a()
	{
		return debug_name == type().debug_name;
	}
};
}

#endif
