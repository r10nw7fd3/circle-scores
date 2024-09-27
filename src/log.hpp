#pragma once

#include <iostream>

extern char g_log_time_buf[];

void log_update_time_buf();

#define LOGI std::cerr << "I " << (log_update_time_buf(), g_log_time_buf)
#define LOGW std::cerr << "W " << (log_update_time_buf(), g_log_time_buf)
#define LOGE std::cerr << "E " << (log_update_time_buf(), g_log_time_buf)
