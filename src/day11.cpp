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

struct day11nodeinfo {
	uint64_t num_paths{};
	bool has_fft{};
	bool has_dac{};
};

struct day11 {
	std::map<std::string, std::set<std::string>> cons;

	std::regex _r{ R"((.*): ([\s\w]+))" };

	void parseline(std::string& line) {
		std::smatch m;
		std::regex_search(line, m, _r);
		const auto& i = m[1];
		const auto& o = split(m[2], " ");

		cons.emplace(i, std::set<std::string>{o.begin(), o.end()});
	}

	uint64_t getp1() {
		std::deque<std::string> paths;
		paths.push_back("you");

		uint64_t num_paths{};

		while (!paths.empty()) {
			auto path = paths.front();
			paths.pop_front();

			const auto& check = cons.at(path);
			for (const auto& c : check) {
				if (c == "out")
					num_paths++;
				else
					paths.push_back(c);
			}
		}

		return num_paths;
	}

	std::vector<std::string> toposort(const std::string& start) {
		std::vector<std::string> rtn;

		auto cons2 = cons;
		
		std::deque<std::string> paths;
		paths.push_back(start);
		while (!paths.empty()) {
			auto path = paths.front();
			paths.pop_front();

			rtn.push_back(path);

			auto& nodes = cons2[path];
			auto n = nodes.begin();
			while (n != nodes.end()) {
				auto npath = *n;
				n = nodes.erase(n);

				bool exists = false;
				for (const auto& c : cons2) {
					if (c.second.find(npath) != c.second.end()) {
						exists = true;
						break;
					}
				}

				if (!exists)
					paths.push_back(npath);
			}
		}
		return rtn;
	}

	uint64_t numpathswithtopo(std::map<std::string, uint64_t>& memo, const std::vector<std::string>& topo, const std::map<std::string, uint64_t>& topoidx, const std::string& start, const std::string& end) {
		if (start == end)
			return 1;

		if (!cons.count(start))
			return 0;

		if (memo.count(start)) {
			return memo.at(start);
		}

		auto pidx = topoidx.at(start);
		uint64_t sum = 0;
		for (auto c : cons.at(start)) {
			auto cidx = topoidx.at(c);
			//if (cidx > pidx)
				sum += numpathswithtopo(memo, topo, topoidx, c, end);
		}
		memo[start] = sum;

		return sum;
	}

	uint64_t memonumpathskillme(const std::vector<std::string>& topo, const std::map<std::string, uint64_t>& topoidx, const std::string& start, const std::string& end) {
		std::map<std::string, std::uint64_t> memo;
		return numpathswithtopo(memo, topo, topoidx, start, end);
	}

	uint64_t getp2() {
		//std::vector<std::string> top;
		auto top = toposort("svr");

		std::map<std::string, uint64_t> topo2;
		for (uint64_t i = 0; const auto& t : top) {
			topo2[t] = i++;
		}

		auto fidx = topo2["fft"];
		auto didx = topo2["dac"];

		uint64_t start{};
		uint64_t end{};
		uint64_t mid{};

		if (fidx <= didx) {
			start = memonumpathskillme(top, topo2, "svr", "fft");
			mid = memonumpathskillme(top, topo2, "fft", "dac");
			end = memonumpathskillme(top, topo2, "dac", "out");
		}
		else {
			start = memonumpathskillme(top, topo2, "svr", "dac");
			mid = memonumpathskillme(top, topo2, "dac", "fft");
			end = memonumpathskillme(top, topo2, "fft", "out");
		}

		return start * mid * end;
	}
};

void day11test() {
	auto data =
		R"(aaa: you hhh
you: bbb ccc
bbb: ddd eee
ccc: ddd eee fff
ddd: ggg
eee: out
fff: out
ggg: out
hhh: ccc fff iii
iii: out)";

	day11 d11;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line);) {
		d11.parseline(line);
	}

	SPDLOG_ERROR("{}", d11.getp1());
}

void day11test2() {
	auto data =
		R"(svr: aaa bbb
aaa: fft
fft: ccc
bbb: tty
tty: ccc
ccc: ddd eee
ddd: hub
hub: fff
eee: dac
dac: fff
fff: ggg hhh
ggg: out
hhh: out)";

	day11 d11;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line);) {
		d11.parseline(line);
	}

	SPDLOG_ERROR("{}", d11.getp2());
}

void day11proc() {
	day11 d11;

	std::fstream in("E:\\gitrepos\\aoc2025\\src\\input11.txt", std::ios_base::in | std::ios_base::binary);
	for (std::string line; std::getline(in, line);) {
		d11.parseline(line);
	}

	SPDLOG_ERROR("{}", d11.getp1());
	SPDLOG_ERROR("{}", d11.getp2());
}

/*
int main() {
	day11test();
	day11test2();
	day11proc();
	return 0;
}*/