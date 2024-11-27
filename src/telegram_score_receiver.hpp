#pragma once

#include "score_receiver.hpp"
#include "post_request.hpp"

class TelegramScoreReceiver: public ScoreReceiver {
public:
	TelegramScoreReceiver(const std::string& token, const std::string& channel);
	virtual void on_score(const Score& score) override;

private:
	std::string channel_;
	PostRequest post_;
};
