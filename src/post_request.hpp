#pragma once

#include "get_request.hpp"

class PostRequest: public GetRequest {
public:
	PostRequest(const std::string&, std::string&, size_t = 0);
	PostRequest(const std::string&);
	PostRequest() = default;

	void set_postdata(std::string&);
	void set_postdata(std::string&, size_t);
};
