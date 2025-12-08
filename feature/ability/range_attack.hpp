#pragma once

#include <property/agility.h>
#include <property/health.h>
#include <property/position.h>
#include <property/range.h>

namespace sw::feature
{
	struct range_attack final
	{
		template <typename Unit, typename Target>
		bool invoke(Unit& attacker, Target& target) const
		{
			if (core::property<health>(attacker).hp <= 0 || core::property<health>(target).hp <= 0)
			{
				return false;
			}

			auto& att_pos = core::property<position>(attacker);
			auto& tar_pos = core::property<position>(target);
			auto& attack_range = core::property<range>(attacker).value;

			if (int dist = chebyshev_distance(att_pos, tar_pos); dist >= 2 && dist <= attack_range)
			{
				auto& dmg = core::property<agility>(attacker).damage;
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
