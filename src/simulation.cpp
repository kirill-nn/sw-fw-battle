#include "simulation.h"

#include <utility>

namespace
{
	class event_adapter final
	{
	public:
		explicit event_adapter(std::vector<sw::io::event>& event_store) :
				_event_store(event_store)
		{}

		template <typename EventType>
		void operator()(const EventType& event) const
		{
			if constexpr (
				std::is_same_v<EventType, sw::feature::close_attack::target_attacked>
				|| std::is_same_v<EventType, sw::feature::range_attack::target_attacked>)
			{
				_event_store.emplace_back(sw::io::unit_attacked{event.id, event.target_id, event.strength, event.hp});
			}
			if constexpr (std::is_same_v<EventType, sw::feature::move::step_event>)
			{
				_event_store.emplace_back(sw::io::unit_moved{event.id, event.x, event.y});
			}
			if constexpr (std::is_same_v<EventType, sw::feature::move::destination_event>)
			{
				_event_store.emplace_back(sw::io::march_ended{event.id, event.x, event.y});
			}
			if constexpr (
				std::is_same_v<EventType, sw::feature::close_attack::target_died>
				|| std::is_same_v<EventType, sw::feature::range_attack::target_died>)
			{
				_event_store.emplace_back(sw::io::unit_died{event.id});
			}
		}

	private:
		std::vector<sw::io::event>& _event_store;
	};
}

sw::assessment::simulation::builder& sw::assessment::simulation::builder::with_arena(
	const uint32_t width, const uint32_t height)
{
	_arena = {width, height};
	_initial_events.emplace_back(io::map_created{width, height});
	return *this;
}

sw::assessment::simulation::builder& sw::assessment::simulation::builder::with_swordsman(
	const uint32_t id, const uint32_t x, const uint32_t y, const uint32_t hp, const uint32_t strength)
{
	_arena.append(
		feature::swordsman{
			feature::identity{id}, feature::position{x, y}, feature::health{hp}, feature::strength{strength}});
	_initial_events.emplace_back(io::unit_spawned{id, x, y, "Swordsman"});
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
	_arena.append(
		feature::hunter{
			feature::identity{id},
			feature::position{x, y},
			feature::health{hp},
			feature::strength{strength},
			feature::agility{agility},
			feature::range{range}});
	_initial_events.emplace_back(io::unit_spawned{id, x, y, "Hunter"});
	return *this;
}

sw::assessment::simulation::builder& sw::assessment::simulation::builder::with_march(
	const uint32_t id, const uint32_t x, const uint32_t y)
{
	for (auto& entry : _arena.units())
	{
		std::visit(
			[&](auto& unit)
			{
				if (core::property<feature::identity>(unit).id == id)
				{
					core::property<feature::destination>(unit).x = x;
					core::property<feature::destination>(unit).y = y;
					core::property<feature::destination>(unit).specified = true;
					_initial_events.emplace_back(
						io::march_started(
							id,
							core::property<feature::position>(unit).x,
							core::property<feature::position>(unit).y,
							x,
							y));
				}
			},
			entry);
	}
	return *this;
}

sw::assessment::simulation sw::assessment::simulation::builder::build() const
{
	return simulation{_arena, _initial_events};
}

sw::assessment::simulation::builder sw::assessment::simulation::create()
{
	return builder{};
}

std::vector<sw::io::event> sw::assessment::simulation::next_tick(size_t& ticker)
{
	if (_arena.finished())
	{
		return {};
	}

	ticker = ++_ticker;
	if (_ticker == 1)
	{
		return _initial_events;
	}
	return tick();
}

sw::assessment::simulation::simulation(core::arena<unit_t> arena, std::vector<io::event> events) :
		_ticker{0},
		_arena{std::move(arena)},
		_initial_events{std::move(events)}
{}

std::vector<sw::io::event> sw::assessment::simulation::tick()
{
	std::vector<io::event> events{};
	event_adapter adapter{events};
	_arena.tick(adapter);
	return events;
}
