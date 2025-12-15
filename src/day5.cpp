#include <algorithm>
#include <cstdio>
#include <cstdint>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <map>
#include <set>
#include <vector>

#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

struct day5 {
	bool fresh{ true };

	std::vector<std::pair<uint64_t, uint64_t>> freshvec;
	std::vector<uint64_t> avail;

	void parseline(std::string& line) {
		if (line.empty()) {
			fresh = false;
			return;
		}
		if (fresh) {
			line[line.find('-')] = ' ';

			auto fmt = std::string{ "%llu %llu" };
			uint64_t low, high;
			auto ret = std::sscanf(line.c_str(), fmt.c_str(), &low, &high);
			if (ret != 2)
				assert(false);

			freshvec.push_back({ low, high });
		}
		else {
			auto fmt = std::string{ "%llu "};
			uint64_t num;
			auto ret = std::sscanf(line.c_str(), fmt.c_str(), &num);

			avail.push_back(num);
		}
	}
	
	int getp1() {
		int total{};
		for (const auto& a : avail) {
			for (const auto& r : freshvec) {
				if (a >= r.first && r.second >= a) {
					total++;
					break;
				}
			}
		}
		return total;
	}

	uint64_t getp2() {
		std::vector<std::pair<uint64_t, uint64_t>> squish;
		for (const auto& a : freshvec) {
			bool found = false;
			for (auto& s : squish) {
				if (s.first <= a.second && a.first <= s.second) {
					s.first = std::min(a.first, s.first);
					s.second = std::max(a.second, s.second);
					found = true;
				}
			}
			if (!found) {
				squish.push_back({ a.first, a.second });
			}

		}

		while (true) {
			bool squished = false;
			std::vector<std::pair<uint64_t, uint64_t>> squish2;
			for (const auto& a : squish) {
				bool found = false;
				for (auto& s : squish2) {
					if (s.first <= a.second && a.first <= s.second) {
						auto old = s;
						s.first = std::min(a.first, s.first);
						s.second = std::max(a.second, s.second);
						squished = old != s;
						found = true;
					}
				}
				if (!found) {
					squish2.push_back({ a.first, a.second });
				}

			}
			if (!squished)
				break;
			squish = squish2;
		}

		uint64_t count = 0;
		for (const auto& a : squish) {
			count += a.second - a.first + 1;
		}
		return count;
	}
};


void testd5() {
	auto data =
		R"(3-5
10-14
16-20
12-18

1
5
8
11
17
32
)";

	day5 d5;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line);) {
		d5.parseline(line);
	}

	SPDLOG_ERROR("{}", d5.getp1());
	SPDLOG_ERROR("{}", d5.getp2());
}

void day5proc() {
	day5 d5;

	std::fstream in("E:\\gitrepos\\aoc2025\\src\\input5.txt", std::ios_base::in | std::ios_base::binary);
	for (std::string line; std::getline(in, line);) {
		d5.parseline(line);
	}

	SPDLOG_ERROR("{}", d5.getp1());
	SPDLOG_ERROR("{}", d5.getp2());
}

/*int main() {
	testd5();
	day5proc();
	return 0;
}*/