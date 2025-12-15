#include <algorithm>
#include <cstdio>
#include <cstdint>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

struct d3p1 {
	int total{ 0 };
	uint64_t total2{ 0 };

	void parseline(const std::string& line) {
		int max = 0;
		std::string tmp;
		tmp.resize(2);
		for (int i = 0; i < line.length(); ++i) {
			auto c = line[i];
			tmp[0] = c;
			for (int j = i + 1; j < line.length(); ++j) {
				auto d = line[j];
				tmp[1] = d;
				auto num = std::atoi(tmp.data());
				max = std::max(num, max);
			}
		}
		total += max;
	}
	void parseline2(const std::string& line) {
		std::string maxstr = "000000000000";

		int start = 0;
		int pos = 0;
		for (auto& c : maxstr) {
			auto it = std::max_element(line.begin() + start, line.end() - (11 - pos));
			pos++;
			c = *it;
			start = std::distance(line.begin(), it) + 1;
		}

		auto fmt = std::string{ "%llu" };
		uint64_t num;
		auto ret = std::sscanf(maxstr.c_str(), fmt.c_str(), &num);
		SPDLOG_ERROR("max = {}", num);

		total2 += num;
	}
};


void day3part1() {

	d3p1 d3;

	std::fstream in("E:\\gitrepos\\aoc2025\\src\\input3.txt", std::ios_base::in | std::ios_base::binary);
	for (std::string line; std::getline(in, line);) {
		d3.parseline(line);
	}
	SPDLOG_ERROR("max {}", d3.total);
}

void day3part2() {

	d3p1 d3;

	std::fstream in("E:\\gitrepos\\aoc2025\\src\\input3.txt", std::ios_base::in | std::ios_base::binary);
	for (std::string line; std::getline(in, line);) {
		d3.parseline2(line);
	}
	SPDLOG_ERROR("max {}", d3.total2);
}

void d3test() {
	auto data =
		R"(987654321111111
811111111111119
234234234234278
818181911112111
)";

	d3p1 d3;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line);) {
		d3.parseline(line);
	}

	SPDLOG_ERROR("max = {}", d3.total);
}

void d3test2() {
	auto data =
		R"(987654321111111
811111111111119
234234234234278
818181911112111
)";

	d3p1 d3;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line);) {
		d3.parseline2(line);
	}

	SPDLOG_ERROR("max = {}", d3.total2);
}

/*int main() {
	//d3test();
	//d3test2();
	//day3part1();
	day3part2();
	return 0;
}*/