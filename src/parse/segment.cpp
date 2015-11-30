/*
 * segment.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: nbingham
 */

#include "segment.h"

segment::segment()
{
}

segment::~segment()
{

}

int segment::char_to_line(int character_index)
{
	return (upper_bound(lines.begin(), lines.end(), character_index) - lines.begin()) - 1;
}

int segment::char_to_token(int character_index)
{
	return (upper_bound(tokens.begin(), tokens.end(), ::token(character_index, character_index, 0)) - tokens.begin()) - 1;
}

int segment::token_to_line(int token_index)
{
	return char_to_line(tokens[token_index].start);
}

int segment::token_to_char(int token_index)
{
	return tokens[token_index].start;
}

int segment::line_to_token(int line_number)
{
	return char_to_token(lines[line_number]);
}

int segment::line_to_char(int line_number)
{
	return lines[line_number];
}

segment segment::subseg(int character_index, int character_length)
{
	segment result;
	result.name = name;
	result.buffer = buffer.substr(character_index, character_length);
	vector<int>::iterator start = upper_bound(lines.begin(), lines.end(), character_index);
	vector<int>::iterator end = lower_bound(start, lines.end(), character_index+character_length);
	result.lines.reserve(end - start + 1);
	result.lines.push_back(0);
	for (vector<int>::iterator i = start+1; i != end; i++)
		result.lines.push_back(*i - character_length);

	return result;
}

string segment::substr(int character_index, int character_length)
{
	return buffer.substr(character_index, character_length);
}

string segment::get_token(int token_index)
{
	if (token_index >= 0 && token_index < (int)tokens.size())
		return buffer.substr(tokens[token_index].start, tokens[token_index].end - tokens[token_index].start);
	else
		return "";
}

string segment::get_line(int line_index)
{
	if (line_index >= 0 && line_index < (int)lines.size()-1)
		return buffer.substr(lines[line_index], lines[line_index+1]-lines[line_index]);
	else if (line_index >= 0 && line_index == (int)lines.size()-1)
		return buffer.substr(lines[line_index]);
	else
		return "";
}
