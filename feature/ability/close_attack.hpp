#pragma once

#include <unit.hpp>
#include <property/health.h>
#include <property/position.h>
#include <property/strength.h>

namespace sw::feature
{
	struct close_attack final
	{
		template <typename Unit, typename Target>
		bool invoke(Unit& attacker, Target& target) const
		{
			if (core::property<health>(attacker).hp <= 0 || core::property<health>(target).hp <= 0)
			{
				return false;
			}
			auto& pos = core::property<position>(attacker);
			auto& tpos = core::property<position>(target);

			if (chebyshev_distance(pos, tpos) <= 1)
			{
				auto dmg = core::property<strength>(attacker).sp;
				if (auto& hp = core::property<health>(target).hp; dmg > hp)
				{
					hp = 0;
				}
				else
				{
					hp -= dmg;
				}
				return true;
			}
			return false;
		}
	};
}
