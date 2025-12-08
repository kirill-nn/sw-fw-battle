#pragma once

namespace sw::io
{
	struct unit_died final
	{
		constexpr static const char* Name = "UNIT_DIED";

		explicit unit_died(const uint32_t id) :
				_id(id)
		{}

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", _id);
		}

	private:
		uint32_t _id;
	};
}
