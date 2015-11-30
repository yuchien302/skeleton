/*
 * sfValue.cpp
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "sfValue.h"
#include "nodeDeclaration.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/number.h>
#include <parse/default/text.h>


namespace parse_vrml
{
sfValue::sfValue()
{
	debug_name = "sfValue";
	node = NULL;
}

sfValue::sfValue(const sfValue &cpy) : parse::syntax(cpy)
{
	type = cpy.type;
	values = cpy.values;
	if (cpy.node != NULL)
		node = (parse_vrml::nodeDeclaration*)cpy.node->clone();
	else
		node = NULL;
}

sfValue::sfValue(tokenizer &tokens, string type, void *data)
{
	this->type = type;
	node = NULL;
	debug_name = "sfValue";
	parse(tokens, data);
}

sfValue::~sfValue()
{
	if (node != NULL)
		delete node;
	node = NULL;
}

void sfValue::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	if (type == "SFBool")
	{
		tokens.increment(true);
		tokens.expect("TRUE");
		tokens.expect("FALSE");

		if (tokens.decrement(__FILE__,  __LINE__, data))
			values.push_back(tokens.next());
	}
	else if (type == "SFVec2f")
	{
		for (int i = 0; i < 2; i++)
		{
			tokens.increment(true);
			tokens.expect<parse::number>();
		}

		for (int i = 0; i < 2; i++)
			if (tokens.decrement(__FILE__, __LINE__, data))
				values.push_back(tokens.next());
	}
	else if (type == "SFColor" || type == "SFVec3f")
	{
		for (int i = 0; i < 3; i++)
		{
			tokens.increment(true);
			tokens.expect<parse::number>();
		}

		for (int i = 0; i < 3; i++)
			if (tokens.decrement(__FILE__, __LINE__, data))
				values.push_back(tokens.next());
	}
	else if (type == "SFFloat" || type == "SFInt32" || type == "SFTime")
	{
		tokens.increment(true);
		tokens.expect<parse::number>();

		if (tokens.decrement(__FILE__, __LINE__, data))
			values.push_back(tokens.next());
	}
	else if (type == "SFImage")
	{
		tokens.increment(true);
		tokens.expect<parse::number>();

		while (tokens.decrement(__FILE__, __LINE__, data))
		{
			values.push_back(tokens.next());

			tokens.increment(false);
			tokens.expect<parse::number>();
		}
	}
	else if (type == "SFRotation")
	{
		for (int i = 0; i < 4; i++)
		{
			tokens.increment(true);
			tokens.expect<parse::number>();
		}

		for (int i = 0; i < 4; i++)
			if (tokens.decrement(__FILE__, __LINE__, data))
				values.push_back(tokens.next());
	}
	else if (type == "SFString")
	{
		tokens.increment(true);
		tokens.expect<parse::text>();

		if (tokens.decrement(__FILE__, __LINE__, data))
			values.push_back(tokens.next());
	}
	else if (type == "SFNode")
	{
		tokens.increment(true);
		tokens.expect<nodeDeclaration>();
		tokens.expect("NULL");

		if (tokens.decrement(__FILE__, __LINE__, data))
		{
			if (tokens.found<nodeDeclaration>())
				node = new nodeDeclaration(tokens, data);
			else
				tokens.next();
		}
	}

	tokens.syntax_end(this);
}

bool sfValue::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next("FALSE", i) || tokens.is_next("TRUE") ||
		   tokens.is_next<parse::number>() || tokens.is_next<parse::text>() ||
		   nodeDeclaration::is_next(tokens, i, data);
}

void sfValue::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<sfValue>())
	{
		tokens.register_syntax<sfValue>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::number>();
		tokens.register_token<parse::text>();
	}
}

string sfValue::to_string(string tab) const
{
	if (node != NULL && node->valid)
		return node->to_string(tab);
	else
	{
		string result = "";
		for (int i = 0; i < values.size(); i++)
		{
			if (i != 0)
				result += " ";

			result += values[i];
		}
		return result;
	}
}

parse::syntax *sfValue::clone() const
{
	return new sfValue(*this);
}

sfValue &sfValue::operator=(const sfValue &value)
{
	type = value.type;
	values = value.values;
	if (value.node != NULL)
		node = (nodeDeclaration*)value.node->clone();
	else
		node = NULL;
	return *this;
}

}
