#pragma once

#include <string>

class Args {
private:
	int delay_ = 300; // 5 mins
	int page_ = 0;
	std::string creds_filename_ = "credentials.txt";

	static int read_int(char*);
	static void check_next(const char*, int&, int);
public:
	int get_delay() const;
	int get_page() const;
	const std::string& get_creds_filename() const;

	void parse(int, char**);
};