#pragma once

#include <string>

class Credentials {
private:
	std::string filename_;
	std::string osu_id_;
	std::string osu_key_;
	std::string osu_legacy_key_;
	std::string discord_hook_url_;

	void fnotpresent(const std::string&);
	void fnotpresentbutnotnes(const std::string&);
public:
	Credentials(std::string);

	int read();

	const std::string& get_filename() const;
	const std::string& get_osu_id() const;
	const std::string& get_osu_key() const;
	const std::string& get_osu_legacy_key() const;
	const std::string& get_discord_hook_url() const;
};

