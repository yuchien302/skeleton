/*
 * tokenizer.h
 *
 *  Created on: Apr 7, 2014
 *      Author: nbingham
 */

#include <common/text.h>
#include <common/message.h>
#include "segment.h"
#include "token.h"
#include "syntax.h"

#ifndef parse_tokenizer_h
#define parse_tokenizer_h

struct tokenizer;

typedef bool (*is_next_ptr)(tokenizer&, int, void*);
typedef token (*consume_ptr)(tokenizer&, void *);

struct token_entry
{
	token_entry(is_next_ptr n, consume_ptr c, int t)
	{
		is_next = n;
		consume = c;
		type = t;
	}

	~token_entry(){}

	is_next_ptr is_next;
	consume_ptr consume;
	int type;
};

/** This structure splits up multiple input files onto a stream of
 *  tokens and then facilitates search and iteration though those
 *  tokens and handling errors.
 */
struct tokenizer
{
	tokenizer();
	tokenizer(bool export_messages);
	~tokenizer();

	map<string, is_next_ptr> syntax_registry;
	typedef map<string, is_next_ptr>::iterator syntax_registry_iterator;
	map<string, token_entry> token_registry;
	typedef map<string, token_entry>::iterator token_registry_iterator;
	vector<pair<is_next_ptr, consume_ptr> > comment_registry;
	vector<segment> segments;
	vector<int> index;
	int offset;
	int segment_index;

	vector<pair<int, vector<int> > > stack;

	vector<pair<vector<string>, bool> > expected_hierarchy;
	typedef vector<pair<vector<string>, bool> >::iterator level;
	string found_type;

	map<string, pair<string, int> > bookmarks;

	int num_errors;
	int num_warnings;
	int num_internal;
	int num_notes;
	int num_log;
	bool export_messages;

	void internal(string internal, string debug_file, int debug_line, int token_offset = 0, int character_offset = 0);
	void error(string error, string debug_file, int debug_line, int token_offset = 0, int character_offset = 0);
	void warning(string warning, string debug_file, int debug_line, int token_offset = 0, int character_offset = 0);
	void note(string note, string debug_file, int debug_line, int token_offset = 0, int character_offset = 0);
	void log(string log, string debug_file, int debug_line, int token_offset = 0, int character_offset = 0);

	void token_internal(string internal, string debug_file, int debug_line, int character_offset = 0);
	void token_error(string error, string debug_file, int debug_line, int character_offset = 0);
	void token_warning(string warning, string debug_file, int debug_line, int character_offset = 0);
	void token_note(string note, string debug_file, int debug_line, int character_offset = 0);
	void token_log(string log, string debug_file, int debug_line, int character_offset = 0);
	bool is_clean();

	void syntax_start(parse::syntax *syntax);
	void syntax_end(parse::syntax *syntax);

	bool save(string key, const parse::syntax *syntax);
	bool load(const parse::syntax *syntax);
	bool load(string key);
	bool erase(string key);

	void push();
	void pop();

	template <class type>
	void register_syntax()
	{
		syntax_registry.insert(pair<string, is_next_ptr>("[" + type().debug_name + "]", &type::is_next));
	}

	template <class type>
	bool syntax_registered()
	{
		return (syntax_registry.find("[" + type().debug_name + "]") != syntax_registry.end());
	}

	template <class type>
	void register_comment()
	{
		comment_registry.push_back(pair<is_next_ptr, consume_ptr>(&type::is_next, &type::consume));
	}

	template <class type>
	bool comment_registered()
	{
		return (find(comment_registry.begin(), comment_registry.end(), pair<is_next_ptr, consume_ptr>(&type::is_next, &type::consume)) != comment_registry.end());
	}

	template <class type>
	int comment_type()
	{
		return (find(comment_registry.begin(), comment_registry.end(), pair<is_next_ptr, consume_ptr>(&type::is_next, &type::consume)) - comment_registry.begin());
	}

	template <class type>
	void register_token()
	{
		string name = "[" + type().debug_name + "]";
		if (token_registry.find(name) == token_registry.end())
			token_registry.insert(pair<string, token_entry>(name, token_entry(&type::is_next, &type::consume, (int)token_registry.size())));
	}

	template <class type>
	bool token_registered()
	{
		return (token_registry.find("[" + type().debug_name + "]") != token_registry.end());
	}

	template <class type>
	int token_type()
	{
		return token_registry.find("[" + type().debug_name + "]")->second.type;
	}

	level increment(bool required = true);
	level increment(level it, bool required = true);
	bool decrement(string debug_file, int debug_line, void *data = NULL);

	void expect(string s);
	void expect(vector<string> &s);
	template <class type>
	void expect()
	{
		if (!syntax_registered<type>() && !token_registered<type>())
			internal("syntax or token not registered \"" + type().debug_name + "\"", __FILE__, __LINE__);
		expected_hierarchy.back().first.push_back("[" + type().debug_name + "]");
	}

	void expect(level it, string s);
	void expect(level it, vector<string> &s);
	template <class type>
	void expect(level it)
	{
		if (!syntax_registered<type>() && !token_registered<type>())
			internal("syntax or token not registered \"" + type().debug_name + "\"", __FILE__, __LINE__);
		it->first.push_back("[" + type().debug_name + "]");
	}
	
	pair<int, int> expected(int off = 1, void *data = NULL);
	pair<int, int> expected(int off = 1);
	pair<int, int> expected(string s);

	int number_expected(int required_index);

	bool found(string s);
	template <class type>
	bool found()
	{
		return (found_type == ("[" + type().debug_name + "]"));
	}

	void insert(string name, string contents, void *data = NULL);

	bool is_next(string str, int i = 1);
	template <class type>
	bool is_next(int i = 1)
	{
		return (peek_type(i) == "[" + type().debug_name + "]");
	}

	void normalize_token(int &segment_index, int &token_index);
	void normalize_character(int &segment_index, int &character_index);
	void normalize(int &segment_index, int &token_index, int &character_offset);
	string file(int segment_offset = 0, int token_offset = 0, int character_offset = 0);
	string line(int segment_offset = 0, int token_offset = 0, int character_offset = 0);
	string absolute_location(int segment_index, int character_index, string *line = NULL, string *space = NULL);
	string relative_location(int token_offset, int character_offset, string *line = NULL, string *space = NULL);
	int line_number(int segment_offset = 0, int token_offset = 0, int character_offset = 0);
	int line_offset(int segment_offset = 0, int token_offset = 0, int character_offset = 0);
	int character_offset(int segment_offset = 0, int token_offset = 0, int character_offset = 0);

	char curr_char();
	char prev_char();
	char next_char();
	char peek_char(int i);

	string curr_type();
	string curr();
	string prev();
	string next();
	string peek_type(int i);
	string peek(int i);

	bool segment_loaded(string name);
	bool segment_loading(string name);

	void reset();
};

#endif
