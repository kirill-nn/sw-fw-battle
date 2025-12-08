#pragma once

namespace sw::io
{
	struct unit_attacked final
	{
		constexpr static const char* Name = "UNIT_ATTACKED";

		unit_attacked(
			const uint32_t attacker_id, const uint32_t target_id, const uint32_t damage, const uint32_t target_hp) :
				_attacker_id(attacker_id),
				_target_id(target_id),
				_damage(damage),
				_target_hp(target_hp)
		{}

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("attackerUnitId", _attacker_id);
			visitor.visit("targetUnitId", _target_id);
			visitor.visit("damage", _damage);
			visitor.visit("targetHp", _target_hp);
		}

	private:
		uint32_t _attacker_id;
		uint32_t _target_id;
		uint32_t _damage;
		uint32_t _target_hp;
	};
}
