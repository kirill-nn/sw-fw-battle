#pragma once

#include <property/destination.h>
#include <property/health.h>
#include <property/position.h>

namespace sw::feature
{
	struct move final
	{
		enum class state
		{
			movable,
			edge_reached,
			destination_reached,
			immovable
		};

		template <class Unit>
		state invoke(Unit& unit, core::arena& arena) const
		{
			if (!core::property<destination>(unit).specified)
			{
				return state::immovable;
			}

			if (core::property<health>(unit).hp <= 0)
			{
				return state::immovable;
			}

			auto& pos = core::property<position>(unit);
			auto& dest = core::property<destination>(unit);

			if (pos.x == dest.x && pos.y == dest.y)
			{
				return state::immovable;
			}

			int dx = dest.x > pos.x ? +1 : (dest.x < pos.x ? -1 : 0);
			int dy = dest.y > pos.y ? +1 : (dest.y < pos.y ? -1 : 0);

			int new_x = pos.x + dx;
			int new_y = pos.y + dy;

			if (arena.in_bounds(new_x, new_y))
			{
				pos.x = new_x;
				pos.y = new_y;
				if (pos.x == dest.x && pos.y == dest.y)
				{
					return state::destination_reached;
				}
				return state::movable;
			}
			return state::edge_reached;
		}
	};
}
