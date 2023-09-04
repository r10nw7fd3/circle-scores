#pragma once

#include <curl/curl.h>
#include <string>

class GetRequest {
private:
	CURLcode curl_last_;
	struct curl_slist* curl_headers_ = nullptr;

	static size_t write_callback(char*, size_t, size_t, void*);
	void init();
public:
	GetRequest();
	GetRequest(const std::string&);
	~GetRequest();

	void set_endpoint(const std::string&);
	int add_header(std::string);
	long long perform(std::string&);
	std::string last_err() const;
	CURL* get_curl() const;
protected:
	CURL* curl_;
};
