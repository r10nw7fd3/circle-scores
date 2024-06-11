#include "discord_score_receiver.hpp"
#include "log.hpp"

DiscordScoreReceiver::DiscordScoreReceiver(const std::string& url)
	: url_(url), post_(url, "") {
}

void DiscordScoreReceiver::on_score(const Score& score) {
	std::string text = score.format(true);

	get_.set_endpoint(score.get_cover_url());
	std::string cover_data;
	long long ret = get_.perform(cover_data);
	if(ret < 0 || ret / 100 != 2)
		std::cout << LOGE"Failed to download cover, ret = " << ret << std::endl;

	std::string payload = "{ \"content\": \"";
	payload += text;
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
}
