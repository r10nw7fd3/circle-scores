#include <time.h>

// Time string, space, zero byte
char g_log_time_buf[19 + 1 + 1];

void log_update_time_buf() {
	time_t t = time(nullptr);
	struct tm* tim = localtime(&t);
	if(!strftime(g_log_time_buf, sizeof(g_log_time_buf), "%F %H:%M:%S ", tim)) {
		g_log_time_buf[0] = '?';
		g_log_time_buf[1] = 0;
	}
}
