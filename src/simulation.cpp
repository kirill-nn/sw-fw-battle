#include "simulation.h"

sw::assessment::simulation::builder& sw::assessment::simulation::builder::with_arena(
	const uint32_t width, const uint32_t height)
{
	_arena = {width, height};
	_events.emplace_back(io::map_created{width, height});
	return *this;
}

sw::assessment::simulation::builder& sw::assessment::simulation::builder::with_swordsman(
	const uint32_t id, const uint32_t x, const uint32_t y, const uint32_t hp, const uint32_t strength)
{
	_units.emplace_back(
		feature::swordsman{
			feature::identity{id}, feature::position{x, y}, feature::health{hp}, feature::strength{strength}});
	_events.emplace_back(io::unit_spawned{id, x, y, "Swordsman"});
	return *this;
}

sw::assessment::simulation::builder& sw::assessment::simulation::builder::with_hunter(
	const uint32_t id,
	const uint32_t x,
	const uint32_t y,
	const uint32_t hp,
	const uint32_t strength,
	const uint32_t agility,
	const uint32_t range)
{
	_units.emplace_back(
		feature::hunter{
			feature::identity{id},
			feature::position{x, y},
			feature::health{hp},
			feature::strength{strength},
			feature::agility{agility},
			feature::range{range}});
	_events.emplace_back(io::unit_spawned{id, x, y, "Hunter"});
	return *this;
}

sw::assessment::simulation::builder& sw::assessment::simulation::builder::with_march(
	const uint32_t id, const uint32_t x, const uint32_t y)
{
	for (auto& entry : _units)
	{
		std::visit(
			[&](auto& unit)
			{
				if (core::property<feature::identity>(unit).id == id)
				{
					core::property<feature::destination>(unit).x = x;
					core::property<feature::destination>(unit).y = y;
					core::property<feature::destination>(unit).specified = true;
					_events.emplace_back(
						io::march_started{
							id,
							core::property<feature::position>(unit).x,
							core::property<feature::position>(unit).y,
							x,
							y});
				}
			},
			entry.unit);
	}
	return *this;
}

sw::assessment::simulation sw::assessment::simulation::builder::build() const
{
	return simulation{_arena, _units, _events};
}

sw::assessment::simulation::builder sw::assessment::simulation::create()
{
	return builder{};
}

std::vector<sw::io::event> sw::assessment::simulation::next_tick(size_t& ticker)
{
	ticker = _ticker;
	if (_ticker == 1)
	{
		++_ticker;
		return _events;
	}
	return tick();
}

sw::assessment::simulation::simulation(
	core::arena arena, std::vector<unit_entry> units, std::vector<io::event> events) :
		_ticker{1},
		_arena{arena},
		_units{std::move(units)},
		_events{std::move(events)}
{}

void sw::assessment::simulation::remove_dead_units()
{
	std::erase_if(
		_units,
		[](auto& entry)
		{
			return std::visit(
				[](auto& unit)
				{
					using unit_type = std::decay_t<decltype(unit)>;
					if constexpr (core::has_ability<unit_type, feature::mortal>())
					{
						return core::use_ability<feature::mortal>(unit);
					}
					else
					{
						return false;
					}
				},
				entry.unit);
		});
}

std::vector<sw::io::event> sw::assessment::simulation::tick()
{
	if (_units.size() <= 1)
	{
		return {};
	}
	std::vector<io::event> events{};

	for (auto& entry : _units)
	{
		std::visit(
			[&](auto& unit)
			{
				using unit_type = std::decay_t<decltype(unit)>;

				bool made_attack = false;
				if constexpr (core::has_ability<unit_type, feature::close_attack>())
				{
					for (auto& entry_target : _units)
					{
						std::visit(
							[&](auto& target_unit)
							{
								if (&entry == &entry_target)
								{
									return;
								}

								if (core::use_ability<feature::close_attack>(unit, target_unit))
								{
									made_attack = true;
									events.emplace_back(
										io::unit_attacked{
											core::property<feature::identity>(unit).id,
											core::property<feature::identity>(target_unit).id,
											core::property<feature::strength>(unit).sp,
											core::property<feature::health>(target_unit).hp});
									if (core::use_ability<feature::mortal>(target_unit))
									{
										events.emplace_back(
											io::unit_died{core::property<feature::identity>(target_unit).id});
									}
								}
							},
							entry_target.unit);
					}
				}

				if (made_attack == false)
				{
					if constexpr (core::has_ability<unit_type, feature::range_attack>())
					{
						for (auto& entry_target : _units)
						{
							std::visit(
								[&](auto& target_unit)
								{
									if (&entry_target == &entry)
									{
										return;
									}

									if (core::use_ability<feature::range_attack>(unit, target_unit))
									{
										made_attack = true;
										events.emplace_back(
											io::unit_attacked{
												core::property<feature::identity>(unit).id,
												core::property<feature::identity>(target_unit).id,
												core::property<feature::agility>(unit).damage,
												core::property<feature::health>(target_unit).hp});
										if (core::use_ability<feature::mortal>(target_unit))
										{
											events.emplace_back(
												io::unit_died{core::property<feature::identity>(target_unit).id});
										}
									}
								},
								entry_target.unit);
						}
					}
				}

				if (made_attack == false)
				{
					if constexpr (core::has_ability<unit_type, feature::move>())
					{
						if (const auto state = core::use_ability<feature::move>(unit, _arena);
							state == feature::move::state::movable)
						{
							events.emplace_back(
								io::unit_moved{
									core::property<feature::identity>(unit).id,
									core::property<feature::position>(unit).x,
									core::property<feature::position>(unit).y});
						}
						else if (state == feature::move::state::destination_reached)
						{
							events.emplace_back(
								io::unit_moved{
									core::property<feature::identity>(unit).id,
									core::property<feature::position>(unit).x,
									core::property<feature::position>(unit).y});
							events.emplace_back(
								io::march_ended{
									core::property<feature::identity>(unit).id,
									core::property<feature::position>(unit).x,
									core::property<feature::position>(unit).y});
						}
					}
				}
			},
			entry.unit);
	}
	remove_dead_units();
	++_ticker;
	return events;
}
