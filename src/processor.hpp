#pragma once

#include <unordered_map>
#include <ctime>
#include "args.hpp"
#include "credentials.hpp"
#include "token.hpp"
#include "score.hpp"
#include "discord_hook.hpp"

class Processor {
private:
	const Args& args_;
	const Credentials& creds_;
	osu::requests::Token token_;
	std::unordered_map<int, time_t> last_update_;
	DiscordHook discord_;

	static void to_text(const osu::Score&, std::string&);
	time_t get_last_update(int);
	time_t set_last_update(int);
	void download_replay(const osu::Score&, std::string&);
	void post_discord(const osu::Score&);
	void query();
public:
	Processor(const Args&, const Credentials&);

	int run();
	const osu::requests::Token& get_token() const;
};
