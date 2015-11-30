/*
 * fieldValue.cpp
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "fieldValue.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>

namespace parse_vrml
{
fieldValue::fieldValue()
{
	debug_name = "fieldValue";
}

fieldValue::fieldValue(tokenizer &tokens, string type, void *data)
{
	this->type = type;
	debug_name = "fieldValue";
	parse(tokens, data);
}

fieldValue::~fieldValue()
{

}

void fieldValue::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	if (type.substr(0,2) == "MF")
	{
		tokens.increment(false);
		tokens.expect("[");

		if (tokens.decrement(__FILE__, __LINE__, data))
		{
			tokens.next();

			tokens.increment(true);
			tokens.expect("]");

			tokens.increment(false);
			tokens.expect<sfValue>();

			while (tokens.decrement(__FILE__, __LINE__, data))
			{
				values.push_back(sfValue(tokens, "SF" + type.substr(2), data));

				tokens.increment(false);
				tokens.expect(",");

				if (tokens.decrement(__FILE__, __LINE__, data))
					tokens.next();

				tokens.increment(false);
				tokens.expect<sfValue>();
			}

			if (tokens.decrement(__FILE__, __LINE__, data))
				tokens.next();
		}
		else
		{
			tokens.increment(true);
			tokens.expect<sfValue>();

			if (tokens.decrement(__FILE__, __LINE__, data))
				values.push_back(sfValue(tokens, "SF" + type.substr(2), data));
		}
	}
	else
	{
		tokens.increment(true);
		tokens.expect<sfValue>();

		if (tokens.decrement(__FILE__, __LINE__, data))
			values.push_back(sfValue(tokens, type, data));
	}

	tokens.syntax_end(this);
}

bool fieldValue::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next("[", i) || sfValue::is_next(tokens, i, data);
}

void fieldValue::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<fieldValue>())
	{
		tokens.register_syntax<fieldValue>();
		sfValue::register_syntax(tokens);
		tokens.register_token<parse::symbol>();
	}
}

string fieldValue::to_string(string tab) const
{
	string result;
	string input_tab = tab;
	if (values.size() != 1)
	{
		result += "[";
		input_tab += "\t";
	}

	for (int i = 0; i < (int)values.size(); i++)
	{
		if (i != 0)
			result += "\n" + input_tab;

		result += values[i].to_string(input_tab);
	}

	if (values.size() != 1)
		result += "]";

	return result;
}

parse::syntax *fieldValue::clone() const
{
	return new fieldValue(*this);
}

}
