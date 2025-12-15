#include <algorithm>
#include <cstdio>
#include <cstdint>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <deque>
#include <map>
#include <set>
#include <vector>

#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

struct day7 {
	std::vector<std::vector<int64_t>> grid;
	std::pair<int, int> start;


	void parseline(std::string& line) {
		std::vector<int64_t> row;
		for (const auto& c : line) {
			if (c == '.') {
				row.push_back(0);
			}
			else if (c == 'S') {
				start = { (int)row.size(), (int)grid.size() };
				row.push_back(2);
			}
			else if (c == '^') {
				row.push_back(1);
			}
		}
		grid.push_back(std::move(row));
	}

	int getp1() {
		auto grid2 = grid;

		int splits = 0;

		for (int row = 1; row < grid2.size(); ++row) {
			const auto& prev = grid2[row - 1];
			for (int p = 0; const auto& c : prev) {
				auto pos = p++;
				if (c == 2) {
					if (grid2[row][pos] == 1) {
						splits++;
						if (pos > 0)
							grid2[row][pos - 1] = 2;
						if (pos < grid[0].size())
							grid2[row][pos + 1] = 2;
					}
					else if (grid2[row][pos] == 0) {
						grid2[row][pos] = 2;
					}
				}
				
			}
		}

		/*for (const auto& r : grid2) {
			std::string row;
			for (const auto& c : r) {
				if (c == 0)
					row.append(".");
				else if (c == 2)
					row.append("|");
				else if (c == 1)
					row.append("^");
				//row.append(fmt::format("{}", c));
			}
			SPDLOG_ERROR("{}", row);
		}*/

		return splits;
	}

	uint64_t getp2old() {
		std::deque<std::pair<int, int>> q;
		q.push_back(start);

		int maxx = grid[0].size();
		int maxy = grid.size();

		uint64_t splits = 0;

		while (!q.empty()) {
			auto p = q.front();
			q.pop_front();
			auto x = p.first;
			auto y = p.second;

			// search down
			if (y >= maxy - 1) {
				continue;
			}
			for (int row = y + 1; row < maxy; ++row) {
				if (grid[row][x] == 0) {
					if (row == maxy - 1)
						splits++;
					continue;
				}
				else if (grid[row][x] == 1) {
					if (x < maxx - 1) {
						q.push_back({ x + 1, row });
					}
					if (x > 0) {
						q.push_back({ x - 1, row });
					}
				}
			}
		}
		return splits;
	}

	uint64_t bfspaths(int x, int y) {
		std::deque<std::pair<int, int>> q;
		q.push_back({x,y});

		int maxx = grid[0].size();
		int maxy = grid.size();

		uint64_t splits{};

		while (!q.empty()) {
			auto p = q.front();
			q.pop_front();
			auto x = p.first;
			auto y = p.second;

			if (y >= maxy - 1) {
				continue;
			}
			for (int row = y + 1; row < maxy; ++row) {
				if (grid[row][x] == 0) {
					if (row == maxy - 1)
						splits++;
					continue;
				}
				else if (grid[row][x] == 1) {
					if (x < maxx - 1) {
						q.push_back({ x + 1, row });
					}
					if (x > 0) {
						q.push_back({ x - 1, row });
					}
				}
				else if (grid[row][x] < 0) {
					splits += std::abs(grid[row][x]);
					break;
				}
			}
		}
		return splits;
	}

	uint64_t getp2() {
		std::deque<std::pair<int, int>> q;
		q.push_back(start);

		int maxx = grid[0].size();
		int maxy = grid.size();

		uint64_t splits = 0;

		{
			auto& row = grid[grid.size() - 2];
			for (auto& c : row) {
				if (c == 1) {
					c = -2;
				}
			}
		}

		for (int row = grid.size() - 4; row > 0; row--) {
			for (int c = 0; c < maxx - 1; ++c) {
				auto num = grid[row][c];
				if (num == 1) {
					int64_t cnt = 0;
					if (c < maxx - 1)
						cnt += bfspaths(c + 1, row);
					if (c > 0)
						cnt += bfspaths(c - 1, row);
					grid[row][c] = cnt * -1;
				}
			}
		}

		return bfspaths(start.first, start.second);
	}
};

void day7test() {
	auto data =
		R"(.......S.......
...............
.......^.......
...............
......^.^......
...............
.....^.^.^.....
...............
....^.^...^....
...............
...^.^...^.^...
...............
..^...^.....^..
...............
.^.^.^.^.^...^.
...............)";

	day7 d7;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line);) {
		d7.parseline(line);
	}

	SPDLOG_ERROR("{}", d7.getp1());
	SPDLOG_ERROR("{}", d7.getp2());
}

void day7proc() {
	day7 d7;

	std::fstream in("E:\\gitrepos\\aoc2025\\src\\input7.txt", std::ios_base::in | std::ios_base::binary);
	for (std::string line; std::getline(in, line);) {
		d7.parseline(line);
	}

	SPDLOG_ERROR("{}", d7.getp1());
	SPDLOG_ERROR("{}", d7.getp2());
}

/*int main() {
	day7test();
	day7proc();

	return 0;
}*/