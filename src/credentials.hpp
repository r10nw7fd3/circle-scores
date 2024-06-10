#pragma once

#include <string>

class Credentials {
private:
	std::string filename_;
	std::string osu_id_;
	std::string osu_key_;
	std::string discord_hook_url_;

	static void fnotpresent(const std::string&);
	static void fnotpresentbutnotnes(const std::string&);
public:
	Credentials(const std::string&);

	int read();

	const std::string& get_filename() const;
	const std::string& get_osu_id() const;
	const std::string& get_osu_key() const;
	const std::string& get_discord_hook_url() const;
};

