/*
 * scene.cpp
 *
 *  Created on: Feb 3, 2014
 *      Author: nbingham
 */

#include "scene.h"
#include <parse/default/instance.h>
#include <parse/default/symbol.h>
#include <parse/default/line_comment.h>

namespace parse_vrml
{

scene::scene()
{
	debug_name = "scene";
	variables = NULL;
	owner = false;
}

scene::scene(const scene &cpy) : parse::syntax(cpy)
{
	declarations = cpy.declarations;
	variables = cpy.variables;
	owner = false;
}

scene::scene(tokenizer &tokens, void *data)
{
	debug_name = "scene";
	variables = NULL;
	owner = false;
	parse(tokens, data);
}

scene::~scene()
{
	if (owner && variables != NULL)
		delete variables;
	variables = NULL;
}

void scene::parse(tokenizer &tokens, void *data)
{
	variables = (globals*)data;
	if (variables == NULL)
	{
		variables = new globals();
		owner = true;
	}
	tokens.syntax_start(this);

	tokens.increment(true);
	tokens.expect<declaration>();

	while (tokens.decrement(__FILE__, __LINE__, variables))
	{
		declarations.push_back(declaration(tokens, variables));

		tokens.increment(false);
		tokens.expect<declaration>();
	}

	tokens.syntax_end(this);
}

bool scene::is_next(tokenizer &tokens, int i, void *data)
{
	return declaration::is_next(tokens, i, data);
}

void scene::register_syntax(tokenizer &tokens)
{
	if (!tokens.syntax_registered<scene>())
	{
		tokens.register_syntax<scene>();
		declaration::register_syntax(tokens);
		tokens.register_token<parse::instance>();
		tokens.register_token<parse::symbol>();
		tokens.register_comment<parse::line_comment>();
	}
}

string scene::to_string(string tab) const
{
	string result = "";
	for (int i = 0; i < (int)declarations.size(); i++)
		result += tab + declarations[i].to_string(tab) + "\n";
	return result;
}

parse::syntax *scene::clone() const
{
	return new scene(*this);
}

}
