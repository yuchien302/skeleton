/*
 * message.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: nbingham
 */

#include "message.h"

int num_errors = 0;
int num_warnings = 0;
int num_internal = 0;
int num_notes = 0;
int num_log = 0;
bool verbose = true;
bool debug = true;

int progress_length = 0;

void internal(string location, string internal, string debug_file, int debug_line)
{
	if (location != "")
		printf("%s:%d:%s: internal failure: %s\n", debug_file.c_str(), debug_line, location.c_str(), internal.c_str());
	else
		printf("%s:%d: internal failure: %s\n", debug_file.c_str(), debug_line, internal.c_str());
	num_internal++;
	exit(0);
}

void error(string location, string error, string debug_file, int debug_line)
{
	if (debug && location != "")
		printf("%s:%d:%s: error: %s\n", debug_file.c_str(), debug_line, location.c_str(), error.c_str());
	else if (debug && location == "")
		printf("%s:%d: error: %s\n", debug_file.c_str(), debug_line, error.c_str());
	else if (!debug && location != "")
		printf("%s: error: %s\n", location.c_str(), error.c_str());
	else if (!debug && location == "")
		printf("error: %s\n", error.c_str());
	num_errors++;
}

void warning(string location, string warning, string debug_file, int debug_line)
{
	if (debug && location != "")
		printf("%s:%d:%s: warning: %s\n", debug_file.c_str(), debug_line, location.c_str(), warning.c_str());
	else if (debug && location == "")
		printf("%s:%d: warning: %s\n", debug_file.c_str(), debug_line, warning.c_str());
	else if (!debug && location != "")
		printf("%s: warning: %s\n", location.c_str(), warning.c_str());
	else if (!debug && location == "")
		printf("warning: %s\n", warning.c_str());
	num_warnings++;
}

void note(string location, string note, string debug_file, int debug_line)
{
	if (debug && location != "")
		printf("%s:%d:%s: note: %s\n", debug_file.c_str(), debug_line, location.c_str(), note.c_str());
	else if (debug && location == "")
		printf("%s:%d: note: %s\n", debug_file.c_str(), debug_line, note.c_str());
	else if (!debug && location != "")
		printf("%s: note: %s\n", location.c_str(), note.c_str());
	else if (!debug && location == "")
		printf("note: %s\n", note.c_str());
	num_notes++;
}

void log(string location, string log, string debug_file, int debug_line)
{
	if (verbose)
	{
		if (debug && location != "")
			printf("%s:%d:%s: log: %s\n", debug_file.c_str(), debug_line, location.c_str(), log.c_str());
		else if (debug && location == "")
			printf("%s:%d: log: %s\n", debug_file.c_str(), debug_line, log.c_str());
		else if (!debug && location != "")
			printf("%s: log: %s\n", location.c_str(), log.c_str());
		else if (!debug && location == "")
			printf("log: %s\n", log.c_str());
		num_log++;
	}
}

void progress(string location, string log, string debug_file, int debug_line)
{
	char str[1024];
	if (debug && location != "")
		sprintf(str, "%s:%d:%s: %s", debug_file.c_str(), debug_line, location.c_str(), log.c_str());
	else if (debug && location == "")
		sprintf(str, "%s:%d: %s", debug_file.c_str(), debug_line, log.c_str());
	else if (!debug && location != "")
		sprintf(str, "%s: %s", location.c_str(), log.c_str());
	else if (!debug && location == "")
		sprintf(str, "%s", log.c_str());
	int length = strlen(str);
	if (progress_length > length)
		printf("%s%s\r", str, string(progress_length - length, ' ').c_str());
	else
		printf("%s\r", str);

	progress_length = length+1;
	fflush(stdout);
}

void message(string message, string debug_file, int debug_line)
{
	if (debug)
		printf("%s:%d: %s\n", debug_file.c_str(), debug_line, message.c_str());
	else
		printf("%s\n", message.c_str());
}

void done_progress()
{
	printf("%s\r", string(progress_length, ' ').c_str());
	progress_length = 0;
	fflush(stdout);
}

void complete()
{
	if (num_internal > 0)
		printf("completed with %d internal failures, %d errors, and %d warnings\n", num_internal, num_errors, num_warnings);
	else
		printf("completed with %d errors, and %d warnings\n", num_errors, num_warnings);
}

bool is_clean()
{
	return (num_internal == 0 && num_errors == 0);
}

void flag_errors(int num)
{
	num_errors += num;
}

void flag_warnings(int num)
{
	num_warnings += num;
}

void flag_internal(int num)
{
	num_internal += num;
}

void flag_notes(int num)
{
	num_notes += num;
}

void flag_log(int num)
{
	num_log += num;
}

void set_debug(bool value)
{
	debug = value;
}

bool get_debug()
{
	return debug;
}

void set_verbose(bool value)
{
	verbose = value;
}

bool get_verbose()
{
	return verbose;
}
