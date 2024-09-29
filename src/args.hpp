#pragma once

#include <string>
#include <vector>

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
	std::vector<int> exclude_;

	static int read_int(const char*);
	static void check_next(const char*, int&, int);
	static void print_help_and_exit(const char*);

public:
	int get_delay() const { return delay_; }
	int get_page() const { return page_; }
	const std::string& get_creds_filename() const { return creds_filename_; }
	int get_lower_pp_bound() const { return lower_pp_bound_; }
	int get_catch_sig() const { return catch_sig_; }
	const std::string& get_token_filename() const { return token_filename_; }
	const std::string& get_lams() const { return lams_; }
	const std::string& get_lams_dir() const { return lams_dir_; }
	const std::vector<int>& get_exclude() const { return exclude_; }

	void parse(int, char**);
};
