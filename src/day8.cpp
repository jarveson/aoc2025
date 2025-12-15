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
#include <vector>

#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

struct d8pos {
	int x;
	int y;
	int z;

	constexpr auto operator<=>(const d8pos& b) const = default;
};

struct day8 {
	std::vector<d8pos> data;
	std::vector<std::set<int>> circuits;
	std::vector<std::vector<float>> distances;

	float distance(const d8pos& a, const d8pos& b) {
		return std::sqrtf(std::powf(a.x - b.x, 2.f) + std::powf(a.y - b.y, 2.f) + std::powf(a.z - b.z, 2.f));
	}

	void parseline(std::string& line) {
		auto fmt = std::string{ "%d,%d,%d " };
		d8pos pos;
		auto ret = std::sscanf(line.c_str(), fmt.c_str(), &pos.x, &pos.y, &pos.z);
		data.push_back(pos);
	}

	void calcdistances() {
		std::vector<float> dist;
		for (int x = 0; const auto& d : data) {
			for (const auto& d2 : data) {
				if (d == d2) {
					dist.push_back(std::numeric_limits<float>::infinity());
				}
				else {
					dist.push_back(distance(d, d2));
				}
			}
			distances.push_back(std::move(dist));
		}
	}

	std::tuple<int, int, float> getnextlowest(float min) {
		std::tuple<int, int, float> idxs;
		float lowest = std::numeric_limits<float>::infinity();

		for (int apos = 0;  const auto& a : distances) {
			for (int bpos = 0;  const auto& b : a) {
				if (b > min && b < lowest) {
					lowest = b;
					idxs = { apos, bpos, lowest };
				}
				bpos++;
			}
			apos++;
		}

		return idxs;
	}

	void connectpairs(int num) {
		float lowest = -1.f;
		for (int i = 0; i < num; i++) {
			auto [a, b, newlow] = getnextlowest(lowest);
			lowest = newlow;

			auto acheck = std::ranges::find_if(circuits, [a](const auto& c) {
				return c.find(a) != c.end();
				});
			auto bcheck = std::ranges::find_if(circuits, [b](const auto& c) {
				return c.find(b) != c.end();
				});

			if (acheck != circuits.end() && bcheck != circuits.end()) {
				if (acheck != bcheck) {
					acheck->insert(bcheck->begin(), bcheck->end());
					circuits.erase(bcheck);
				}
			}
			else if (acheck != circuits.end()) {
				acheck->insert(b);
			}
			else if (bcheck != circuits.end()) {
				bcheck->insert(a);
			}
			else {
				std::set<int> tmp;
				tmp.insert(a);
				tmp.insert(b);
				circuits.push_back(std::move(tmp));
			}
		}
	}

	uint64_t connectallpairs() {
		float lowest = -1.f;
		while(true) {
			auto [a, b, newlow] = getnextlowest(lowest);
			lowest = newlow;

			auto acheck = std::ranges::find_if(circuits, [a](const auto& c) {
				return c.find(a) != c.end();
				});
			auto bcheck = std::ranges::find_if(circuits, [b](const auto& c) {
				return c.find(b) != c.end();
				});

			if (acheck != circuits.end() && bcheck != circuits.end()) {
				if (acheck != bcheck) {
					acheck->insert(bcheck->begin(), bcheck->end());
					circuits.erase(bcheck);
				}
			}
			else if (acheck != circuits.end()) {
				acheck->insert(b);
			}
			else if (bcheck != circuits.end()) {
				bcheck->insert(a);
			}
			else {
				std::set<int> tmp;
				tmp.insert(a);
				tmp.insert(b);
				circuits.push_back(std::move(tmp));
			}

			if (circuits.size() == 1 && circuits[0].size() == data.size()) {
				return data[a].x * data[b].x;
			}
		}
	}

	uint64_t getp1(int pairs) {
		calcdistances();

		connectpairs(pairs);

		std::sort(circuits.begin(), circuits.end(), [](const auto& a, const auto& b) {
			return a.size() > b.size();
			});

		return circuits[0].size() * circuits[1].size() * circuits[2].size();
	}

	uint64_t getp2() {
		calcdistances();
		return connectallpairs();
	}
};

void day8test() {
	auto data =
		R"(162,817,812
57,618,57
906,360,560
592,479,940
352,342,300
466,668,158
542,29,236
431,825,988
739,650,466
52,470,668
216,146,977
819,987,18
117,168,530
805,96,715
346,949,466
970,615,88
941,993,340
862,61,35
984,92,344
425,690,689)";

	day8 d8;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line);) {
		d8.parseline(line);
	}

	SPDLOG_ERROR("{}", d8.getp1(10));
	SPDLOG_ERROR("{}", d8.getp2());
}

void day8proc() {
	day8 d8;

	std::fstream in("E:\\gitrepos\\aoc2025\\src\\input8.txt", std::ios_base::in | std::ios_base::binary);
	for (std::string line; std::getline(in, line);) {
		d8.parseline(line);
	}

	SPDLOG_ERROR("{}", d8.getp1(1000));
	SPDLOG_ERROR("{}", d8.getp2());
}

/*int main() {
	day8test();
	day8proc();
	return 0;
}*/