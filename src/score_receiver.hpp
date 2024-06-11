#pragma once

#include "score.hpp"

class ScoreReceiver {
public:
	virtual ~ScoreReceiver() {}
	virtual void on_score(const Score&) = 0;
};
