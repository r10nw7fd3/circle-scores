#include <iostream>
#include <memory>
#include "args.hpp"
#include "config.hpp"
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
	
	Config config(args.get_config_filename());
	if(!config.is_valid()) {
		std::cerr << "Failed to read config" << std::endl;
		return 1;
	}

	Credentials creds(args.get_creds_filename());
	if(creds.read()) {
		std::cerr << "Failed to read credentials file" << std::endl;
		return 1;
	}

	curl_global_init(CURL_GLOBAL_ALL);

	Processor prc(config, creds);

	StdoutScoreReceiver stdout_recvr;
	prc.register_receiver(stdout_recvr);

#ifdef ENABLE_DISCORD_HOOK
	std::unique_ptr<DiscordScoreReceiver> discord_recvr;
	if(config.get_discord_hook_enabled()) {
		discord_recvr.reset(new DiscordScoreReceiver(creds.get_discord_hook_url()));
		prc.register_receiver(*discord_recvr);
	}
#endif

#ifdef ENABLE_LAMS
	std::unique_ptr<LamsScoreReceiver> lams_recvr;
	if(config.get_lams_enabled()) {
		lams_recvr.reset(new LamsScoreReceiver(config.get_lams_address(), config.get_lams_dir()));
		prc.register_receiver(*lams_recvr);
	}
#endif

	if(config.get_catch_sigint())
		register_handler([&prc] (int code) {
			(void) code;
			prc.get_token().revoke();
			std::exit(0);
		});

	prc.run();

	curl_global_cleanup();
	return 0;
}
