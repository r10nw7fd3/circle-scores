#include <iostream>
#include "args.hpp"
#include "credentials.hpp"
#include "processor.hpp"

int main(int argc, char** argv) {
	Args args;
	args.parse(argc, argv);
	Credentials creds(args.get_creds_filename());
	if(creds.read()) {
		std::cout << "Failed to open credentials file" << std::endl;
		return 1;
	}

	curl_global_init(CURL_GLOBAL_ALL);

	Processor prc(args.get_delay(), args.get_page(), creds);
	prc.run();

	curl_global_cleanup();
	return 0;
}
