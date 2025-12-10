#pragma once

#include <unit.hpp>

namespace sw::feature
{
	template <typename Arena>
	void remove_dead_unit(Arena& arena, uint32_t id)
	{
		std::erase_if(
			arena.units(),
			[id](auto& unit)
			{ return std::visit([id](auto& u) { return core::property<identity>(u).id == id; }, unit); });
	}
}
