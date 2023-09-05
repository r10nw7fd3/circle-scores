#include "args.hpp"

#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include "log.hpp"

int Args::read_int(char* str) {
	int res = -1;
	try {
		res = std::stoi(str);
	}
	catch(const std::exception& e) {
		std::cout << LOGE << e.what() << std::endl;
		std::exit(1);
	}
	return res;
}

void Args::check_next(const char* arg, int& i, int argc) {
	if(++i >= argc) {
		std::cout << "Argument \"" << arg << "\" requires value" << std::endl;
		std::exit(1);
	}
}

void Args::parse(int argc, char** argv) {
	for(int i = 1; i < argc; ++i) {
		if(!std::strcmp(argv[i], "-h") || !std::strcmp(argv[i], "-help") || !std::strcmp(argv[i], "--help")) {
			std::cout << "Usage: " << argv[0] << " [args]" << std::endl << std::endl;
			std::cout << "Args:" << std::endl;
			std::cout << "-h         Display help message" << std::endl;
			std::cout << "-d <delay> Delay in seconds between scans. Default = 300" << std::endl;
			std::cout << "-p <page>  Leaderboard page to scan. Default = 0" << std::endl;
			std::cout << "-c <fname> Credentials filename. Default = credentials.txt" << std::endl;
			std::cout << "-no-dr     Do not post replays to discord" << std::endl;
			std::exit(0);
		}
		if(!std::strcmp(argv[i], "-d")) {
			check_next("-d", i, argc);
			delay_ = read_int(argv[i]);
		}
		else if(!std::strcmp(argv[i], "-p")) {
			check_next("-p", i, argc);
			page_ = read_int(argv[i]);
		}
		else if(!std::strcmp(argv[i], "-c")) {
			check_next("-c", i, argc);
			creds_filename_ = argv[i];
		}
		else if(!std::strcmp(argv[i], "-no-dr")) {
			discord_replays_ = 0;
		}
		else {
			std::cout << "Unknown argument \"" << argv[i] << "\"" << std::endl;
			std::exit(1);
		}
	}
}

int Args::get_delay() const {
	return delay_;
}

int Args::get_page() const {
	return page_;
}

const std::string& Args::get_creds_filename() const {
	return creds_filename_;
}

int Args::get_discord_replays() const {
	return discord_replays_;
}
