#pragma once

#include <string>
#include <vector>

class Config {
public:
	Config(const std::string& filename);

	bool is_valid() const { return valid_; }

	int get_interval() const { return interval_; }
	int get_page() const { return page_; }
	int get_lower_pp_bound() const { return lower_pp_bound_; }
	bool get_catch_sigint() const { return catch_sigint_; }
	bool get_save_token_to_file() const { return save_token_to_file_; }
	const std::string& get_token_filename() const { return token_filename_; }
	const std::vector<int>& get_excluded_ids() const { return excluded_ids_; }
	bool get_discord_hook_enabled() const { return discord_hook_enabled_; }
	bool get_telegram_bot_enabled() const { return telegram_bot_enabled_; }
	const std::string& get_telegram_bot_channel() const { return telegram_bot_channel_; }
	bool get_lams_enabled() const { return lams_enabled_; }
	const std::string& get_lams_address() const { return lams_address_; }
	const std::string& get_lams_dir() const { return lams_dir_; }

private:
	bool valid_ = false;

	int interval_;
	int page_;
	int lower_pp_bound_;
	bool catch_sigint_;
	bool save_token_to_file_;
	std::string token_filename_;
	std::vector<int> excluded_ids_;
	bool discord_hook_enabled_;
	bool telegram_bot_enabled_;
	std::string telegram_bot_channel_;
	bool lams_enabled_;
	std::string lams_address_;
	std::string lams_dir_;
};
