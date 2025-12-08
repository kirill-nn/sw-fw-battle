#pragma once

namespace sw::io
{
	struct march_started final
	{
		constexpr static const char* Name = "MARCH_STARTED";

		march_started(
			const uint32_t id, const uint32_t x, const uint32_t y, const uint32_t target_x, const uint32_t target_y) :
				_id(id),
				_x(x),
				_y(y),
				_target_x(target_x),
				_target_y(target_y)
		{}

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", _id);
			visitor.visit("x", _x);
			visitor.visit("y", _y);
			visitor.visit("targetX", _target_x);
			visitor.visit("targetY", _target_y);
		}

	private:
		uint32_t _id;
		uint32_t _x;
		uint32_t _y;
		uint32_t _target_x;
		uint32_t _target_y;
	};
}
