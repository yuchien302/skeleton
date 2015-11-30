/*
 * nodeGut.cpp
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "nodeGut.h"
#include "globals.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>

namespace parse_vrml
{
nodeGut::nodeGut()
{
	debug_name = "nodeGut";
}

nodeGut::nodeGut(tokenizer &tokens, string nodeType, void *data)
{
	debug_name = "nodeGut";
	this->nodeType = nodeType;
	parse(tokens, data);
}

nodeGut::~nodeGut()
{

}

void nodeGut::parse(tokenizer &tokens, void *data)
{
	globals *variables = (globals*)data;

	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect<routeDeclaration>();
	tokens.expect<protoDeclaration>();
	tokens.expect<parse::instance>();

	if (tokens.decrement(__FILE__, __LINE__, data))
	{
		if (tokens.found<routeDeclaration>())
			route = routeDeclaration(tokens, data);
		else if (tokens.found<protoDeclaration>())
			proto = protoDeclaration(tokens, data);
		else if (tokens.found<parse::instance>())
		{
			first = tokens.next();

			string type = "";
			if (variables != NULL)
			{
				map<string, field>::iterator type_iter = variables->fields.find(nodeType + "." + first);
				if (type_iter == variables->fields.end())
					tokens.error("unrecognized field '" + nodeType + "." + first + "'", __FILE__, __LINE__);
				else
					type = type_iter->second.type;
			}

			tokens.increment(false);
			tokens.expect("IS");
			tokens.expect<fieldValue>();

			if (tokens.decrement(__FILE__, __LINE__, data))
			{
				if (tokens.found<fieldValue>())
					value = fieldValue(tokens, type, data);
				else if (tokens.found("IS"))
				{
					tokens.next();

					tokens.increment(true);
					tokens.expect<parse::instance>();

					if (tokens.decrement(__FILE__, __LINE__, data))
						second = tokens.next();
				}
			}
		}
	}

	tokens.syntax_end(this);
}

bool nodeGut::is_next(tokenizer &tokens, int i, void *data)
{
	return protoDeclaration::is_next(tokens, i, data) ||
		   routeDeclaration::is_next(tokens, i, data) ||
		   tokens.is_next<parse::instance>(i);
}

void nodeGut::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<nodeGut>())
	{
		tokens.register_syntax<nodeGut>();
		tokens.register_token<parse::instance>();
		protoDeclaration::register_syntax(tokens);
		routeDeclaration::register_syntax(tokens);
		fieldValue::register_syntax(tokens);
	}
}

string nodeGut::to_string(string tab) const
{
	if (proto.valid)
		return proto.to_string(tab);
	else if (route.valid)
		return route.to_string(tab);
	else if (value.valid)
		return first + " " + value.to_string(tab);
	else
		return first + " " + second;
}

parse::syntax *nodeGut::clone() const
{
	return new nodeGut(*this);
}

}
