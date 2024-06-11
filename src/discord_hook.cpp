#include "discord_hook.hpp"
#include "log.hpp"

DiscordHook::DiscordHook(const std::string& url)
	: url_(url), post_(url, "") {
}

long long DiscordHook::post(const std::string& data, const std::string& cover_url) {
	get_.set_endpoint(cover_url);
	std::string cover_data;
	long long ret = get_.perform(cover_data);
	if(ret < 0 || ret / 100 != 2)
		std::cout << LOGE"Failed to download cover, ret = " << ret << std::endl;

	std::string payload = "{ \"content\": \"";
	payload += data;
	payload += "\"";
	if(!cover_data.empty()) {
		payload += ", \"attachments\": [";
		payload += "{ \"id\": 0 }";
		payload += "]";
		post_.add_mimepart("files[0]", cover_data, "cover.jpg", "image/jpeg");
	}
	payload += "}";
	std::cout << LOGI"Resulting payload: " << payload << std::endl;

	post_.add_mimepart("payload_json", payload, "", "application_json");

	std::string response;
	ret = post_.perform(response);
	std::cout << LOGI"Discord response: " << response << std::endl;
	std::cout << LOGI"Discord response code: " << ret << std::endl;

	post_.clear_mime();

	return ret;
}
