#pragma once

#include "post_request.hpp"
#include "get_request.hpp"

class DiscordHook {
private:
	std::string url_;
	PostRequest post_;
	GetRequest get_;
public:
	DiscordHook(const std::string&);
	long long post(const std::string&, const std::string&, const std::string&);
};
