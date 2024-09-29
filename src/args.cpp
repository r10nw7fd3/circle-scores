#include "args.hpp"

#include <sstream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include "log.hpp"

int Args::read_int(const char* str) {
	int res = -1;
	try {
		res = std::stoi(str);
	}
	catch(const std::exception& e) {
		std::cerr << "Invalid argument value" << std::endl;
		std::exit(1);
	}
	return res;
}

void Args::check_next(const char* arg, int& i, int argc) {
	if(++i >= argc) {
		std::cerr << "Argument \"" << arg << "\" requires value" << std::endl;
		std::exit(1);
	}
}

void Args::print_help_and_exit(const char* program) {
	std::cerr << "Usage: " << program << " [args]\n\n";
	std::cerr << "Args:\n";
	std::cerr << "-h                Display help message\n";
	std::cerr << "-d <delay>        Delay in seconds between scans. Default = 300\n";
	std::cerr << "-p <page>         Leaderboard page to scan. Default = 0\n";
	std::cerr << "-c <fname>        Credentials filename. Default = credentials.json\n";
	std::cerr << "-pp <pp>          Lower PP bound. Default = 800\n";
	std::cerr << "-no-sig           Do not catch Ctrl+C/SIGINT/SIGTERM to revoke token\n";
	std::cerr << "-tf <fname>       Save token to a file\n";
	std::cerr << "-ex <id1,id2,...> Exclude ids from being scanned\n";
#ifdef ENABLE_LAMS
	std::cerr << "-lams <addr>      Address of the Look At My Score!-compatible service to download score images from\n";
	std::cerr << "-lams-dir <dir>   Save score images into <dir>. Default = score-images\n";
#endif

	std::cerr << "\nBuild config: ";
	std::cerr << "ENABLE_DISCORD_HOOK := " <<
#ifdef ENABLE_DISCORD_HOOK
	"1 ";
#else
	"0 ";
#endif
	std::cerr << "ENABLE_LAMS := " <<
#ifdef ENABLE_LAMS
	"1 ";
#else
	"0 ";
#endif

	std::cerr << std::endl;
	std::exit(0);
}

void Args::parse(int argc, char** argv) {
	for(int i = 1; i < argc; ++i) {
		if(!std::strcmp(argv[i], "-h") || !std::strcmp(argv[i], "-help") || !std::strcmp(argv[i], "--help")) {
			print_help_and_exit(argv[0]);
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
		else if(!std::strcmp(argv[i], "-ex")) {
			check_next("-ex", i, argc);
			std::stringstream ss(argv[i]);
			while(ss) {
				std::string id;
				std::getline(ss, id, ',');
				if(id.empty())
					continue;
				exclude_.push_back(read_int(id.c_str()));
			}
		}
#ifdef ENABLE_LAMS
		else if(!std::strcmp(argv[i], "-lams")) {
			check_next("-lams", i, argc);
			lams_ = argv[i];
		}
		else if(!std::strcmp(argv[i], "-lams-dir")) {
			check_next("-lams-dir", i, argc);
			lams_dir_ = argv[i];
		}
#endif
		else {
			std::cerr << "Unknown argument \"" << argv[i] << "\"" << std::endl;
			std::exit(1);
		}
	}
}
