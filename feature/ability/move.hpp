#pragma once

#include <property/destination.h>
#include <property/identity.h>
#include <property/health.h>
#include <property/position.h>

namespace sw::feature
{
	struct move final
	{
		struct step_event final
		{
			uint32_t id, x, y;
		};

		struct destination_event final
		{
			uint32_t id, x, y;
		};

		template<typename Unit, typename Arena, typename Handler>
		bool operator()(Unit& unit, Arena& arena, Handler& handler) const
		{
			if (!core::property<destination>(unit).specified || core::property<health>(unit).hp <= 0)
			{
				return false;
			}

			auto& pos = core::property<position>(unit);
			auto& dest = core::property<destination>(unit);

			if (pos.x == dest.x && pos.y == dest.y)
			{
				return false;
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
					handler(destination_event{core::property<identity>(unit).id, pos.x, pos.y});
				} else
				{
					handler(step_event{core::property<identity>(unit).id, pos.x, pos.y});
				}
				return true;
			}
			return false;
		}
	};
}
