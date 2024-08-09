#pragma once

#include "score_receiver.hpp"
#include "post_request.hpp"
#include "get_request.hpp"

class LamsScoreReceiver: public ScoreReceiver {
public:
	LamsScoreReceiver(const std::string& address, const std::string& dir);
	virtual void on_score(const Score& score) override;

private:
	PostRequest post_;
	GetRequest get_;
	std::string dir_;

	std::string make_postdata(const Score& score);
	void try_create_dir();
};
