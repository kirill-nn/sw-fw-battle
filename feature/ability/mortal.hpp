#pragma once

#include <property/health.h>

namespace sw::feature
{
	struct mortal final
	{
		template <class Unit>
		bool invoke(Unit& unit) const
		{
			return core::property<health>(unit).hp <= 0;
		}
	};
}
