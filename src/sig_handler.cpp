#include <functional>
#include "log.hpp"

static std::function<void(int)> global_callback;

#if defined(__unix__) || defined(__unix) // Let's assume unix implies that posix is available
#include <signal.h>

static void handler(int code) {
	global_callback(code);
}

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>

static BOOL WINAPI handler(DWORD ctrl) {
	if(ctrl == CTRL_C_EVENT) {
		global_callback(-1);
		return TRUE;
	}
	return FALSE;
}
#else
#warning "WTF is your platform!? Signal handling will be unavailable"
#endif

int register_handler(const std::function<void(int)>& callback) {
	global_callback = callback;
#if defined(__unix__) || defined(__unix)
	struct sigaction act;
	act.sa_handler = handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	int ret = sigaction(SIGINT, &act, nullptr);
	ret |= sigaction(SIGTERM, &act, nullptr);
	return ret;
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
	return !SetConsoleCtrlHandler(handler, TRUE);
#else
	LOGE << "Unknown platform" << std::endl;
	return 1;
#endif
}
