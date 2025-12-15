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

struct d6data {
	std::vector<uint64_t> nums;
	bool add{};
};

struct day6 {
	std::vector<d6data> data;
	std::vector<std::string> grid;

	void parseline(const std::string& src) {
		grid.push_back(src);

		std::istringstream input;
		input.str(src);

		int pos = 0;
		int cols = 0;
		for (std::string line; std::getline(input, line, ' ');) {
			if (line.empty()) {
				cols++;
				continue;
			}

			if (data.size() <= pos)
				data.resize(pos + 1);
			auto& dat = data[pos];

			if (line[0] == '*' || line[0] == '+') {
				dat.add = line[0] == '+';
			}
			else {
				auto fmt = std::string{ "%llu " };
				uint64_t num;
				auto ret = std::sscanf(line.c_str(), fmt.c_str(), &num);
				dat.nums.push_back(num);
			}
			pos++;
		}
	}

	uint64_t getp1() {
		uint64_t total{};
		for (const auto& d : data) {
			uint64_t subtotal{};
			if (d.add) {
				for (const auto& n : d.nums) {
					subtotal += n;
				}
			}
			else {
				subtotal = 1;
				for (const auto& n : d.nums) {
					subtotal *= n;
				}
			}
			total += subtotal;
		}
		return total;
	}
	uint64_t getp2() {
		int start = grid[0].size() - 1;
		uint64_t total{};
		d6data data;
		while (true) {
			bool end = false;
			std::string num;
			while (true) {
				int cur = start;
				if (grid[0][cur] != ' ')
					num.push_back(grid[0][cur]);
				if (grid[1][cur] != ' ')
					num.push_back(grid[1][cur]);
				if (grid[2][cur] != ' ')
					num.push_back(grid[2][cur]);
				if (grid[3][cur] != ' ') {
					if (grid[3][cur] == '*' || grid[3][cur] == '+') {
						end = true;
						data.add = grid[3][cur] == '+';
					}
					else {
						num.push_back(grid[3][cur]);
					}
				}
				if (grid.size() > 4 && (grid[4][cur] == '*' || grid[4][cur] == '+')) {
					end = true;
					data.add = grid[4][cur] == '+';
				}

				data.nums.push_back(std::atoi(num.data()));
				break;
			}
			if (end) {
				uint64_t subtotal{};
				if (data.add) {
					for (const auto& n : data.nums) {
						subtotal += n;
					}
				}
				else {
					subtotal = 1;
					for (const auto& n : data.nums) {
						subtotal *= n;
					}
				}
				total += subtotal;
				data = {};
				start--;
			}

			start--;
			if (start < 0)
				break;
		}
		return total;
	}
};

void testd6() {
	auto data =
		R"(123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  )";

	day6 d6;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line);) {
		d6.parseline(line);
	}

	SPDLOG_ERROR("{}", d6.getp1());
	SPDLOG_ERROR("{}", d6.getp2());
}

void day6proc() {
	day6 d6;

	std::fstream in("E:\\gitrepos\\aoc2025\\src\\input6.txt", std::ios_base::in | std::ios_base::binary);
	for (std::string line; std::getline(in, line);) {
		d6.parseline(line);
	}

	SPDLOG_ERROR("{}", d6.getp1());
	SPDLOG_ERROR("{}", d6.getp2());
}

/*int main() {
	testd6();
	day6proc();
	return 0;
}*/