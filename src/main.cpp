#include <iostream>
#include <cstdlib>
#include "args.hpp"
#include "credentials.hpp"
#include "processor.hpp"
#include "sig_handler.hpp"
#include "discord_score_receiver.hpp"
#include "stdout_score_receiver.hpp"

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

	StdoutScoreReceiver stdout_recvr;
	prc.register_receiver(stdout_recvr);

	DiscordScoreReceiver hook(creds.get_discord_hook_url());
	if(!creds.get_discord_hook_url().empty()) {
		prc.register_receiver(hook);
	}

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
