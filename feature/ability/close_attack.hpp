#pragma once

#include <property/health.h>
#include <property/identity.h>
#include <property/position.h>
#include <property/strength.h>
#include <ability/common.hpp>
#include <unit.hpp>

namespace sw::feature
{
	struct close_attack final
	{
		struct target_attacked final
		{
			uint32_t id, target_id, strength, hp;
		};

		struct target_died final
		{
			uint32_t id;
		};

		template <typename Unit, typename Arena, typename Handler>
		bool operator()(Unit& attacker, Arena& arena, Handler& handler) const
		{
			auto& pos = core::property<position>(attacker);
			bool attack_happened = false;
			std::optional<uint32_t> dead_target{};
			for (auto& unit : arena.units())
			{
				std::visit(
					[&](auto& target)
					{
						const auto att_id = core::property<identity>(attacker).id;
						const auto target_id = core::property<identity>(target).id;
						if (att_id == target_id)
						{
							return;
						}
						if (core::property<health>(attacker).hp <= 0 || core::property<health>(target).hp <= 0)
						{
							return;
						}
						auto& tpos = core::property<position>(target);

						if (chebyshev_distance(pos, tpos) <= 1)
						{
							auto dmg = core::property<strength>(attacker).sp;
							auto& hp = core::property<health>(target).hp;
							if (dmg > hp)
							{
								hp = 0;
							}
							else
							{
								hp -= dmg;
							}
							attack_happened = true;
							handler(target_attacked{att_id, target_id, dmg, hp});
							if (hp == 0)
							{
								handler(target_died{target_id});
								dead_target = target_id;
							}
						}
					},
					unit);
				if (attack_happened)
				{
					break;
				}
			}
			if (dead_target.has_value())
			{
				remove_dead_unit(arena, dead_target.value());
			}
			return attack_happened;
		}
	};
}
