#pragma once

#include <abilities.h>
#include <properties.h>
#include <unit.hpp>

namespace sw::feature
{
	using swordsman = core::unit_base<
		core::abilities<move, close_attack, mortal>,
		core::properties<identity, position, destination, health, strength>>;

	using hunter = core::unit_base<
		core::abilities<move, close_attack, range_attack, mortal>,
		core::properties<identity, position, destination, health, strength, agility, range>>;
}
