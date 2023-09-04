#include "processor.hpp"

#include <thread>
#include <stdexcept>
#include <chrono>
#include <cmath>
#include <sstream>
#include "log.hpp"
#include "rankings.hpp"
#include "recent_scores.hpp"
#include "legacy_replay.hpp"

Processor::Processor(int delay, int page, Credentials& creds)
	: delay_(delay), page_(page), creds_(creds), token_(creds.get_osu_id(), creds.get_osu_key()), discord_(creds.get_discord_hook_url()) {
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

void Processor::to_text(const osu::Score& e, std::string& ret) {
	std::string country = std::string(":flag_").append(e.get_country()).append(":");
	for(char& c : country)
		c = std::tolower(c);
	std::string acc = std::to_string(std::round(e.get_acc() / 0.01) * 0.01);
	for(int i = 0; i < 4 && !acc.empty(); ++i)
			acc.pop_back();

	ret.append(country).append(" ").append(e.get_player());
	ret += " | ";
	ret.append(e.get_artist()).append(" - ").append(e.get_song()).append(" [").append(e.get_diff()).append("] ");
	ret += acc.append("% +").append(e.get_mods());
	ret += " ";
	ret += (e.get_misses() ? std::to_string(e.get_misses()).append("miss ") : "FC? ");
	ret += std::to_string(e.get_pp());
	ret += "pp ";
	ret += e.get_score_url();
}

void Processor::download_replay(const osu::Score& e, std::string& ret) {
	osu::requests::LegacyReplay lr(creds_.get_osu_legacy_key(), e.get_score_id());
	std::string replay_lzma;
	long long lr_ret = lr.perform(replay_lzma);
	std::cout << LOGI"LegacyReplay code: " << lr_ret << std::endl;
	if(lr_ret >= 0 || lr_ret / 100 == 2) {
		std::ostringstream ss;
		if(!e.write_dummy_replay(ss, replay_lzma))
			ret = ss.str();
		else
			std::cout << LOGE"write_dummy_replay() failed" << std::endl;
	}
}

void Processor::post_discord(const osu::Score& e) {
	std::string msg;
	to_text(e, msg);
	std::cout << msg << std::endl;

	std::string osr;
	download_replay(e, osr);

	long long ret = discord_.post(msg, e.get_cover_url(), osr);
	if(ret < 0 || ret / 100 != 2)
		std::cout << LOGE"Failed to post score to discord, ret = " << ret << std::endl;
}

// TODO: Do not needlessly reconstruct objects
void Processor::query() {
	const std::string& tkn = token_.get_token();
	if(tkn.size() == 0)
		return;

	long long ret;

	unsigned long long start = get_unix_ms();
	std::cout << LOGI"Pulling rankings" << std::endl;
	osu::requests::Rankings rankings(tkn, page_);
	std::vector<std::tuple<int, std::string>> lb;
	ret = rankings.perform(lb);
	if(ret < 0 || ret / 100 != 2) {
		std::cout << LOGE"Failed to pull rankings, ret = " << ret << std::endl;
		return;
	}

	for(const auto& p : lb) {
		std::cout << LOGI"Pulling scores for " << std::get<1>(p) << std::endl;
		osu::requests::RecentScores rs(tkn, std::get<0>(p));
		std::vector<osu::Score> scores;
		ret = rs.perform(scores);
		if(ret < 0 || ret / 100 != 2) {
			std::cout << LOGE"Failed to pull scores, ret = " << ret << std::endl;
			continue;
		}

		for(const osu::Score& s : scores) {
			// RecentScores class only warns us about errors in score,
			// let's skip failed scores manually
			if(s.get_error())
				continue;

			if(s.get_time() < get_last_update(std::get<0>(p)) || s.get_pp() < 800)
				continue;

			post_discord(s);
		}
		set_last_update(std::get<0>(p));;
	}
	std::cout << LOGI"Done. Took " << get_unix_ms() - start << "ms" << std::endl;
}

int Processor::run() {
	for(;;) {
		query();
		std::this_thread::sleep_for(std::chrono::seconds(delay_));
	}
	return 0;
}
