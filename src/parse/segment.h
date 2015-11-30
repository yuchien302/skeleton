/*
 * segment.h
 *
 *  Created on: Oct 15, 2014
 *      Author: nbingham
 */

#include <common/standard.h>
#include "token.h"

#ifndef parse_segment_h
#define parse_segment_h

struct segment
{
	segment();
	~segment();

	string name;
	string buffer;
	vector<int> lines;
	vector<token> tokens;

	int char_to_line(int character_index);
	int char_to_token(int character_index);
	int token_to_line(int token_index);
	int token_to_char(int token_index);
	int line_to_token(int line_number);
	int line_to_char(int line_number);

	segment subseg(int character_index, int character_length);
	string substr(int character_index, int character_length);

	string get_token(int token_index);
	string get_line(int line_index);
};

#endif
