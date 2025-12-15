#include <cstdio>
#include <cstdint>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

struct day1lock {
	uint32_t zerocnt{};
	uint32_t zerocntpart2{};
	uint32_t current{ 50 };

	void movedial(bool left, uint32_t num) {
		if (num == 0)
			return;

		bool first = true;
		if (left) {
			while (num != 0) {
				if (current == 0) {
					current = 99;
				}
				else
					current--;
				if (current == 0)
					zerocntpart2++;
				num--;
			}
		}
		else {
			while (num != 0) {
				if (current == 99) {
					current = 0;
				}
				else 
					current++;
				if (current == 0)
					zerocntpart2++;
				num--;
			}
		}
		if (current == 0)
			zerocnt++;
		/*zerocntpart2 += num / 100;
		num %= 100;

		if (left) {
			if (num > current) {
				num -= current;
				current = 100 - num;
				zerocntpart2++;
			}
			else {
				current -= num;
			}
		}
		else {
			if (num + current >= 100) {
				current += num;
				current %= 100;
				zerocntpart2++;
			}
			else {
				current += num;
			}
		}
		if (current == 0)
			zerocnt++;*/
	}

	void parseline(std::string& data) {
		auto dir = data[0];
		auto i = atoi(&data[1]);

		movedial(dir == 'L', i);
	}
};


void givendata() {
	auto data = 
R"(L68
L30
R48
L5
R60
L55
L1
L99
R14
L82)";

	day1lock d1l;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line);) {
		d1l.parseline(line);
	}
	
	EXPECT_EQ(d1l.zerocnt, 3);

	EXPECT_EQ(d1l.zerocntpart2, 6);
}

void day1part1() {
	day1lock d1l;

	std::fstream in("E:\\gitrepos\\aoc2025\\src\\input1.txt", std::ios_base::in | std::ios_base::binary);
	for (std::string line; std::getline(in, line);) {
		d1l.parseline(line);
	}

	SPDLOG_ERROR("day1 {}", d1l.zerocnt);
	SPDLOG_ERROR("day1p2 {}", d1l.zerocntpart2);
}

/*int main(int argc, char* argv[]) {
	SPDLOG_ERROR("henlo world");
	//givendata();
	day1part1();

	return 0;
}*/
