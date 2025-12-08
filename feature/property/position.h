#pragma once

namespace sw::feature
{
	struct position final
	{
		uint32_t x, y;
	};

	inline uint32_t chebyshev_distance(const position& a, const position& b)
	{
		const uint32_t dx = a.x > b.x ? a.x - b.x : b.x - a.x;
		const uint32_t dy = a.y > b.y ? a.y - b.y : b.y - a.y;
		return std::max(dx, dy);
	}
}
