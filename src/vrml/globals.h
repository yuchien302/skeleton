/*
 * globals.h
 *
 *  Created on: Aug 15, 2015
 *      Author: nbingham
 */

#include "common/standard.h"

#ifndef vrml_globals_h
#define vrml_globals_h

namespace parse_vrml
{

struct addr
{
	addr();
	addr(string kind, string name);
	~addr();

	string kind;
	string name;
};

bool operator<(addr a0, addr a1);
bool operator>(addr a0, addr a1);
bool operator<=(addr a0, addr a1);
bool operator>=(addr a0, addr a1);
bool operator==(addr a0, addr a1);
bool operator!=(addr a0, addr a1);

struct field
{
	field();
	field(string kind, string type);
	~field();

	string kind;
	string type;
};

struct globals
{
	globals();
	~globals();

	map<string, field> fields;
};

}

#endif
