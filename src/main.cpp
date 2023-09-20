#include <iostream>
#include <cstdlib>
#include "args.hpp"
#include "credentials.hpp"
#include "processor.hpp"
#include "sig_handler.hpp"

static Processor* prc = nullptr;

static void handler(int sig) {
	(void) sig;

	if(prc)
		prc->get_token().revoke();
	std::exit(0);
}

int main(int argc, char** argv) {
	Args args;
	args.parse(argc, argv);
	Credentials creds(args.get_creds_filename());
	if(creds.read()) {
		std::cout << "Failed to open credentials file" << std::endl;
		return 1;
	}

	if(args.get_catch_sig())
		register_handler(handler);

	curl_global_init(CURL_GLOBAL_ALL);

	prc = new Processor(args, creds);
	prc->run();
	delete prc;

	curl_global_cleanup();
	return 0;
}
