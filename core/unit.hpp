#pragma once

#include <tuple>

namespace sw::core
{
	template <typename T, typename... List>
	struct is_one_of : std::disjunction<std::is_same<T, List>...>
	{};

	template <typename...>
	struct abilities final
	{};

	template <typename...>
	struct properties final
	{};

	template <typename Abilities, typename Properties>
	struct unit_base final
	{};

	template <typename... Abilities, typename... Properties>
	struct unit_base<abilities<Abilities...>, properties<Properties...>> final
	{
		template <typename Ability>
		static constexpr bool has_ability = is_one_of<Ability, Abilities...>::value;

		template <typename Property>
		static constexpr bool has_property = is_one_of<Property, Properties...>::value;

		template <typename... ProvidedProps>
			requires(sizeof...(ProvidedProps) > 0) && ((has_property<std::decay_t<ProvidedProps>>) && ...)
		explicit unit_base(ProvidedProps&&... provided)
		{
			(set_property(std::forward<ProvidedProps>(provided)), ...);
		}

		template <typename Arena, typename Handler>
		bool next_action(Arena& arena, Handler& handler)
		{
			return ability_lookup(arena, handler);
		}

	private:
		template <std::size_t I = 0, typename Arena, typename Handler>
		bool ability_lookup(Arena& arena, Handler& handler)
		{
			if constexpr (I == sizeof...(Abilities))
			{
				return false;
			}
			else
			{
				if (auto& ability = std::get<I>(_abilities); ability(*this, arena, handler))
				{
					return true;
				}
				return ability_lookup<I + 1>(arena, handler);
			}
		}

		template <typename Prop, typename Unit>
		friend auto& property(Unit& u);

		template <typename P>
		void set_property(P&& p)
		{
			static_assert(has_property<P>, "Property does not exist!");
			std::get<P>(_properties) = std::forward<P>(p);
		}

		template <typename Property>
		auto& get()
		{
			static_assert(has_property<Property>, "Property not assigned to this unit!");
			return std::get<Property>(_properties);
		}

		std::tuple<Properties...> _properties;
		std::tuple<Abilities...> _abilities;
	};

	template <typename Prop, typename Unit>
	auto& property(Unit& u)
	{
		return u.template get<Prop>();
	}
}
