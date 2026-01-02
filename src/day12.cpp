#include <algorithm>
#include <cstdio>
#include <cstdint>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <array>
#include <bit>
#include <limits>
#include <deque>
#include <map>
#include <ranges>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <numeric>
#include <unordered_map>
#include <memory>

#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

static std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

struct day12region {
	uint64_t w{};
	uint64_t h{};
	std::vector<uint64_t> reqs;
};

using PresentType = std::array<std::array<uint8_t, 3>, 3>;


struct skyline {
	std::vector<std::vector<uint16_t>> _sky;
	uint16_t _maxw;
	uint16_t _maxh;
	
	uint16_t _skylinecount{1};

	skyline(uint16_t w, uint16_t h) : _maxw(w), _maxh(h) {
		_sky.resize(w);
		for (auto& s : _sky) {
			s.resize(h);
		}
	}

	bool add2d(uint16_t w, uint16_t h, uint16_t x, uint16_t y) {
		if (!w || !h)
			return false;

		uint16_t idxbest = std::numeric_limits<uint16_t>::max();
		uint16_t idxbest2 = std::numeric_limits<uint16_t>::max();
		uint16_t bestx = std::numeric_limits<uint16_t>::max();
		uint16_t besty = std::numeric_limits<uint16_t>::max();


	}
};

struct day12 {
	std::regex _r{ R"(([[:digit:]]*)x([[:digit:]]*): ([\s\w]+))" };
	bool _parsingpresent = false;
	size_t _curpresent = 0;
	size_t _curpresentline = 0;

	std::vector<PresentType> _presents;

	std::vector<day12region> _regions;

	void parseline(std::string& line) {
		if (line.empty()) {
			_curpresent++;
			_curpresentline = 0;
			_parsingpresent = false;
			return;
		}

		if (line.length() == 2) {
			_parsingpresent = true;
			_presents.push_back({});
			return;
		}

		if (_parsingpresent) {
			if (line[0] == '#')
				_presents[_curpresent][_curpresentline][0] |= 4;
			if (line[1] == '#')
				_presents[_curpresent][_curpresentline][1] |= 2;
			if (line[2] == '#')
				_presents[_curpresent][_curpresentline][2] |= 1;
			_curpresentline++;
			return;
		}

		std::smatch m;
		std::regex_search(line, m, _r);
		const auto& w = m[1];
		const auto& h = m[2];
		const auto& r = split(m[3], " ");

		day12region reg{};
		reg.w = atoi(w.str().data());
		reg.h = atoi(h.str().data());

		for (const auto& req : r) {
			reg.reqs.push_back(atoi(req.data()));
		}

		_regions.push_back(reg);
	}

	uint64_t getp1() {

		uint64_t numimpossible{};

		for (const auto& r : _regions) {
			// quick check on 'impossible' ones
			uint64_t totalarea = r.w * r.h;
			uint64_t presentsarea = 0;
			for (int idx = 0; const auto& p : _presents) {
				uint64_t parea = 0;
				for (const auto& s : p) {
					parea += std::popcount(s[0]) + std::popcount(s[1]) + std::popcount(s[2]);
				}
				presentsarea += r.reqs[idx++] * parea;
			}

			if (presentsarea > totalarea) {
				//fmt::println("impossible rawr");
				numimpossible++;
			}
		}

		return _regions.size() - numimpossible;
	}

	bool doesfits(const PresentType& present, const day12region& region, uint64_t x, uint64_t y) {
		return false;
	}
};

void day12test() {
	auto data =
		R"(0:
###
##.
##.

1:
###
##.
.##

2:
.##
###
##.

3:
##.
###
##.

4:
###
#..
###

5:
###
.#.
###

4x4: 0 0 0 0 2 0
12x5: 1 0 1 0 2 2
12x5: 1 0 1 0 3 2)";

	day12 d12;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line);) {
		d12.parseline(line);
	}

	SPDLOG_ERROR("{}", d12.getp1());
}

void day12proc() {
	day12 d12;

	std::fstream in("E:\\gitrepos\\aoc2025\\src\\input12.txt", std::ios_base::in | std::ios_base::binary);
	for (std::string line; std::getline(in, line);) {
		d12.parseline(line);
	}

	SPDLOG_ERROR("{}", d12.getp1());
}

int main() {
	day12test();
	day12proc();
	return 0;
}