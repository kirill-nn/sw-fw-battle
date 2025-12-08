#pragma once

namespace sw::io
{
	struct march_ended final
	{
		constexpr static const char* Name = "MARCH_ENDED";

		march_ended(const uint32_t id, const uint32_t x, const uint32_t y) :
				_id(id),
				_x(x),
				_y(y)
		{}

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", _id);
			visitor.visit("x", _x);
			visitor.visit("y", _y);
		}

	private:
		uint32_t _id;
		uint32_t _x;
		uint32_t _y;
	};
}
