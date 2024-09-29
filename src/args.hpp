#pragma once

#include <string>

class Args {
private:
	std::string config_filename_ = "config.json";
	std::string creds_filename_ = "credentials.json";

	static void check_next(const char*, int&, int);
	static void print_help_and_exit(const char*);

public:
	const std::string& get_config_filename() const { return config_filename_; }
	const std::string& get_creds_filename() const { return creds_filename_; }

	void parse(int, char**);
};
