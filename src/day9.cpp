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
#include <unordered_map>

#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

#include <geom/Polygon.h>
#include <geom/LinearRing.h>
#include <geom/GeometryFactory.h>

struct d9pos {
	int64_t x{};
	int64_t y{};
	constexpr bool operator==(const d9pos& b) const {
		return x == b.x && b.y == y;
	}
	constexpr auto operator<=>(const d9pos& b) const = default;
};

struct d9area {
	d9pos a{};
	d9pos b{};
	int64_t size{};
	constexpr bool operator==(const d9area& _b) const {
		return _b.a == a && _b.b == b || _b.b == a && _b.a == b;
	}
	constexpr auto operator<=>(const d9area& b) const = default;

	int64_t x() const {
		return std::min(a.x, b.x);
	}
	int64_t y() const {
		return std::min(a.y, b.y);
	}
	int64_t height() const {
		return std::abs(a.y - b.y);
	}
	int64_t width() const {
		return std::abs(a.x - b.x);
	}
	bool contains(const d9area& _b) const {
		return x() <= _b.x() && y() <= _b.y() && x() + width() >= _b.x() + _b.width() && y() + height() >= _b.y() + _b.height();
		//rect1.x <= rect2.x && rect1.y <= rect2.y && rect1.x + rect1.width >= rect2.x + rect2.width && rect1.y + rect1.height >= rect2.y + rect2.height
	}
};


namespace std {
	template <>
	struct hash<d9pos> {
		std::size_t operator()(const d9pos& key) const {
			return std::hash<int64_t>()(key.x) ^ std::hash<int64_t>()(key.y);
		}
	};

	template <>
	struct hash<d9area> {
		std::size_t operator()(const d9area& key) const {
			return std::hash<d9pos>()(key.a) ^ std::hash<d9pos>()(key.b);
		}
	};
} // namespace std

struct day9 {
	std::vector<d9pos> data;
	std::vector<d9area> areas;
	std::unordered_map<d9area, d9area> areamap;

	std::mutex _lock;
	std::vector<int64_t> _areas;
	std::vector<d9area> _validareas;

	void parseline(std::string& line) {
		auto fmt = std::string{ "%d,%d" };
		d9pos pos{};
		auto ret = std::sscanf(line.c_str(), fmt.c_str(), &pos.x, &pos.y);
		data.push_back(pos);
	}

	uint64_t getp1() {
		uint64_t maxarea = 0;
		for (const auto& a : data) {
			for (const auto& b: data) {
				auto width = std::abs(a.x - b.x + 1);
				auto height = std::abs(a.y - b.y + 1);
				maxarea = std::max(maxarea, (uint64_t)width * height);
			}
		}
		return maxarea;
	}

	bool validpoint(int x, int y) {
		/*bool valid = false;
		for (int i = 0, j = data.size() - 1; i < data.size(); j = i++) {
			if ((data[i].y > y) != (data[j].y > y) && x < (data[j].x - data[i].x) * (y - data[i].y) / (data[j].y - data[i].y) + data[i].x)
				valid = !valid;

		}
		return valid;*/

		auto between = [](int p, int a, int b) { return p >= a && p <= b || p <= a && p >= b; };

		bool inside = false;
		for (int i = data.size() - 1, j = 0; j < data.size(); i = j, j++) {
			const auto& a = data[i];
			const auto& b = data[j];

			if (x == a.x && y == a.y || x == b.x && y == b.y)
				return true;
			if (a.y == b.y && y == a.y && between(x, a.x, b.x)) 
				return true;
			if (between(y, a.y, b.y)) {
				if (y == a.y && b.y >= a.y || y == b.y && a.y >= b.y)
					continue;
				auto c = (a.x - x) * (b.y - y) - (b.x - x) * (a.y - y);
				if (!c)
					return true;
				if ((a.y < b.y) == (c > 0))
					inside = !inside;
			}
		}
		return inside;
	}


	void task(int start, int end) {
		for (int i = start; i < end; ++i) {
			if (i >= areas.size())
				return;

			const auto& a = areas[i];
			auto s = a.size;

			int startx = std::min(a.a.x, a.b.x);
			int endx = std::max(a.a.x, a.b.x);

			int starty = std::min(a.a.y, a.b.y);
			int endy = std::max(a.a.y, a.b.y);

			bool valid = false;
			for (int x = startx; x <= endx; ++x) {
				for (int y = starty; y <= endy; ++y) {
					valid = validpoint(x, y);
					if (!valid)
						break;
				}
				if (!valid)
					break;
			}
			if (valid) {
				auto l = std::lock_guard{ _lock };
				_areas.push_back(a.size);
				return;
			}
		}
	}

