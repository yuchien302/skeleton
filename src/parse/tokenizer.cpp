/*
 * tokenizer.cpp
 *
 *  Created on: Apr 7, 2014
 *      Author: nbingham
 */

#include "tokenizer.h"

tokenizer::tokenizer()
{
	segment_index = -1;
	offset = -1;
	num_errors = 0;
	num_warnings = 0;
	num_internal = 0;
	num_notes = 0;
	num_log = 0;
	export_messages = true;
}

tokenizer::tokenizer(bool export_messages)
{
	segment_index = -1;
	offset = -1;
	num_errors = 0;
	num_warnings = 0;
	num_internal = 0;
	num_notes = 0;
	num_log = 0;
	this->export_messages = export_messages;
}

tokenizer::~tokenizer()
{

}

void tokenizer::internal(string internal, string debug_file, int debug_line, int token_offset, int character_offset)
{
	string line, space;
	string location = relative_location(token_offset, character_offset, &line, &space);
	printf("%s:%d:%s: internal failure: %s\n", debug_file.c_str(), debug_line, location.c_str(), internal.c_str());
	printf("%s\n%s^\n", line.c_str(), space.c_str());
	if (export_messages)
		::flag_internal(1);
	num_internal++;
	exit(0);
}

void tokenizer::error(string error, string debug_file, int debug_line, int token_offset, int character_offset)
{
	string line, space;
	string location = relative_location(token_offset, character_offset, &line, &space);
	if (::get_debug())
		printf("%s:%d:%s: error: %s\n", debug_file.c_str(), debug_line, location.c_str(), error.c_str());
	else
		printf("%s: error: %s\n", location.c_str(), error.c_str());
	printf("%s\n%s^\n", line.c_str(), space.c_str());
	if (export_messages)
		::flag_errors(1);
	num_errors++;
}

void tokenizer::warning(string warning, string debug_file, int debug_line, int token_offset, int character_offset)
{
	string line, space;
	string location = relative_location(token_offset, character_offset, &line, &space);
	if (::get_debug())
		printf("%s:%d:%s: warning: %s\n", debug_file.c_str(), debug_line, location.c_str(), warning.c_str());
	else
		printf("%s: warning: %s\n", location.c_str(), warning.c_str());
	printf("%s\n%s^\n", line.c_str(), space.c_str());
	if (export_messages)
		::flag_warnings(1);
	num_warnings++;
}

void tokenizer::note(string note, string debug_file, int debug_line, int token_offset, int character_offset)
{
	string line, space;
	string location = relative_location(token_offset, character_offset, &line, &space);
	if (::get_debug())
		printf("%s:%d:%s: note: %s\n", debug_file.c_str(), debug_line, location.c_str(), note.c_str());
	else
		printf("%s: note: %s\n", location.c_str(), note.c_str());
	printf("%s\n%s^\n", line.c_str(), space.c_str());
	if (export_messages)
		::flag_notes(1);
	num_notes++;
}

void tokenizer::log(string log, string debug_file, int debug_line, int token_offset, int character_offset)
{
	if (::get_verbose())
	{
		string location = relative_location(token_offset, character_offset);
		if (::get_debug())
			printf("%s:%d:%s: log: %s\n", debug_file.c_str(), debug_line, location.c_str(), log.c_str());
		else
			printf("%s: log: %s\n", location.c_str(), log.c_str());
		num_log++;
		if (export_messages)
			::flag_log(1);
	}
}

void tokenizer::token_internal(string internal, string debug_file, int debug_line, int character_offset)
{
	string line, space;
	string location = absolute_location(segment_index, offset+character_offset, &line, &space);
	printf("%s:%d:%s: internal failure: %s\n", debug_file.c_str(), debug_line, location.c_str(), internal.c_str());
	printf("%s\n%s^\n", line.c_str(), space.c_str());
	if (export_messages)
		::flag_internal(1);
	num_internal++;
}

