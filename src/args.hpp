#pragma once

#include <string>

class Args {
private:
	int delay_ = 300; // 5 mins
	int page_ = 0;
	std::string creds_filename_ = "credentials.json";
	int lower_pp_bound_ = 800;
	int catch_sig_ = 1;
	std::string token_filename_;

	static int read_int(char*);
	static void check_next(const char*, int&, int);
public:
	int get_delay() const;
	int get_page() const;
	const std::string& get_creds_filename() const;
	int get_lower_pp_bound() const;
	int get_catch_sig() const;
	const std::string& get_token_filename() const;

	void parse(int, char**);
};
