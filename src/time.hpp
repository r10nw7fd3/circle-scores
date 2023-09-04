#pragma once

#include <ctime>

time_t parse_iso8601(char*);
unsigned long long unix_to_winticks(time_t);
unsigned long long get_unix_ms();
