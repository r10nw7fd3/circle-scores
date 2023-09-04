#pragma once

#include "get_request.hpp"

class PostRequest: public GetRequest {
public:
	PostRequest(const std::string&, std::string&, size_t);
	PostRequest(const std::string&, std::string&);
	PostRequest(const std::string&);
	PostRequest();

	void set_postdata(std::string&);
	void set_postdata(std::string&, size_t);
};
