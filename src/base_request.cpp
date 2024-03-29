#include "base_request.hpp"

size_t BaseRequest::write_callback(char* ptr, size_t size, size_t nmemb, void* ctx) {
	(void) size;
	std::string* str = reinterpret_cast<std::string*>(ctx);
	size_t size_old = str->size();
	str->append(ptr, nmemb);
	return str->size() - size_old;
}

BaseRequest::BaseRequest() {
	init();
}

BaseRequest::BaseRequest(const std::string& endpoint) {
	init();
	set_endpoint(endpoint);
}

void BaseRequest::init() {
	curl_ = curl_easy_init();
	curl_easy_setopt(curl_, CURLOPT_PROTOCOLS_STR, "HTTP,HTTPS");
	curl_easy_setopt(curl_, CURLOPT_USERAGENT, "circle-scores");
	curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &write_callback);
	curl_easy_setopt(curl_, CURLOPT_TIMEOUT, 25L);
}

BaseRequest::~BaseRequest() {
	if(curl_)
		curl_easy_cleanup(curl_);

	if(curl_headers_)
		curl_slist_free_all(curl_headers_);
}

void BaseRequest::set_endpoint(const std::string& endpoint) {
	curl_easy_setopt(curl_, CURLOPT_URL, &endpoint[0]);
}

int BaseRequest::add_header(const std::string& header) {
	struct curl_slist* tmp = curl_slist_append(curl_headers_, &header[0]);
	if(!tmp)
		return 1;

	curl_headers_ = tmp;
	curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, curl_headers_);
	return 0;
}

long long BaseRequest::perform(std::string& data) {
	curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (void*) &data);
	curl_last_ = curl_easy_perform(curl_);
	if(curl_last_ != CURLE_OK)
		return -1;

	long long code;
	curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &code);
	return code;
}

std::string BaseRequest::last_err() const {
	return curl_easy_strerror(curl_last_);
}
