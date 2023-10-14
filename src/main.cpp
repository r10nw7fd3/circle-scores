#include <iostream>
#include <cstdlib>
#include "args.hpp"
#include "credentials.hpp"
#include "processor.hpp"
#include "sig_handler.hpp"

int main(int argc, char** argv) {
	Args args;
	args.parse(argc, argv);
	Credentials creds(args.get_creds_filename());
	if(creds.read()) {
		std::cout << "Failed to read credentials file" << std::endl;
		return 1;
	}

	curl_global_init(CURL_GLOBAL_ALL);

	Processor prc(args, creds);
	if(args.get_catch_sig())
		register_handler([&prc] (int code) {
			(void) code;
			prc.get_token().revoke();
			std::exit(0);
		});

	prc.run();

	curl_global_cleanup();
	return 0;
}
