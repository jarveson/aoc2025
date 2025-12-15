#include <algorithm>
#include <cstdio>
#include <cstdint>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <limits>
#include <deque>
#include <map>
#include <ranges>
#include <set>
#include <thread>
#include <vector>
#include <mutex>
#include <numeric>
#include <unordered_map>

#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
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

struct d10mach {
	uint32_t lights{};
	std::vector<uint32_t> buttons;
	std::vector<int> joltage;
};

struct day10 {
	std::vector<d10mach> machines;

	void parseline(std::string& line) {
		auto a = line.find(']');
		auto l = line.substr(1, a - 1);
		line = line.substr(a + 1);

		d10mach mach;

		for (int i = 0; i < l.size(); ++i) {
			int bit = 1 << i;
			mach.lights |= (bit * (l[i] == '.' ? 0 : 1));
		}

		while (true) {
			auto p1 = line.find('(');
			auto p2 = line.find(')');
			if (p2 == std::string::npos) {
				break;
			}
			auto bs = split(line.substr(p1 + 1, p2 - p1 - 1), ",");
			uint32_t but{};
			for (const auto& b : bs) {
				but |= 1 << (atoi(b.data()));
			}
			mach.buttons.push_back(but);

			line = line.substr(p2 + 1);
		}

		auto j1 = line.find('{');
		auto j2 = line.find('}');

		auto js = split(line.substr(j1 + 1, j2 - j1 - 1), ",");
		for (const auto& j : js) {
			mach.joltage.push_back(atoi(j.data()));
		}
		machines.push_back(mach);
	}

	uint64_t getminpresses(const d10mach& m) {
		const uint64_t attempts = 1ull << m.buttons.size();
		auto set = 0;
		while (true) {
			set += 1;
			auto lights = m.lights;

			auto n = (1ull << set) - 1;
			if (n == 0 || n > attempts)
				assert(false);
			while (n < attempts) {
				auto check = 0;
				for (int i = 0; i < m.buttons.size(); ++i) {
					if (((1ull << i) & n) != 0) {
						check ^= m.buttons[i];
						if (lights == check)
							return set;
					}
				}
				auto smallest = n & -n;
				auto ripple = n + smallest;
				auto ones = n ^ ripple;
				auto next = (ones >> 2) / smallest;
				n = ripple | next;
			}
		}

		return 0;
	}

	uint64_t getp1() {
		uint32_t cnt = 0;
		for (const auto& m : machines)
			cnt += getminpresses(m);
		return cnt;
	}
};

void day10test() {
	auto data =
		R"([.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5})";

	day10 d10;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line);) {
		d10.parseline(line);
	}

	SPDLOG_ERROR("{}", d10.getp1());
	//SPDLOG_ERROR("{}", d9.getp2());
}

void day10proc() {
	day10 d10;

	std::fstream in("E:\\gitrepos\\aoc2025\\src\\input10.txt", std::ios_base::in | std::ios_base::binary);
	for (std::string line; std::getline(in, line);) {
		d10.parseline(line);
	}

	SPDLOG_ERROR("{}", d10.getp1());
	//SPDLOG_ERROR("{}", d9.getp2());
}

int main() {
	day10test();
	day10proc();
	return 0;
}