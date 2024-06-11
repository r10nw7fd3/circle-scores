#pragma once

#include "base_request.hpp"

class PostRequest: public BaseRequest {
private:
	curl_mime* mime_ = nullptr;
protected:
	virtual void init() override;

public:
	using BaseRequest::BaseRequest;
	PostRequest(const std::string& endpoint, const std::string& postdata, size_t size = 0);
	virtual ~PostRequest() override;

	void set_postdata(const std::string&);
	void set_postdata(const std::string&, size_t);

	void add_mimepart(const std::string& name, const std::string& data,
		const std::string& filename, const std::string& type);
	void clear_mime();
};
