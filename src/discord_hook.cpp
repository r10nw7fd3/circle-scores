#include "discord_hook.hpp"
#include "log.hpp"

DiscordHook::DiscordHook(const std::string& url)
	: url_(url), post_(url) {
}

long long DiscordHook::post(const std::string& data, const std::string& cover_url, const std::string& osr) {
	get_.set_endpoint(cover_url);
	std::string cover_data;
	long long ret = get_.perform(cover_data);
	if(ret < 0 || ret / 100 != 2)
		std::cout << LOGE"Failed to download cover, ret = " << ret << std::endl;

	curl_mime* form = curl_mime_init(post_.get_curl());
	curl_mimepart* field = nullptr;

	std::string payload = "{ \"content\": \"";
	payload += data;
	payload += "\"";
	if(!osr.empty() || !cover_data.empty()) {
		payload += ", \"attachments\": [";
		if(!osr.empty()) {
			payload += "{ \"id\": 0 }";
			field = curl_mime_addpart(form);
			curl_mime_name(field, "files[0]");
			curl_mime_data(field, &osr[0], osr.size());
			curl_mime_filename(field, "replay.osr");
		}
		if(!cover_data.empty()) {
			if(!osr.empty())
				payload += ", ";

			payload += "{ \"id\": 1 }";
			field = curl_mime_addpart(form);
			curl_mime_name(field, "files[1]");
			curl_mime_data(field, &cover_data[0], cover_data.size());
			curl_mime_filename(field, "cover.jpg");
			curl_mime_type(field, "image/jpeg");
		}
		payload += "]";
	}
	payload += "}";

	std::cout << LOGI"Resulting payload: " << payload << std::endl;

	field = curl_mime_addpart(form);
	curl_mime_name(field, "payload_json");
	curl_mime_data(field, &payload[0], CURL_ZERO_TERMINATED);
	curl_mime_type(field, "application/json");

	curl_easy_setopt(post_.get_curl(), CURLOPT_POSTFIELDS, "");
	curl_easy_setopt(post_.get_curl(), CURLOPT_MIMEPOST, form);

	std::string response;
	ret = post_.perform(response);
	std::cout << LOGI"Discord response: " << response << std::endl;
	std::cout << LOGI"Discord response code: " << ret << std::endl;

	return ret;
}
