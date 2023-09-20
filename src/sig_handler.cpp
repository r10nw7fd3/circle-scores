#include "log.hpp"

#if defined(__unix__) || defined(__unix) // Let's assume unix implies that posix is available
#include <signal.h>
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>

static void (*global_callback)(int);

static BOOL WINAPI handler(DWORD ctrl) {
	if(ctrl == CTRL_C_EVENT) {
		(*global_callback)(-1);
		return TRUE;
	}
	return FALSE;
}
#else
#warning "WTF is your platform!? Signal handling will be unavailable"
#endif

int register_handler(void (*callback)(int)) {
#if defined(__unix__) || defined(__unix)
	struct sigaction act;
	act.sa_handler = callback;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	int ret = sigaction(SIGINT, &act, nullptr);
	ret |= sigaction(SIGTERM, &act, nullptr);
	return ret;
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
	global_callback = callback;
	return !SetConsoleCtrlHandler(handler, TRUE);
#else
	std::cout << LOGE"Unknown platform" << std::endl;
	return 1;
#endif
}
