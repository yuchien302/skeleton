/*
 * token.h
 *
 *  Created on: Oct 28, 2014
 *      Author: nbingham
 */

#ifndef parse_token_h
#define parse_token_h

struct token
{
	token();
	token(int s, int e, int t);
	~token();

	int start;
	int end;
	int type;
};

bool operator<(token t1, token t2);

#endif
