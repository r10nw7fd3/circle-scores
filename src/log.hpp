#pragma once

#include <iostream>

extern char g_log_time_buf[];

void log_update_time_buf();

#define LOGI std::cout << "I " << (log_update_time_buf(), g_log_time_buf)
#define LOGW std::cout << "W " << (log_update_time_buf(), g_log_time_buf)
#define LOGE std::cout << "E " << (log_update_time_buf(), g_log_time_buf)
