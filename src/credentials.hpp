#pragma once

#include <string>

class Credentials {
private:
	std::string filename_;
	std::string osu_id_;
	std::string osu_key_;
#ifdef ENABLE_DISCORD_HOOK
	std::string discord_hook_url_;
#endif
#ifdef ENABLE_TELEGRAM_BOT
	std::string telegram_bot_token_;
#endif

	static void fnotpresent(const std::string&);
	static void fnotpresentbutnotnes(const std::string&);
public:
	Credentials(const std::string&);

	int read();

	const std::string& get_filename() const { return filename_; }
	const std::string& get_osu_id() const { return osu_id_; }
	const std::string& get_osu_key() const { return osu_key_; }
#ifdef ENABLE_DISCORD_HOOK
	const std::string& get_discord_hook_url() const { return discord_hook_url_; }
#endif
#ifdef ENABLE_TELEGRAM_BOT
	const std::string& get_telegram_bot_token() const { return telegram_bot_token_; }
#endif
};

