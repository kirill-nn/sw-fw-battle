#pragma once

namespace sw::io
{
	struct create_map final
	{
		constexpr static const char* Name = "CREATE_MAP";

		uint32_t width{};
		uint32_t height{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("width", width);
			visitor.visit("height", height);
		}
	};
}
