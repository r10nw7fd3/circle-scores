#include "args.hpp"

#include <sstream>
#include <cstring>
#include "log.hpp"

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
	std::cerr << "-conf <fname>     Config filename. Default = config.json\n";
	std::cerr << "-creds <fname>    Credentials filename. Default = credentials.json\n";

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
		else if(!std::strcmp(argv[i], "-conf")) {
			check_next("-conf", i, argc);
			config_filename_ = argv[i];
		}
		else if(!std::strcmp(argv[i], "-creds")) {
			check_next("-creds", i, argc);
			creds_filename_ = argv[i];
		}
		else {
			std::cerr << "Unknown argument \"" << argv[i] << "\"" << std::endl;
			std::exit(1);
		}
	}
}
