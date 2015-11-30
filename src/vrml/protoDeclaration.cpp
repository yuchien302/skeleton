/*
 * protoDeclaration.cpp
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "protoDeclaration.h"
#include "interfaceDeclaration.h"
#include "scene.h"

#include <parse/default/instance.h>
#include <parse/default/symbol.h>

namespace parse_vrml
{
protoDeclaration::protoDeclaration()
{
	debug_name = "protoDeclaration";
	sub = NULL;
	external = false;
}

protoDeclaration::protoDeclaration(const protoDeclaration &cpy) : parse::syntax(cpy)
{
	external = cpy.external;
	nodeType = cpy.nodeType;
	interfaces = cpy.interfaces;
	value = cpy.value;
	if (cpy.sub != NULL)
		sub = (parse_vrml::scene*)cpy.sub->clone();
	else
		sub = NULL;
}

protoDeclaration::protoDeclaration(tokenizer &tokens, void *data)
{
	debug_name = "protoDeclaration";
	external = false;
	sub = NULL;
	parse(tokens, data);
}

protoDeclaration::~protoDeclaration()
{
	if (sub != NULL)
		delete sub;
	sub = NULL;
}

void protoDeclaration::parse(tokenizer &tokens, void *data)
{
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect("]");

	tokens.increment(true);
	tokens.expect<interfaceDeclaration>();

	tokens.increment(true);
	tokens.expect("[");

	tokens.increment(true);
	tokens.expect<parse::instance>();

	tokens.increment(true);
	tokens.expect("PROTO");
	tokens.expect("EXTERNPROTO");

	if (tokens.decrement(__FILE__, __LINE__, data))
	{
		if (tokens.found("PROTO"))
			external = false;
		else if (tokens.found("EXTERNPROTO"))
			external = true;
	}

	if (tokens.decrement(__FILE__, __LINE__, data))
		nodeType = tokens.next();

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	while (tokens.decrement(__FILE__, __LINE__, data))
	{
		interfaces.push_back(interfaceDeclaration(tokens, external, data));

		tokens.increment(false);
		tokens.expect<interfaceDeclaration>();
	}

	if (tokens.decrement(__FILE__, __LINE__, data))
		tokens.next();

	if (external)
	{
		tokens.increment(true);
		tokens.expect<fieldValue>();

		if (tokens.decrement(__FILE__, __LINE__, data))
			value = fieldValue(tokens, "MFString", data);
	}
	else
	{
		tokens.increment(true);
		tokens.expect("}");

		tokens.increment(true);
		tokens.expect<scene>();

		tokens.increment(true);
		tokens.expect("{");

		if (tokens.decrement(__FILE__, __LINE__, data))
			tokens.next();

		if (tokens.decrement(__FILE__, __LINE__, data))
			sub = new scene(tokens, data);

		if (tokens.decrement(__FILE__, __LINE__, data))
			tokens.next();
	}

	tokens.syntax_end(this);
}

bool protoDeclaration::is_next(tokenizer &tokens, int i, void *data)
{
	return tokens.is_next("PROTO", i) ||
		   tokens.is_next("EXTERNPROTO", i);
}

void protoDeclaration::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<protoDeclaration>())
	{
		tokens.register_syntax<protoDeclaration>();
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::symbol>();
		fieldValue::register_syntax(tokens);
		interfaceDeclaration::register_syntax(tokens);
	}
}

string protoDeclaration::to_string(string tab) const
{
	string result;

	if (external)
		result += "EXTERN";

	result += "PROTO " + nodeType + " [";

	for (int i = 0; i < interfaces.size(); i++)
	{
		if (i != 0)
			result += " ";

		result += interfaces[i].to_string(tab);
	}

	result += "]";

	if (external)
		result += value.to_string(tab);
	else if (sub != NULL)
		result += " { " + sub->to_string(tab) + " }";
	else
		result += " { NULL }";

	return result;
}

parse::syntax *protoDeclaration::clone() const
{
	return new protoDeclaration(*this);
}

}
