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
	std::string lams_;
	std::string lams_dir_ = "score-images";

	static int read_int(char*);
	static void check_next(const char*, int&, int);

public:
	int get_delay() const { return delay_; }
	int get_page() const { return page_; }
	const std::string& get_creds_filename() const { return creds_filename_; }
	int get_lower_pp_bound() const { return lower_pp_bound_; }
	int get_catch_sig() const { return catch_sig_; }
	const std::string& get_token_filename() const { return token_filename_; }
	const std::string& get_lams() const { return lams_; }
	const std::string& get_lams_dir() const { return lams_dir_; }

	void parse(int, char**);
};
