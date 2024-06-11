#include "stdout_score_receiver.hpp"
#include <iostream>

void StdoutScoreReceiver::on_score(const Score& score) {
	std::cout << score.format() << std::endl;
}
