/*
 * token.cpp
 *
 *  Created on: Jan 22, 2015
 *      Author: nbingham
 */

#include "token.h"

token::token()
{
	start = -1;
	end = -1;
	type = -1;
}

token::token(int s, int e, int t)
{
	start = s;
	end = e;
	type = t;
}

token::~token()
{

}

bool operator<(token t1, token t2)
{
	return (t1.start < t2.start) ||
		   (t1.start == t2.start && t1.end < t2.end) ||
		   (t1.start == t2.start && t1.end == t2.end && t1.type < t2.type);
}
