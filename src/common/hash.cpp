/*
 * hashmap.cpp
 *
 *  Created on: Jun 20, 2015
 *      Author: nbingham
 */

#include "hash.h"

hasher::hasher()
{

}

hasher::~hasher()
{

}

void hasher::put(const char *value, int n)
{
	data.reserve(data.size() + sizeof(char)*n);
	for (int i = 0; i < (int)sizeof(char)*n; i++)
		data.push_back(*(((const char *)value) + i));
}

void hasher::put(const unsigned char *value, int n)
{
	data.reserve(data.size() + sizeof(unsigned char)*n);
	for (int i = 0; i < (int)sizeof(unsigned char)*n; i++)
		data.push_back(*(((const char *)value) + i));
}

void hasher::put(const short *value, int n)
{
	data.reserve(data.size() + sizeof(short)*n);
	for (int i = 0; i < (int)sizeof(short)*n; i++)
		data.push_back(*(((const char *)value) + i));
}

void hasher::put(const unsigned short *value, int n)
{
	data.reserve(data.size() + sizeof(unsigned short)*n);
	for (int i = 0; i < (int)sizeof(unsigned short)*n; i++)
		data.push_back(*(((const char *)value) + i));
}

void hasher::put(const int *value, int n)
{
	data.reserve(data.size() + sizeof(int)*n);
	for (int i = 0; i < (int)sizeof(int)*n; i++)
		data.push_back(*(((const char *)value) + i));
}

void hasher::put(const unsigned int *value, int n)
{
	data.reserve(data.size() + sizeof(unsigned int)*n);
	for (int i = 0; i < (int)sizeof(unsigned int)*n; i++)
		data.push_back(*(((const char *)value) + i));
}

void hasher::put(const long *value, int n)
{
	data.reserve(data.size() + sizeof(long)*n);
	for (int i = 0; i < (int)sizeof(long)*n; i++)
		data.push_back(*(((const char *)value) + i));
}

void hasher::put(const unsigned long *value, int n)
{
	data.reserve(data.size() + sizeof(unsigned long)*n);
	for (int i = 0; i < (int)sizeof(unsigned long)*n; i++)
		data.push_back(*(((const char *)value) + i));
}

void hasher::put(const long long *value, int n)
{
	data.reserve(data.size() + sizeof(long long)*n);
	for (int i = 0; i < (int)sizeof(long long)*n; i++)
		data.push_back(*(((const char *)value) + i));
}

void hasher::put(const unsigned long long *value, int n)
{
	data.reserve(data.size() + sizeof(unsigned long long)*n);
	for (int i = 0; i < (int)sizeof(unsigned long long)*n; i++)
		data.push_back(*(((const char *)value) + i));
}

void hasher::put(string value)
{
	data.reserve(data.size() + value.size());
	for (int i = 0; i < (int)value.size(); i++)
		data.push_back(value[i]);
}

void hasher::put(const bool *value, int n)
{
	data.reserve(data.size() + sizeof(bool)*n);
	for (int i = 0; i < (int)sizeof(bool)*n; i++)
		data.push_back(*(((const char *)value) + i));
}

void hasher::put(const float *value, int n)
{
	data.reserve(data.size() + sizeof(float)*n);
	for (int i = 0; i < (int)sizeof(float)*n; i++)
		data.push_back(*(((const char *)value) + i));
}

void hasher::put(const double *value, int n)
{
	data.reserve(data.size() + sizeof(double)*n);
	for (int i = 0; i < (int)sizeof(double)*n; i++)
		data.push_back(*(((const char *)value) + i));
}

uint32_t hasher::get()
{
	uint32_t len = data.size();
	const char* ptr = data.data();
	uint32_t hash = len, tmp;
	int rem;

	if (len <= 0 || ptr == NULL)
		return 0;

	rem = len & 3;
	len >>= 2;

	/* Main loop */
	for (;len > 0; len--)
	{
		hash  += (*((const uint16_t *) (ptr)));
		tmp    = ((*((const uint16_t *) (ptr+2))) << 11) ^ hash;
		hash   = (hash << 16) ^ tmp;
		ptr  += 2*sizeof (uint16_t);
		hash  += hash >> 11;
	}

	/* Handle end cases */
	switch (rem)
	{
		case 3: hash += (*((const uint16_t *) (ptr)));
				hash ^= hash << 16;
				hash ^= ((signed char)ptr[sizeof (uint16_t)]) << 18;
				hash += hash >> 11;
				break;
		case 2: hash += (*((const uint16_t *) (ptr)));
				hash ^= hash << 11;
				hash += hash >> 17;
				break;
		case 1: hash += (signed char)*ptr;
				hash ^= hash << 10;
				hash += hash >> 1;
	}

	/* Force "avalanching" of final 127 bits */
	hash ^= hash << 3;
	hash += hash >> 5;
	hash ^= hash << 4;
	hash += hash >> 17;
	hash ^= hash << 25;
	hash += hash >> 6;

	data.clear();
	return hash;
}
