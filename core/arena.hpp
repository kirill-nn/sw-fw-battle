#pragma once

#include <cstdint>

namespace sw::core
{
	class arena final
	{
	public:
		arena(const uint32_t width = 0, const uint32_t height = 0) :
				_height(height),
				_width(width)
		{}

		bool in_bounds(const uint32_t x, const uint32_t y) const
		{
			return x < _width && y < _height;
		}

		uint32_t height() const
		{
			return _height;
		}

		uint32_t width() const
		{
			return _width;
		}

	private:
		uint32_t _height, _width;
	};
}
