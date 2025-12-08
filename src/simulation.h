#pragma once

#include <arena.hpp>
#include <event/event.h>
#include <units.hpp>
#include <vector>

namespace sw::assessment
{
	class simulation final
	{
	public:
		using unit_t = std::variant<feature::swordsman, feature::hunter>;

		struct unit_entry final
		{
			unit_t unit;
		};

		class builder final
		{
		public:
			builder& with_arena(uint32_t width, uint32_t height);

			builder& with_swordsman(uint32_t id, uint32_t x, uint32_t y, uint32_t hp, uint32_t strength);

			builder& with_hunter(
				uint32_t id, uint32_t x, uint32_t y, uint32_t hp, uint32_t strength, uint32_t agility, uint32_t range);

			builder& with_march(uint32_t id, uint32_t x, uint32_t y);

			simulation build() const;

		private:
			core::arena _arena;
			std::vector<unit_entry> _units;
			std::vector<io::event> _events;
		};

		static builder create();

		std::vector<io::event> next_tick(size_t& ticker);

	private:
		simulation(core::arena arena, std::vector<unit_entry> units, std::vector<io::event> events);

		std::vector<io::event> tick();

		void remove_dead_units();

		size_t _ticker;
		core::arena _arena;
		std::vector<unit_entry> _units;
		std::vector<io::event> _events;
	};
}
