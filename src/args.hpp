#pragma once

#include <string>

class Args {
private:
	int delay_ = 300; // 5 mins
	int page_ = 0;
	std::string creds_filename_ = "credentials.txt";
	int discord_replays_ = 1;
	int lower_pp_bound_ = 800;

	static int read_int(char*);
	static void check_next(const char*, int&, int);
public:
	int get_delay() const;
	int get_page() const;
	const std::string& get_creds_filename() const;
	int get_discord_replays() const;
	int get_lower_pp_bound() const;

	void parse(int, char**);
};
