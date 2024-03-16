#pragma once

#include "base_request.hpp"

class PostRequest: public BaseRequest {
protected:
	virtual void init() override;

public:
	using BaseRequest::BaseRequest;
	PostRequest(const std::string& endpoint, std::string& postdata, size_t size = 0);

	void set_postdata(std::string&);
	void set_postdata(std::string&, size_t);
};
