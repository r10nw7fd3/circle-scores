#pragma once

#include "score_receiver.hpp"
#include "post_request.hpp"
#include "get_request.hpp"

class DiscordScoreReceiver: public ScoreReceiver {
private:
	std::string url_;
	PostRequest post_;
	GetRequest get_;

public:
	DiscordScoreReceiver(const std::string&);
	virtual void on_score(const Score& score) override;
};
