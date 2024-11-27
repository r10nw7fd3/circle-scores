#include "telegram_score_receiver.hpp"
#include "log.hpp"

TelegramScoreReceiver::TelegramScoreReceiver(const std::string& token, const std::string& channel)
	: channel_(channel), post_("https://api.telegram.org/bot" + token + "/sendPhoto") {
	post_.add_header("Content-Type: application/json");
}

void TelegramScoreReceiver::on_score(const Score& score) {
	std::string text = "{\"chat_id\": \"" + channel_ + "\", \"caption\": \"" +
		score.format(false) + "\", \"photo\": \"" + score.get_cover_url() + "\"}";
	post_.set_postdata(text);

	std::string response;
	long long ret = post_.perform(response);
	(ret < 0 || ret / 100 != 2) ? LOGE : LOGI
		<< "Telegram response code:" << ret << std::endl;

	LOGI << "Telegram response: " << response << std::endl;
}
