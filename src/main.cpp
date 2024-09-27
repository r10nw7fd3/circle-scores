#include <iostream>
#include <cstdlib>
#include "args.hpp"
#include "credentials.hpp"
#include "processor.hpp"
#include "sig_handler.hpp"
#include "stdout_score_receiver.hpp"
#ifdef ENABLE_DISCORD_HOOK
#include "discord_score_receiver.hpp"
#endif
#ifdef ENABLE_LAMS
#include "lams_score_receiver.hpp"
#endif

int main(int argc, char** argv) {
	Args args;
	args.parse(argc, argv);
	Credentials creds(args.get_creds_filename());
	if(creds.read()) {
		std::cerr << "Failed to read credentials file" << std::endl;
		return 1;
	}

	curl_global_init(CURL_GLOBAL_ALL);

	Processor prc(args, creds);

	StdoutScoreReceiver stdout_recvr;
	prc.register_receiver(stdout_recvr);

#ifdef ENABLE_DISCORD_HOOK
	DiscordScoreReceiver hook(creds.get_discord_hook_url());
	if(!creds.get_discord_hook_url().empty())
		prc.register_receiver(hook);
#endif

#ifdef ENABLE_LAMS
	LamsScoreReceiver lams_recvr(args.get_lams(), args.get_lams_dir());
	if(!args.get_lams().empty())
		prc.register_receiver(lams_recvr);
#endif

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
