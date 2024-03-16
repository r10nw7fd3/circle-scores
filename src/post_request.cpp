#include "post_request.hpp"

PostRequest::PostRequest(const std::string& endpoint, std::string& postdata, size_t size)
	: BaseRequest(endpoint) {
	set_postdata(postdata, size);
}

void PostRequest::init() {
	BaseRequest::init();
	curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "POST");
}

void PostRequest::set_postdata(std::string& postdata) {
	set_postdata(postdata, postdata.size());
}

void PostRequest::set_postdata(std::string& postdata, size_t size) {
	curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, &postdata[0]);
	curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, size);
}
