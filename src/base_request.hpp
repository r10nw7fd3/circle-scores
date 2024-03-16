#pragma once

#include <curl/curl.h>
#include <string>

class BaseRequest {
private:
	CURLcode curl_last_;
	struct curl_slist* curl_headers_ = nullptr;

	static size_t write_callback(char*, size_t, size_t, void*);

protected:
	CURL* curl_;

	virtual void init();

public:
	BaseRequest();
	BaseRequest(const std::string&);
	virtual ~BaseRequest();

	void set_endpoint(const std::string&);
	int add_header(const std::string&);
	long long perform(std::string&);
	std::string last_err() const;
	CURL* get_curl() { return curl_; }
};
