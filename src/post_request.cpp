#include "post_request.hpp"

PostRequest::PostRequest(const std::string& endpoint, const std::string& postdata, size_t size)
	: BaseRequest(endpoint) {
	set_postdata(postdata, size);
}

PostRequest::~PostRequest() {
	clear_mime();
}

void PostRequest::init() {
	BaseRequest::init();
	curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "POST");
}

void PostRequest::set_postdata(const std::string& postdata) {
	set_postdata(postdata, postdata.size());
}

void PostRequest::set_postdata(const std::string& postdata, size_t size) {
	curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, &postdata[0]);
	curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, size);
}

void PostRequest::add_mimepart(const std::string& name, const std::string& data,
	const std::string& filename, const std::string& type) {
	if(!mime_) {
		mime_ = curl_mime_init(curl_);
		curl_easy_setopt(curl_, CURLOPT_MIMEPOST, mime_);
	}

	curl_mimepart* field = curl_mime_addpart(mime_);
	if(!name.empty())
		curl_mime_name(field, name.c_str());
	if(!data.empty())
		curl_mime_data(field, data.c_str(), data.size());
	if(!filename.empty())
		curl_mime_filename(field, filename.c_str());
	if(!type.empty())
		curl_mime_type(field, type.c_str());
}

void PostRequest::clear_mime() {
	if(mime_) {
		curl_mime_free(mime_);
		mime_ = nullptr;
	}
}
