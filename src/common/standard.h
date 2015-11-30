/*

 * standard.h
 *
 * Common is a collection of functions not specific to the compiler that
 * we found useful to define. Note that our #defines for user flags are
 * also stored in common.h as it is accessed by everyone.
 */

#ifndef common_standard_h
#define common_standard_h

#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <sstream>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <vector>
#include <set>
#include <list>
#include <map>
#include <string>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <iterator>
#include <deque>
#include <limits>
#include "hash.h"

using namespace std;

template <typename type>
map<type, int> count_elements(const vector<type> &v1, map<type, int> &result = map<type, int>())
{
	for (typename vector<type>::const_iterator i = v1.begin(); i != v1.end(); i++)
	{
		typename map<type, int>::iterator it = result.find(*i);
		if (it != result.end())
			it->second++;
		else
			result.insert(pair<type, int>(*i, 1));
	}

	return result;
}

template <typename type>
int vector_intersection_size(const vector<type> &v1, const vector<type> &v2)
{
	int result = 0;
	typename vector<type>::const_iterator i, j;
	for (i = v1.begin(), j = v2.begin(); i != v1.end() && j != v2.end();)
	{
		if (*j > *i)
			i++;
		else if (*i > *j)
			j++;
		else
		{
			result++;
			i++;
			j++;
		}
	}

	return result;
}

template <typename type>
void vector_symmetric_compliment(vector<type> &v1, vector<type> &v2)
{
	vector<type> result;
	typename vector<type>::iterator i, j;
	for (i = v1.begin(), j = v2.begin(); i != v1.end() && j != v2.end();)
	{
		if (*j > *i)
			i++;
		else if (*i > *j)
			j++;
		else
		{
			i = v1.erase(i);
			j = v2.erase(j);
		}
	}
}

template <typename type>
ostream &operator<<(ostream &os, vector<type> s)
{
	os << "{";
	for (size_t i = 0; i < s.size(); i++)
	{
		if (i != 0)
			os << ", ";
		os << s[i];
	}
	os << "}";
	return os;
}

template <typename type>
ostream &operator<<(ostream &os, list<type> s)
{
	os << "{";
	for (typename list<type>::iterator i = s.begin(); i != s.end(); i++)
	{
		if (i != s.begin())
			os << ", ";
		os << *i;
	}
	os << "}";
	return os;
}

template <typename type1, typename type2>
ostream &operator<<(ostream &os, map<type1, type2> s)
{
	os << "{";
	for (typename map<type1, type2>::iterator i = s.begin(); i != s.end(); i++)
	{
		if (i != s.begin())
			os << ", ";
		os << i->first << "=>" << i->second;
	}
	os << "}";
	return os;
}

template <typename type1, typename type2>
ostream &operator<<(ostream &os, pair<type1, type2> s)
{
	os << "(" << s.first << ", " << s.second << ")";
	return os;
}

#endif