	uint64_t getp2() {
		for (const auto& a : data) {
			for (const auto& b : data) {
				if (a.x == b.x || a.y == b.y)
					continue;

				auto width = std::abs(a.x - b.x) + 1;
				auto height = std::abs(a.y - b.y) + 1;

				d9area d9a{ a, b, width * height };
				//areas.push_back(d9a);
				areamap[d9a] = d9a;
			}
		}
		for (const auto& a : areamap) {
			areas.push_back(a.second);
		}

		std::sort(areas.begin(), areas.end(), [](const auto& a, const auto& b) {
			return a.size > b.size;
			});

		
		auto f = geos::geom::GeometryFactory::create();
		auto cs = geos::geom::CoordinateSequence();
		for (const auto& d : data) {
			cs.add(geos::geom::Coordinate(d.x, d.y));
		}
		cs.add(geos::geom::Coordinate(data[0].x, data[0].y));
		auto poly = f->createPolygon(std::move(cs));


		for (const auto& a : areas) {
			cs = {};
			cs.add(geos::geom::Coordinate(a.x(), a.y()));
			cs.add(geos::geom::Coordinate(a.x() + a.width(), a.y()));
			cs.add(geos::geom::Coordinate(a.x() + a.width(), a.y() + a.height()));
			cs.add(geos::geom::Coordinate(a.x(), a.y() + a.height()));
			cs.add(geos::geom::Coordinate(a.x(), a.y()));
			auto poly2 = f->createPolygon(std::move(cs));
			if (poly->contains(poly2->getGeometryN(0)))
				return a.size;
		}

		/*std::vector<std::thread> workers;
		auto range = ((areas.size()) / 32) + 1;
		if (areas.size() > 1000) {
			range = 0;
			for (int i = 0; i < 32; ++i) {
				workers.push_back(std::thread([=]() { this->task(range, range+100); }));
				range += 100;
			}
		}
		else {
			for (int i = 0; i < 32; ++i) {
				workers.push_back(std::thread([=]() { this->task(i * range, i * range + range); }));
			}
		}

		for (std::thread& t : workers) {
			if (t.joinable())
				t.join();
		}

		assert(_areas.size());

		std::sort(_areas.begin(), _areas.end(), [](const auto& a, const auto& b) {return a > b; });

		return _areas[0];*/

		/*std::sort(areas.begin(), areas.end(), [](const auto& a, const auto& b) {return a.size < b.size; });

		for (int i = 0; const auto& a : areas) {
			i++;
			int startx = std::min(a.a.x, a.b.x);
			int endx = std::max(a.a.x, a.b.x);

			int starty = std::min(a.a.y, a.b.y);
			int endy = std::max(a.a.y, a.b.y);

			bool valid = false;

			for (int x = startx; x <= endx; ++x) {
				for (int y = starty; y <= endy; ++y) {
					bool skipped = false;
					for (const auto& va : _validareas) {
						auto vasx = std::min(va.a.x, va.b.x);
						auto vaex = std::max(va.a.x, va.b.x);
						auto vasy = std::min(va.a.y, va.b.y);
						auto vaey = std::max(va.a.y, va.b.y);

						// skip forward
						if ((vasx <= x && vaex >= x) && (vasy <= y && vaey >= y)) {
							//x = vaex;
							y = vaey;
							skipped = true;
							valid = true;
							break;
						}
					}
					if (skipped)
						continue;

					valid = validpoint(x, y);
					if (!valid)
						break;
				}
				if (!valid)
					break;
			}
			if (valid) {
				_validareas.push_back(a);
				//return a.size;
			}
		}

		std::sort(_validareas.begin(), _validareas.end(), [](const auto& a, const auto& b) {return a.size > b.size; });
		return _validareas[0].size;*/
		return 0;
	}
};

void day9test() {
	auto data =
		R"(7,1
11,1
11,7
9,7
9,5
2,5
2,3
7,3)";

	day9 d9;

	std::istringstream input;
	input.str(data);
	for (std::string line; std::getline(input, line);) {
		d9.parseline(line);
	}

	SPDLOG_ERROR("{}", d9.getp1());
	SPDLOG_ERROR("{}", d9.getp2());
}

void day9proc() {
	day9 d9;

	std::fstream in("E:\\gitrepos\\aoc2025\\src\\input9.txt", std::ios_base::in | std::ios_base::binary);
	for (std::string line; std::getline(in, line);) {
		d9.parseline(line);
	}

	SPDLOG_ERROR("{}", d9.getp1());
	SPDLOG_ERROR("{}", d9.getp2());
}

/*int main() {
	day9test();
	day9proc();
	return 0;
}*/