void tokenizer::token_error(string error, string debug_file, int debug_line, int character_offset)
{
	string line, space;
	string location = absolute_location(segment_index, offset+character_offset, &line, &space);
	if (::get_debug())
		printf("%s:%d:%s: error: %s\n", debug_file.c_str(), debug_line, location.c_str(), error.c_str());
	else
		printf("%s: error: %s\n", location.c_str(), error.c_str());
	printf("%s\n%s^\n", line.c_str(), space.c_str());
	if (export_messages)
		::flag_errors(1);
	num_errors++;
}

void tokenizer::token_warning(string warning, string debug_file, int debug_line, int character_offset)
{
	string line, space;
	string location = absolute_location(segment_index, offset+character_offset, &line, &space);
	if (::get_debug())
		printf("%s:%d:%s: warning: %s\n", debug_file.c_str(), debug_line, location.c_str(), warning.c_str());
	else
		printf("%s: warning: %s\n", location.c_str(), warning.c_str());
	printf("%s\n%s^\n", line.c_str(), space.c_str());
	if (export_messages)
		::flag_warnings(1);
	num_warnings++;
}

void tokenizer::token_note(string note, string debug_file, int debug_line, int character_offset)
{
	string line, space;
	string location = absolute_location(segment_index, offset+character_offset, &line, &space);
	if (::get_debug())
		printf("%s:%d:%s: note: %s\n", debug_file.c_str(), debug_line, location.c_str(), note.c_str());
	else
		printf("%s: note: %s\n", location.c_str(), note.c_str());
	printf("%s\n%s^\n", line.c_str(), space.c_str());
	if (export_messages)
		::flag_notes(1);
	num_notes++;
}

void tokenizer::token_log(string log, string debug_file, int debug_line, int character_offset)
{
	if (::get_verbose())
	{
		string location = absolute_location(segment_index, offset+character_offset);
		if (::get_debug())
			printf("%s:%d:%s: log: %s\n", debug_file.c_str(), debug_line, location.c_str(), log.c_str());
		else
			printf("%s: log: %s\n", location.c_str(), log.c_str());
		if (export_messages)
			::flag_log(1);
		num_log++;
	}
}

bool tokenizer::is_clean()
{
	return (num_internal == 0 && num_errors == 0);
}

void tokenizer::syntax_start(parse::syntax *syntax)
{
	syntax->segment_name = segments[segment_index].name;
	syntax->start = index[segment_index]+1;
}

void tokenizer::syntax_end(parse::syntax *syntax)
{
	if (syntax->start != -1)
	{
		syntax->end = index[segment_index];
		syntax->valid = true;
	}
}

bool tokenizer::save(string key, const parse::syntax *syntax)
{
	if (syntax == NULL || !syntax->valid)
		return false;
	else
		return bookmarks.insert(pair<string, pair<string, int> >(key, pair<string, int>(syntax->segment_name, syntax->start))).second;
}

bool tokenizer::load(const parse::syntax *syntax)
{
	if (syntax == NULL || !syntax->valid)
		return false;

	bool found = false;
	for (int i = 0; i < (int)segments.size() && !found; i++)
		if (segments[i].name == syntax->segment_name)
		{
			segment_index = i;
			found = true;
		}

	if (found)
	{
		index[segment_index] = syntax->start;
		return true;
	}
	else
		return false;
}

