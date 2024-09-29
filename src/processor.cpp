#include "processor.hpp"

#include <thread>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <algorithm>
#include "log.hpp"
#include "rankings.hpp"
#include "recent_scores.hpp"

Processor::Processor(const Args& args, const Credentials& creds)
	: args_(args), creds_(creds),
	token_(creds.get_osu_id(), creds.get_osu_key(), args.get_token_filename()) {
}

time_t Processor::get_last_update(int id) {
	try {
		return last_update_.at(id);
	}
	catch(std::out_of_range& e) {
		return set_last_update(id);
	}
}

time_t Processor::set_last_update(int id) {
	return last_update_[id] = time(nullptr);	
}

void Processor::handle_score(const Score& score) {
	for(auto* val : receivers_) {
		val->on_score(score);
	}
}

// TODO: Do not needlessly reconstruct objects
void Processor::query() {
	const std::string& tkn = token_.get_token();
	if(tkn.size() == 0)
		return;

	long long ret;

	unsigned long long start = get_unix_ms();
	LOGI << "Pulling rankings" << std::endl;
	Rankings rankings(tkn, args_.get_page());
	std::vector<std::tuple<int, std::string>> lb;
	ret = rankings.perform(lb);
	if(ret < 0 || ret / 100 != 2) {
		LOGE << "Failed to pull rankings, ret = " << ret << std::endl;
		return;
	}

	for(const auto& p : lb) {
		auto& exclude = args_.get_exclude();
		if(std::find(exclude.begin(), exclude.end(), std::get<0>(p)) != exclude.end())
			continue;

		LOGI << "Pulling scores for " << std::get<1>(p) << std::endl;
		RecentScores rs(tkn, std::get<0>(p));
		std::vector<Score> scores;
		ret = rs.perform(scores);
		if(ret < 0 || ret / 100 != 2) {
			LOGE << "Failed to pull scores, ret = " << ret << std::endl;
			continue;
		}

		for(const Score& s : scores) {
			// RecentScores class only warns us about errors in score,
			// let's skip failed scores manually
			if(s.get_error())
				continue;

			if(s.get_time() < get_last_update(std::get<0>(p)) || s.get_pp() < args_.get_lower_pp_bound())
				continue;

			handle_score(s);
		}
		set_last_update(std::get<0>(p));;
	}
	LOGI << "Done. Took " << get_unix_ms() - start << "ms" << std::endl;
}

int Processor::run() {
	for(;;) {
		query();
		std::this_thread::sleep_for(std::chrono::seconds(args_.get_delay()));
	}
	return 0;
}

void Processor::register_receiver(ScoreReceiver& recvr) {
	receivers_.push_back(&recvr);
}

const Token& Processor::get_token() const {
	return token_;
}
