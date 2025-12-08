#pragma once

#include <event/map_created.hpp>
#include <event/march_ended.hpp>
#include <event/march_started.hpp>
#include <event/unit_attacked.hpp>
#include <event/unit_died.hpp>
#include <event/unit_moved.hpp>
#include <event/unit_spawned.hpp>
#include <variant>

namespace sw::io
{
	struct event final
	{
		using event_t
			= std::variant<unit_died, unit_moved, unit_spawned, map_created, unit_attacked, march_started, march_ended>;

		event_t _event;
	};
}
