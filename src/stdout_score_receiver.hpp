#pragma once

#include "score_receiver.hpp"

class StdoutScoreReceiver: public ScoreReceiver {
public:
	virtual void on_score(const Score& score) override;
};
