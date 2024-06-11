#pragma once

#include <unordered_map>
#include <vector>
#include <ctime>
#include "args.hpp"
#include "credentials.hpp"
#include "token.hpp"
#include "score.hpp"
#include "score_receiver.hpp"

class Processor {
private:
	const Args& args_;
	const Credentials& creds_;
	Token token_;
	std::unordered_map<int, time_t> last_update_;
	std::vector<ScoreReceiver*> receivers_;

	static void to_text(const Score&, std::string&);
	time_t get_last_update(int);
	time_t set_last_update(int);
	void download_replay(const Score&, std::string&);
	void handle_score(const Score& score);
	void query();
public:
	Processor(const Args&, const Credentials&);

	int run();
	void register_receiver(ScoreReceiver& recvr);
	const Token& get_token() const;
};
