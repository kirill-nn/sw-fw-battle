#pragma once

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

	private:
		template <typename Prop, typename Unit>
		friend auto& property(Unit& u);

		template <typename Ability, typename Unit, typename... Args>
		friend auto use_ability(Unit& unit, Args&&... args);

		template <typename P>
		void set_property(P&& p)
		{
			static_assert(has_property<P>, "Property does not exist!");
			std::get<P>(props) = std::forward<P>(p);
		}

		template <typename Property>
		auto& get()
		{
			static_assert(has_property<Property>, "Property not assigned to this unit!");
			return std::get<Property>(props);
		}

		template <typename Ability, typename... Args>
		decltype(auto) use(Args&&... args)
		{
			static_assert(is_one_of<Ability, Abilities...>::value, "Ability not assigned to this unit!");
			return Ability{}.invoke(*this, std::forward<Args>(args)...);
		}

		std::tuple<Properties...> props;
	};

	template <typename Prop, typename Unit>
	auto& property(Unit& u)
	{
		return u.template get<Prop>();
	}

	template <typename Unit, typename Ability>
	constexpr bool has_ability()
	{
		return Unit::template has_ability<Ability>;
	}

	template <typename Ability, typename Unit, typename... Args>
	auto use_ability(Unit& unit, Args&&... args)
	{
		return unit.template use<Ability>(std::forward<Args>(args)...);
	}
}
