#include <algorithm>
#include <cstdio>
#include <cstdint>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <map>
#include <vector>

#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

struct day4 {
	std::vector<std::vector<int>> grid;
	int total = 0;
	int total2 = 0;

	void parseline(const std::string& line) {
		std::vector<int> row;
		for (const auto& c : line) {
			row.push_back(c == '@' ? 1 : 0);
		}
		grid.push_back(std::move(row));
	}

	bool viable(int x, int y) {
		int maxy = grid.size();
		int maxx = grid[0].size();

		int count = 0;
		// top row
		if (y > 0) {
			if (x > 0)
				count += grid[y - 1][x - 1];
			count += grid[y - 1][x];
			if (x < maxx - 1)
				count += grid[y - 1][x + 1];
		}
		// middle row
		if (x > 0)
			count += grid[y][x - 1];
		if (x < maxx - 1)
			count += grid[y][x + 1];

		// bottom row
		if (y < maxy - 1) {
			if (x > 0)
				count += grid[y + 1][x - 1];
			count += grid[y + 1][x];
			if (x < maxx - 1)
				count += grid[y + 1][x + 1];
		}

		return count < 4;
	}

	int getp1() {
		int maxy = grid.size();
		int maxx = grid[0].size();

		for (int y = 0; y < maxy; ++y) {
			for (int x = 0; x < maxx; ++x) {
				if (grid[y][x] == 1) {
					if (viable(x, y)) {
						total++;
					}
				}
			}
		}
		return total;
	}

	int getp2() {
		int maxy = grid.size();
		int maxx = grid[0].size();

		while (true) {
			std::vector<std::pair<int, int>> to_remove;
			int removed = 0;
			for (int y = 0; y < maxy; ++y) {
				for (int x = 0; x < maxx; ++x) {
					if (grid[y][x] == 1) {
						if (viable(x, y)) {
							removed++;
							to_remove.push_back({ x,y });
						}
					}
				}
			}
			if (removed == 0)
				break;
			for (const auto& p : to_remove) {
				grid[p.second][p.first] = 0;
			}
			total2 += removed;
		}

		return total2;
	}
};

void testd4p1() {
	auto data =
		R"(..@@.@@@@.
@@@.@.@.@@
@@@@@.@.@@
@.@@@@..@.
@@.@@@@.@@
.@@@@@@@.@
.@.@.@.@@@
@.@@@.@@@@
.@@@@@@@@.
@.@.@@@.@.
)";

	day4 d4;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line);) {
		d4.parseline(line);
	}

	d4.getp1();
	d4.getp2();

	SPDLOG_ERROR("{}", d4.total);
	SPDLOG_ERROR("{}", d4.total2);
}


void day4part1() {
	day4 d4;

	std::fstream in("E:\\gitrepos\\aoc2025\\src\\input4.txt", std::ios_base::in | std::ios_base::binary);
	for (std::string line; std::getline(in, line);) {
		d4.parseline(line);
	}

	d4.getp1();
	d4.getp2();
	SPDLOG_ERROR("{}", d4.total);
	SPDLOG_ERROR("{}", d4.total2);
}



/*int main() {
	testd4p1();
	day4part1();
	return 0;
}*/