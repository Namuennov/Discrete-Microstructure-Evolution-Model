#pragma once
#include <unordered_map>
#include <algorithm>

struct coordinates
{
	unsigned int x;
	unsigned int y;
	unsigned int z;

	coordinates(unsigned int _x, unsigned int _y, unsigned int _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	bool operator==(const coordinates& c) const { return (x == c.x && y == c.y && z == c.z); }
};

template <class T> inline void hash_combine(std::size_t& s, const T& v)
{
	std::hash<T> h;
	s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

template <> struct std::hash<coordinates>
{
	size_t operator()(const coordinates& c) const noexcept {
		std::size_t result = 0;
		hash_combine(result, c.x);
		hash_combine(result, c.y);
		hash_combine(result, c.z);
		return result;
	}
};