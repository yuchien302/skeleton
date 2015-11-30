/*
 * hashmap.h
 *
 *  Created on: Jun 20, 2015
 *      Author: nbingham
 */

#include <vector>
#include <list>
#include <string>
#include <map>
#include <sys/types.h>
#include <stdint.h>

using namespace std;

#ifndef common_hashmap_h
#define common_hashmap_h

struct hasher
{
	hasher();

	template <class type0, class type1>
	hasher(const pair<type0, type1> *value)
	{
		put(value);
	}

	template <class type>
	hasher(const type *value, int n = 1)
	{
		put(value, n);
	}

	template <class type>
	hasher(const vector<type> *value)
	{
		put(value);
	}

	template <class type>
	hasher(const list<type> *value)
	{
		put(value);
	}

	template <class key_type, class value_type>
	hasher(const map<key_type, value_type> *lst)
	{
		put(lst);
	}

	~hasher();

	vector<char> data;

	template <class type>
	void put(const type *value, int n = 1)
	{
		for (int i = 0; i < n; i++)
		{
			value->hash(*this);
			value++;
		}
	}

	void put(const char *value, int n = 1);
	void put(const unsigned char *value, int n = 1);
	void put(const short *value, int n = 1);
	void put(const unsigned short *value, int n = 1);
	void put(const int *value, int n = 1);
	void put(const unsigned int *value, int n = 1);
	void put(const long *value, int n = 1);
	void put(const unsigned long *value, int n = 1);
	void put(const long long *value, int n = 1);
	void put(const unsigned long long *value, int n = 1);
	void put(string value);
	void put(const bool *value, int n = 1);
	void put(const float *value, int n = 1);
	void put(const double *value, int n = 1);

	template <class type0, class type1>
	void put(const pair<type0, type1> *value)
	{
		put(&value->first);
		put(&value->second);
	}

	template <class type>
	void put(const vector<type> *lst)
	{
		put(lst->data(), (int)lst->size());
	}

	template <class type>
	void put(const list<type> *lst)
	{
		for (typename list<type>::iterator i = lst->begin(); i != lst->end(); i++)
			put(&(*i));
	}

	template <class key_type, class value_type>
	void put(const map<key_type, value_type> *lst)
	{
		for (typename map<key_type, value_type>::iterator i = lst->begin(); i != lst->end(); i++)
		{
			put(&i->first);
			put(&i->second);
		}
	}

	uint32_t get();
};


template <class key_type, class value_type, int num_buckets>
struct hashmap
{
	map<key_type, value_type> buckets[num_buckets];
	int count;

	bool insert(const key_type &key, const value_type &value, typename map<key_type, value_type>::iterator* loc = NULL)
	{
		int bucket = hasher(&key).get()%num_buckets;
		pair<typename map<key_type, value_type>::iterator, bool> result = buckets[bucket].insert(pair<key_type, value_type>(key, value));
		if (loc != NULL)
			*loc = result.first;
		count++;
		return result.second;
	}

	bool find(const key_type &key, typename map<key_type, value_type>::iterator* loc = NULL)
	{
		int bucket = hasher(&key).get()%num_buckets;
		typename map<key_type, value_type>::iterator result = buckets[bucket].find(key);
		if (loc != NULL)
			*loc = result;
		return (result != buckets[bucket].end());
	}

	void erase(typename map<key_type, value_type>::iterator iter)
	{
		int bucket = hasher(&iter->first).get()%num_buckets;
		buckets[bucket].erase(iter);
	}

	int max_bucket_size()
	{
		int max_size = 0;
		for (int i = 0; i < num_buckets; i++)
			if ((int)buckets[i].size() > max_size)
				max_size = buckets[i].size();
		return max_size;
	}
};

template <class value_type, int num_buckets>
struct hashtable
{
	vector<value_type> buckets[num_buckets];
	int count;

	bool insert(const value_type &value, value_type** loc = NULL)
	{
		int bucket = hasher(&value).get()%num_buckets;
		typename vector<value_type>::iterator result = lower_bound(buckets[bucket].begin(), buckets[bucket].end(), value);
		if (result == buckets[bucket].end() || *result != value)
		{
			result = buckets[bucket].insert(result, value);
			count++;
			if (loc != NULL)
				*loc = &(*result);

			return true;
		}

		if (loc != NULL)
			*loc = &(*result);

		return false;
	}

	bool contains(const value_type &value, value_type** loc = NULL)
	{
		int bucket = hasher(value).get()%num_buckets;
		typename vector<value_type>::iterator result = lower_bound(buckets[bucket].begin(), buckets[bucket].end(), value);
		if (loc != NULL)
			*loc = &(*result);

		return (result != buckets[bucket].end() && *result == value);
	}

	void erase(const value_type &value)
	{
		int bucket = hasher(value).get()%num_buckets;
		typename vector<value_type>::iterator result = lower_bound(buckets[bucket].begin(), buckets[bucket].end(), value);
		buckets[bucket].erase(result);
	}

	int max_bucket_size()
	{
		int max_size = 0;
		for (int i = 0; i < num_buckets; i++)
			if ((int)buckets[i].size() > max_size)
				max_size = buckets[i].size();
		return max_size;
	}
};

#endif
