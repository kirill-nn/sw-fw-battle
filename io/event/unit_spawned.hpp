#pragma once

#include <string>

namespace sw::io
{
	struct unit_spawned final
	{
		constexpr static const char* Name = "UNIT_SPAWNED";

		unit_spawned(const uint32_t id, const uint32_t x, const uint32_t y, const std::string& type) :
				_id(id),
				_x(x),
				_y(y),
				_type(type)
		{}

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", _id);
			visitor.visit("unitType", _type);
			visitor.visit("x", _x);
			visitor.visit("y", _y);
		}

	private:
		uint32_t _id;
		uint32_t _x;
		uint32_t _y;
		std::string _type{};
	};
}
