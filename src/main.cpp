#include "simulation.h"

#include <command_parser.hpp>
#include <commands/create_map.hpp>
#include <commands/march.hpp>
#include <commands/spawn_hunter.hpp>
#include <commands/spawn_swordsman.hpp>
#include <event_logger.hpp>
#include <fstream>

int main(int argc, char** argv)
{
	try
	{
		if (argc != 2)
		{
			throw std::invalid_argument("Error: No file specified in command line argument");
		}

		if (std::ifstream file(argv[1]); file)
		{
			auto builder = sw::assessment::simulation::create();
			sw::io::command_parser parser{};
			parser.add<sw::io::create_map>([&](const sw::io::create_map& command)
										   { builder.with_arena(command.width, command.height); });
			parser.add<sw::io::spawn_hunter>(
				[&](const sw::io::spawn_hunter& command)
				{
					builder.with_hunter(
						command.unitId,
						command.x,
						command.y,
						command.hp,
						command.strength,
						command.agility,
						command.range);
				});
			parser.add<sw::io::spawn_swordsman>(
				[&](const sw::io::spawn_swordsman& command)
				{ builder.with_swordsman(command.unitId, command.x, command.y, command.hp, command.strength); });
			parser.add<sw::io::march>([&](const sw::io::march& command)
									  { builder.with_march(command.unitId, command.targetX, command.targetY); });
			parser.parse(file);

			auto simulation = builder.build();
			sw::io::event_logger logger{};
			size_t ticker = 0;
			for (;;)
			{
				if (auto events = simulation.next_tick(ticker); !events.empty())
				{
					for (auto& e : events)
					{
						std::visit([&](auto&& evt) { logger.log(ticker, evt); }, e._event);
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}
	catch (...)
	{
		std::cerr << "unknown error" << "\n";
	}
}
