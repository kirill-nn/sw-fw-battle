#pragma once

#include <cstdint>
#include <vector>
#include <variant>

namespace sw::core
{
	template<typename Unit>
	class arena final
	{
	public:
		arena(const uint32_t width = 0, const uint32_t height = 0) :
				_height(height),
				_width(width)
		{}

		void append(Unit&& unit)
		{
			_units.emplace_back(unit);
		}

		bool in_bounds(const uint32_t x, const uint32_t y) const
		{
			return x < _width && y < _height;
		}

		bool finished() const
		{
			return _units.size() <= 1;
		}

		template<typename EventHandler>
		void tick(EventHandler&& handler)
		{
			for (auto& entry : _units)
			{
				std::visit([&](auto& unit) { unit.next_action(*this, handler); }, entry);
			}
		}

		std::vector<Unit>& units()
		{
			return _units;
		}
	private:
		uint32_t _height, _width;
		std::vector<Unit> _units;
	};
}
