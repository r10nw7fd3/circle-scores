#include "time.hpp"
#include <cstdio>
#include <cstring>
#include <chrono>

// This function, despite the name, only parses ISO8601 strings
// in the following format: yyyy-mm-ddThh:mm:ssZ.
// Suitable for osu! api, but not flexible.
// A better way would be to use std::chrono::parse from C++20, but
// I'm limiting myself to C++11.
// https://en.cppreference.com/w/c/chrono/tm
// https://stackoverflow.com/a/61541168/19823424
time_t parse_iso8601(char* time) {
	const char* fmt = "%d-%d-%dT%d:%d:%dZ";
	struct tm t;
	memset(&t, 0, sizeof(struct tm));
	int count = sscanf(time, fmt, &t.tm_year, &t.tm_mon, &t.tm_mday,
		&t.tm_hour, &t.tm_min, &t.tm_sec
	);
	
	if(count != 6)
		return 0;

	t.tm_year -= 1900;
	t.tm_mon -= 1;

	return (mktime(&t) - timezone);
}

unsigned long long unix_to_winticks(time_t time) {
	return (10000000 * time) + 621355968000000000;
}

unsigned long long get_unix_ms() {
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
