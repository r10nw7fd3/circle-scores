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
			std::cout << "-h              Display help message" << std::endl;
			std::cout << "-d <delay>      Delay in seconds between scans. Default = 300" << std::endl;
			std::cout << "-p <page>       Leaderboard page to scan. Default = 0" << std::endl;
			std::cout << "-c <fname>      Credentials filename. Default = credentials.json" << std::endl;
			std::cout << "-pp <pp>        Lower PP bound. Default = 800" << std::endl;
			std::cout << "-no-sig         Do not catch Ctrl+C/SIGINT/SIGTERM to revoke token" << std::endl;
			std::cout << "-tf <fname>     Save token to a file" << std::endl;
			std::cout << "-lams <addr>    Address of the Look At My Score!-compatible service to download score images from" << std::endl;
			std::cout << "-lams-dir <dir> Save score images into <dir>. Default = score-images" << std::endl;
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
		else if(!std::strcmp(argv[i], "-pp")) {
			check_next("-pp", i, argc);
			lower_pp_bound_ = read_int(argv[i]);
		}
		else if(!std::strcmp(argv[i], "-no-sig")) {
			catch_sig_ = 0;
		}
		else if(!std::strcmp(argv[i], "-tf")) {
			check_next("-tf", i, argc);
			token_filename_ = argv[i];
		}
		else if(!std::strcmp(argv[i], "-lams")) {
			check_next("-lams", i, argc);
			lams_ = argv[i];
		}
		else if(!std::strcmp(argv[i], "-lams-dir")) {
			check_next("-lams-dir", i, argc);
			lams_dir_ = argv[i];
		}
		else {
			std::cout << "Unknown argument \"" << argv[i] << "\"" << std::endl;
			std::exit(1);
		}
	}
}
