#pragma once

namespace sw::io
{
	struct map_created final
	{
		constexpr static const char* Name = "MAP_CREATED";

		map_created(const uint32_t width, const uint32_t height) :
				_width(width),
				_height(height)
		{}

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("width", _width);
			visitor.visit("height", _height);
		}

	private:
		uint32_t _width;
		uint32_t _height;
	};
}
