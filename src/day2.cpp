#include <cstdio>
#include <cstdint>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

struct day2pally {
	uint64_t total{};
	uint64_t total2{};

	void parserange(uint64_t low, uint64_t high) {
		for (auto i = low; i <= high; ++i) {
			auto str = fmt::format("{}", i);
			if ((str.length() % 2) != 0)
				continue;
			int z = 0;
			while (true) {
				if (str[z] != str[str.length() / 2 + z])
					break;
				else if (z == (str.length() / 2) - 1) {
					total += i;
					SPDLOG_ERROR("invalid {}", i);
					break;
				}
				z++;
			}
		}
	}

	void parserange2(uint64_t low, uint64_t high) {
		for (auto i = low; i <= high; ++i) {
			auto str = fmt::format("{}", i);

			for (auto z = 1; z <= str.length() / 2; z++) {
				auto sub = str.substr(0, z);
				auto w = 0;
				bool invalid = false;
				while (true) {
					auto y = str.substr(w, z);
					if (y != sub)
						break;
					else if (w + z == str.length()) {
						total2 += i;
						invalid = true;
						SPDLOG_ERROR("invalid {}", i);
						break;
					}
					w += z;
				}
				if (invalid)
					break;
			}
		}
	}

	void parseline(std::string& str) {
		str[str.find('-')] = ' ';

		auto fmt = std::string{ "%llu %llu" };
		uint64_t low, high;
		auto ret = std::sscanf(str.c_str(), fmt.c_str(), &low, &high);
		if (ret != 2)
			assert(false);

		parserange(low, high);
		parserange2(low, high);
	}
};

void givendata2() {
	auto data =
		R"(11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124)";

	day2pally d1l;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line, ',');) {
		d1l.parseline(line);
	}

	SPDLOG_ERROR("{}", d1l.total);
	SPDLOG_ERROR("{}", d1l.total2);
	EXPECT_EQ(d1l.total, 1227775554);
}

void day2part1() {
	auto data =
		R"(990244-1009337,5518069-5608946,34273134-34397466,3636295061-3636388848,8613701-8663602,573252-688417,472288-533253,960590-988421,7373678538-7373794411,178-266,63577667-63679502,70-132,487-1146,666631751-666711926,5896-10827,30288-52204,21847924-21889141,69684057-69706531,97142181-97271487,538561-555085,286637-467444,93452333-93519874,69247-119122,8955190262-8955353747,883317-948391,8282803943-8282844514,214125-236989,2518-4693,586540593-586645823,137643-211684,33-47,16210-28409,748488-837584,1381-2281,1-19)";
	day2pally d1l;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line, ',');) {
		d1l.parseline(line);
	}

	SPDLOG_ERROR("{}", d1l.total);
	SPDLOG_ERROR("{}", d1l.total2);
}

/*int main() {
	givendata2();
	day2part1();
	return 0;
}*/