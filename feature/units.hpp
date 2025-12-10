#pragma once

#include <abilities.h>
#include <properties.h>
#include <unit.hpp>

namespace sw::feature
{
	using swordsman = core::unit_base<
		core::abilities<close_attack, move>,
		core::properties<identity, position, destination, health, strength>>;

	using hunter = core::unit_base<
		core::abilities<close_attack, range_attack, move>,
		core::properties<identity, position, destination, health, strength, agility, range>>;
}
