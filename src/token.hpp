#pragma once

#include <ctime>
#include "post_request.hpp"

namespace osu::requests {
	class Token {
	private:
		time_t expire_ = 0;
		std::string postfields_;
		PostRequest req_;
		std::string token_;

		int generate();
	public:
		Token(const std::string&, const std::string&);

		const std::string& get_token();
		int revoke() const;
	};
}
