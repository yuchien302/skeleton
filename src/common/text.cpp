/*
 * text.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: nbingham
 */

#include "text.h"

/** Returns true if 'c' has a value that is legal for the first character in a name.
 */
bool ac(char c)
{
	return ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') );
}


/** Returns true if 'c' has a value that is legal in a type or variable name.
 */
bool nc(char c)
{
	return ((c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') ||
			(c == '_') || (c == '.'));
}

/** Returns true if 'c' is an operator character.
 */
bool oc(char c)
{
	return (c == ':' ||
			c == '=' ||
			c == '|' ||
			c == '&' ||
			c == '~' ||
			c == '>' ||
			c == '<' ||
			c == ';' ||
			c == '*' ||
			c == '[' ||
			c == ']' ||
			c == '(' ||
			c == ')' ||
			c == '{' ||
			c == '}' ||
			c == '+' ||
			c == '-' ||
			c == '!' ||
			c == '?' ||
			c == '@' ||
			c == '#' ||
			c == '/');
}

/** Returns true if 'c' is a whitespace character
 */
bool sc(char c)
{
	return (c == ' '  ||
			c == '\t' ||
			c == '\n' ||
			c == '\r');
}

int edit_distance(const string &s1, const string &s2)
{
	vector<int> col((int)s2.size()+1);
	vector<int> prev_col((int)s2.size()+1);

	for (int i = 0; i < (int)prev_col.size(); i++)
		prev_col[i] = i;

	for (int i = 0; i < (int)s1.size(); i++)
	{
		col[0] = i+1;

		for (int j = 0; j < (int)s2.size(); j++)
			col[j+1] = min(min(prev_col[1 + j] + 1, col[j] + 1), prev_col[j] + (s1[i] == s2[j] ? 0 : 1));

		col.swap(prev_col);
	}

	return prev_col[s2.size()];
}

int get_column_end(string line, int column_start, string *space)
{
	string str;
	int column_end = 1;
	for (int j = 0; j < (int)line.size() && j < column_start; j++)
	{
		if (line[j] != '\t')
		{
			str.push_back(' ');
			column_end++;
		}
		else
		{
			str.push_back('\t');
			column_end+=8;
		}
	}

	if (space != NULL)
		*space = str;
	return column_end;
}

string line_wrap(string str, int length)
{
	string result;
	for (int i = 0; i < (int)str.size(); i += length)
	{
		result += str.substr(i, min((int)str.size(), i+length) - i);
		if (i+length < (int)str.size())
			result += "\n";
	}
	return result;
}