bool tokenizer::load(string key)
{
	map<string, pair<string, int> >::iterator iter = bookmarks.find(key);
	if (iter != bookmarks.end())
	{
		bool found = false;
		for (int i = 0; i < (int)segments.size() && !found; i++)
			if (segments[i].name == iter->second.first)
			{
				segment_index = i;
				found = true;
			}

		if (found)
		{
			index[segment_index] = iter->second.second;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool tokenizer::erase(string key)
{
	map<string, pair<string, int> >::iterator iter = bookmarks.find(key);
	if (iter != bookmarks.end())
	{
		bookmarks.erase(iter);
		return true;
	}
	else
		return false;
}

void tokenizer::push()
{
	stack.push_back(pair<int, vector<int> >(segment_index, index));
}

void tokenizer::pop()
{
	if (stack.size() > 0)
	{
		segment_index = stack.back().first;
		index = stack.back().second;
		stack.pop_back();
	}
}

tokenizer::level tokenizer::increment(bool required)
{
	expected_hierarchy.push_back(pair<vector<string>, bool>(vector<string>(), required));
	return (expected_hierarchy.end()-1);
}

tokenizer::level tokenizer::increment(level it, bool required)
{
	return expected_hierarchy.insert(it, pair<vector<string>, bool>(vector<string>(), required));
}

bool tokenizer::decrement(string debug_file, int debug_line, void *data)
{
	pair<int, int> idx(-1, -1);

	int max_required_level;
	for (max_required_level = (int)expected_hierarchy.size()-1; max_required_level >= 0 && !expected_hierarchy[max_required_level].second; max_required_level--);

	idx = expected(1, data);
	if (idx.first < max_required_level && max_required_level == (int)expected_hierarchy.size()-1)
	{
		string error_string = "expected ";
		vector<string> expect_list;
		for (int j = (int)expected_hierarchy.size()-1; j >= max_required_level; j--)
		{
			for (int k = 0; k < (int)expected_hierarchy[j].first.size(); k++)
			{
				if (expected_hierarchy[j].first[k].size() > 2 && expected_hierarchy[j].first[k][0] == '[' && expected_hierarchy[j].first[k][expected_hierarchy[j].first[k].size()-1] == ']')
					expect_list.push_back(expected_hierarchy[j].first[k].substr(1, expected_hierarchy[j].first[k].size()-2));
				else
					expect_list.push_back("'" + expected_hierarchy[j].first[k] + "'");
			}
		}

		if (expect_list.size() > 0)
		error_string += expect_list[0];

		for (int j = 1; j < (int)expect_list.size()-1; j++)
			error_string += " " + expect_list[j];

		if (expect_list.size() > 1)
			error_string += " or " + expect_list.back();

		error(error_string, debug_file, debug_line, 1);

		int old_segment = segment_index;
		vector<int> old_index = index;

		string next_token;
		while ((idx = expected(1, data)).first < max_required_level && (next_token = next()) != "");

		if (next_token == "")
		{
			segment_index = old_segment;
			index = old_index;
		}
	}

	if (expected_hierarchy.size() == 0)
	{
		string temp = segments[segment_index].get_token(index[segment_index] + 1);
		next();
		internal("nothing expected, found '" + temp + "'", debug_file, debug_line);
		prev();
	}
	else if (idx.first != (int)expected_hierarchy.size() - 1)
	{
		found_type = "";
		expected_hierarchy.pop_back();
	}
	else
	{
		found_type = expected_hierarchy[idx.first].first[idx.second];
		expected_hierarchy.pop_back();
	}

	return (found_type != "");
}

void tokenizer::expect(string s)
{
	expected_hierarchy.back().first.push_back(s);
}

void tokenizer::expect(vector<string> &s)
{
	expected_hierarchy.back().first.insert(expected_hierarchy.back().first.end(), s.begin(), s.end());
}

void tokenizer::expect(level it, string s)
{
	it->first.push_back(s);
}

void tokenizer::expect(level it, vector<string> &s)
{
	it->first.insert(it->first.end(), s.begin(), s.end());
}

pair<int, int> tokenizer::expected(int i, void *data)
{
	vector<pair<int, int> > results;
	if (segment_index >= 0 && segment_index < (int)segments.size())
	{
		for (int j = (int)expected_hierarchy.size()-1; j >= 0 && results.size() == 0; j--)
		{
			vector<string>::iterator j2;
			// Look for raw strings
			for (j2 = expected_hierarchy[j].first.begin(); j2 != expected_hierarchy[j].first.end(); j2++)
				if ((j2->size() <= 2 || j2->at(0) != '[' || j2->at(j2->size()-1) == ']') && segments[segment_index].get_token(index[segment_index] + i) == *j2)
					results.push_back(pair<int, int>(j, j2 - expected_hierarchy[j].first.begin()));

			if (results.size() == 0)
			{
				// Look for specific token types
				token_registry_iterator iter = token_registry.find(peek_type(i));
				if (iter != token_registry.end())
				{
					j2 = std::find(expected_hierarchy[j].first.begin(), expected_hierarchy[j].first.end(), iter->first);
					if (j2 != expected_hierarchy[j].first.end())
						results.push_back(pair<int, int>(j, j2 - expected_hierarchy[j].first.begin()));
				}

				// Look for structures
				for (syntax_registry_iterator iter = syntax_registry.begin(); iter != syntax_registry.end(); iter++)
				{
					j2 = std::find(expected_hierarchy[j].first.begin(), expected_hierarchy[j].first.end(), iter->first);
					if (j2 != expected_hierarchy[j].first.end() && iter->second(*this, i, data))
						results.push_back(pair<int, int>(j, j2 - expected_hierarchy[j].first.begin()));
				}
			}
		}
	}

	if (results.size() > 1)
	{
		internal("ambiguous grammar", __FILE__, __LINE__, 1);
		for (int i = 0; i < (int)results.size(); i++)
			::note("", ::to_string(results[i].first) + " " + ::to_string(results[i].second) + " " + ::to_string(expected_hierarchy[results[i].first].second) + " " + expected_hierarchy[results[i].first].first[results[i].second], __FILE__, __LINE__);
		return pair<int, int>(-1, -1);
	}
	else if (results.size() == 1)
		return results[0];

	return pair<int, int>(-1, -1);
}

pair<int, int> tokenizer::expected(string s)
{
	for (int i = (int)expected_hierarchy.size()-1; i >= 0; i--)
	{
		vector<string>::iterator i2 = std::find(expected_hierarchy[i].first.begin(), expected_hierarchy[i].first.end(), s);
		if (i2 != expected_hierarchy[i].first.end())
			return pair<int, int>(i, i2 - expected_hierarchy[i].first.begin());
	}
	return pair<int, int>(-1, -1);
}

int tokenizer::number_expected(int max_required_level)
{
	int total = 0;
	for (int i = (int)expected_hierarchy.size()-1; i >= max_required_level; i--)
		total += (int)expected_hierarchy[i].first.size();
	return total;
}

bool tokenizer::found(string s)
{
	return (found_type == s);
}

void tokenizer::insert(string name, string contents, void *data)
{
	segment_index++;

	segments.insert(segments.begin() + segment_index, segment());
	index.insert(index.begin() + segment_index, -1);

	segments[segment_index].name = name;
	segments[segment_index].buffer = contents;
	segments[segment_index].lines.push_back(0);

	// Parse the input to provide information about line numbers.
	offset = -1;
	while (offset < (int)segments[segment_index].buffer.size()-1)
	{
		char character = peek_char(1);
		if (character == '\r')
			segments[segment_index].buffer.erase(offset+1, 1);
		else if (character == '\n')
		{
			segments[segment_index].lines.push_back(offset+2);
			next_char();
		}
		else
			next_char();
	}

	// Parse the input to provide information about tokens.
	offset = -1;
	while (offset < (int)segments[segment_index].buffer.size()-1)
	{
		char character = peek_char(1);
		if (character == '\n' || character == '\t' || character == ' ')
			next_char();
		else
		{
			vector<int> matching_comments;
			vector<token_registry_iterator> matching_tokens;
			for (int i = 0; i < (int)comment_registry.size(); i++)
				if (comment_registry[i].first(*this, 1, data))
					matching_comments.push_back(i);

			for (token_registry_iterator i = token_registry.begin(); i != token_registry.end(); i++)
				if (i->second.is_next(*this, 1, data))
					matching_tokens.push_back(i);

			if (matching_comments.size() + matching_tokens.size() > 1)
			{
				string token_set = "";
				for (int i = 0; i < (int)matching_tokens.size(); i++)
				{
					if (i != 0)
						token_set += " ";
					token_set += matching_tokens[i]->first;
				}

				token_internal("ambiguous token set " + token_set, __FILE__, __LINE__);
			}

			if (matching_comments.size() > 0)
				comment_registry[matching_comments.back()].second(*this, data);
			else if (matching_tokens.size() > 0)
				segments[segment_index].tokens.push_back(matching_tokens.back()->second.consume(*this, data));
			else
				token_error((string)"stray '" + next_char() + "'", __FILE__, __LINE__);
		}
	}
}

bool tokenizer::is_next(string str, int i)
{
	return (peek(i) == str);
}

void tokenizer::normalize_token(int &segment_index, int &token_index)
{
	while (segment_index >= 0 && segment_index < (int)segments.size() && (token_index >= (int)segments[segment_index].tokens.size() || token_index < 0))
	{
		if (token_index < 0)
		{
			segment_index--;
			if (segment_index >= 0)
				token_index += (int)segments[segment_index].tokens.size();
		}
		else
		{
			token_index -= (int)segments[segment_index].tokens.size();
			segment_index++;
		}
	}
}

void tokenizer::normalize_character(int &segment_index, int &character_index)
{
	while (segment_index >= 0 && segment_index < (int)segments.size() && (character_index >= (int)segments[segment_index].buffer.size() || character_index < 0))
	{
		if (character_index < 0)
		{
			segment_index--;
			if (segment_index >= 0)
				character_index += (int)segments[segment_index].buffer.size();
		}
		else
		{
			character_index -= (int)segments[segment_index].buffer.size();
			segment_index++;
		}
	}
}

void tokenizer::normalize(int &segment_index, int &token_index, int &character_offset)
{
	normalize_token(segment_index, token_index);
	if (segment_index >= 0 && segment_index < (int)segments.size())
	{
		int character_index = character_offset + segments[segment_index].tokens[token_index].start;
		normalize_character(segment_index, character_index);
		if (segment_index >= 0 && segment_index < (int)segments.size())
		{
			token_index = segments[segment_index].char_to_token(character_index);
			character_offset = character_index - segments[segment_index].tokens[token_index].start;
		}
	}
}

string tokenizer::file(int segment_offset, int token_offset, int character_offset)
{
	segment_offset += segment_index;
	if (segment_offset >= 0 && segment_offset < (int)segments.size())
	{
		token_offset += index[segment_offset];
		normalize(segment_offset, token_offset, character_offset);
	}

	if (segment_offset >= 0 && segment_offset < (int)segments.size())
		return segments[segment_offset].name;
	else
		return "";
}

string tokenizer::line(int segment_offset, int token_offset, int character_offset)
{
	segment_offset += segment_index;
	if (segment_offset >= 0 && segment_offset < (int)segments.size())
	{
		token_offset += index[segment_offset];
		normalize(segment_offset, token_offset, character_offset);
	}

	if (segment_offset >= 0 && segment_offset < (int)segments.size())
		return segments[segment_offset].get_line(segments[segment_offset].char_to_line(segments[segment_offset].tokens[token_offset].start + character_offset));
	else
		return "";
}

string tokenizer::absolute_location(int segment_index, int character_index, string *line, string *space)
{
	normalize_character(segment_index, character_index);

	if (segment_index >= 0 && segment_index < (int)segments.size())
	{
		if (character_index >= 0 && character_index < (int)segments[segment_index].buffer.size())
		{
			int line_index = segments[segment_index].char_to_line(character_index);
			int column_start = character_index - segments[segment_index].lines[line_index];
			string line_str;
			line_str = segments[segment_index].get_line(line_index);
			string file = segments[segment_index].name;

			int column_end = get_column_end(line_str, column_start, space);
			if (line != NULL)
				*line = line_str;

			return file + ":" + ::to_string(line_index+1) + ":" + ::to_string(column_start) + "-" + ::to_string(column_end);
		}
		else
			return "undefined";
	}
	else
		return "undefined";
}

string tokenizer::relative_location(int token_offset, int character_offset, string *line, string *space)
{
	int segment_offset = segment_index;
	if (segment_index >= 0 && segment_index < (int)segments.size())
	{
		token_offset += index[segment_offset];
		normalize(segment_offset, token_offset, character_offset);
	}

	if (segment_offset >= (int)segments.size())
	{
		segment_offset = (int)segments.size()-1;
		token_offset = (int)segments[segment_offset].tokens.size()-1;
		character_offset = segments[segment_offset].tokens[token_offset].end;
	}

	if (segment_offset >= 0 && segment_offset < (int)segments.size())
	{
		if (token_offset >= 0 && token_offset < (int)segments[segment_offset].tokens.size())
		{
			int line_index = segments[segment_offset].char_to_line(segments[segment_offset].tokens[token_offset].start + character_offset);
			int column_start = 0;
			if (line_index < (int)segments[segment_offset].lines.size())
				column_start = segments[segment_offset].tokens[token_offset].start + character_offset - segments[segment_offset].lines[line_index];

			string line_str = segments[segment_offset].get_line(line_index);
			string file = segments[segment_offset].name;

			int column_end = get_column_end(line_str, column_start, space);
			if (line != NULL)
				*line = line_str;

			return file + ":" + ::to_string(line_index+1) + ":" + ::to_string(column_start) + "-" + ::to_string(column_end);
		}
		else
			return "undefined";
	}
	else
		return "undefined";
}

int tokenizer::line_number(int segment_offset, int token_offset, int character_offset)
{
	segment_offset += segment_index;
	if (segment_offset >= 0 && segment_offset < (int)segments.size())
	{
		token_offset += index[segment_offset];
		normalize(segment_offset, token_offset, character_offset);
	}

	if (segment_offset >= 0 && segment_offset < (int)segments.size())
		return segments[segment_offset].char_to_line(segments[segment_offset].tokens[token_offset].start + character_offset);
	else
		return -1;
}

int tokenizer::line_offset(int segment_offset, int token_offset, int character_offset)
{
	segment_offset += segment_index;
	if (segment_offset >= 0 && segment_offset < (int)segments.size())
	{
		token_offset += index[segment_offset];
		normalize(segment_offset, token_offset, character_offset);
	}

	if (segment_offset >= 0 && segment_offset < (int)segments.size())
	{
		int line_index = segments[segment_offset].char_to_line(segments[segment_offset].tokens[token_offset].start + character_offset);
		return segments[segment_offset].tokens[token_offset].start + character_offset - segments[segment_offset].lines[line_index];
	}
	else
		return -1;
}

int tokenizer::character_offset(int segment_offset, int token_offset, int character_offset)
{
	segment_offset += segment_index;
	if (segment_offset >= 0 && segment_offset < (int)segments.size())
	{
		token_offset += index[segment_offset];
		normalize(segment_offset, token_offset, character_offset);
	}
	return character_offset;
}

char tokenizer::curr_char()
{
	if (segment_index >= 0 && segment_index < (int)segments.size() && offset >= 0 && offset < (int)segments[segment_index].buffer.size())
		return segments[segment_index].buffer[offset];
	else
		return '\0';
}

char tokenizer::prev_char()
{
	offset--;

	if (segment_index >= 0 && segment_index < (int)segments.size() && offset >= 0 && offset < (int)segments[segment_index].buffer.size())
		return segments[segment_index].buffer[offset];
	else
		return '\0';
}

char tokenizer::next_char()
{
	offset++;

	if (segment_index >= 0 && segment_index < (int)segments.size() && offset >= 0 && offset < (int)segments[segment_index].buffer.size())
		return segments[segment_index].buffer[offset];
	else
		return '\0';
}

char tokenizer::peek_char(int i)
{
	int temp_offset = offset+i;
	if (segment_index >= 0 && segment_index < (int)segments.size() && temp_offset >= 0 && temp_offset < (int)segments[segment_index].buffer.size())
		return segments[segment_index].buffer[temp_offset];
	else
		return '\0';
}

string tokenizer::curr_type()
{
	if (segment_index >= 0 && segment_index < (int)segments.size() && index[segment_index] >= 0 && index[segment_index] < (int)segments[segment_index].tokens.size())
		for (token_registry_iterator i = token_registry.begin(); i != token_registry.end(); i++)
			if (i->second.type == segments[segment_index].tokens[index[segment_index]].type)
				return i->first;

	return "";
}

string tokenizer::curr()
{
	if (segment_index >= 0 && segment_index < (int)segments.size() && index[segment_index] >= 0 && index[segment_index] < (int)segments[segment_index].tokens.size())
		return segments[segment_index].get_token(index[segment_index]);
	else
		return "";
}

string tokenizer::prev()
{
	int remainder = 1;
	while (segment_index >= (int)segments.size() && index[segment_index] - remainder < 0)
	{
		remainder -= index[segment_index];
		index[segment_index] = 0;
		segment_index--;
	}

	if (segment_index < (int)segments.size())
	{
		index[segment_index] -= remainder;
		return segments[segment_index].get_token(index[segment_index]);
	}
	else
		return "";
}

string tokenizer::next()
{
	int remainder = 1;
	while (segment_index < (int)segments.size() && index[segment_index] + remainder >= (int)segments[segment_index].tokens.size())
	{
		remainder -= segments[segment_index].tokens.size() - index[segment_index];
		index[segment_index] = segments[segment_index].tokens.size();
		segment_index++;
	}

	if (segment_index < (int)segments.size())
	{
		index[segment_index] += remainder;
		return segments[segment_index].get_token(index[segment_index]);
	}
	else
		return "";
}

string tokenizer::peek_type(int i)
{
	int temp_index = segment_index;
	int inc = (i >= 0 ? 1 : -1);
	while (temp_index < (int)segments.size() && temp_index >= 0 && (index[temp_index] + i >= (int)segments[temp_index].tokens.size() || index[temp_index]+i < 0))
	{
		i -= inc ? segments[temp_index].tokens.size() - index[temp_index] : -index[temp_index];
		temp_index += inc;
	}

	if (temp_index >= 0 && temp_index < (int)segments.size())
		for (token_registry_iterator j = token_registry.begin(); j != token_registry.end(); j++)
			if (j->second.type == segments[temp_index].tokens[index[temp_index] + i].type)
				return j->first;

	return "";
}

string tokenizer::peek(int i)
{
	int temp_index = segment_index;
	int inc = (i >= 0 ? 1 : -1);
	while (temp_index < (int)segments.size() && temp_index >= 0 && (index[temp_index] + i >= (int)segments[temp_index].tokens.size() || index[temp_index]+i < 0))
	{
		i -= inc ? segments[temp_index].tokens.size() - index[temp_index] : -index[temp_index];
		temp_index += inc;
	}

	if (temp_index >= 0 && temp_index < (int)segments.size())
		return segments[temp_index].get_token(index[temp_index] + i);
	else
		return "";
}

bool tokenizer::segment_loaded(string name)
{
	for (int i = segment_index+1; i < (int)segments.size(); i++)
		if (segments[i].name == name)
			return true;

	return false;
}

bool tokenizer::segment_loading(string name)
{
	for (int i = 0; i <= segment_index; i++)
		if (segments[i].name == name)
			return true;

	return false;
}

void tokenizer::reset()
{
	segments.clear();
	index.clear();
	segment_index = -1;
	offset = -1;
	stack.clear();
	expected_hierarchy.clear();
	bookmarks.clear();
	num_errors = 0;
	num_warnings = 0;
	num_internal = 0;
	num_notes = 0;
	num_log = 0;
}
