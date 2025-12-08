#pragma once

#include <functional>
#include <sstream>
#include <string>

namespace sw::io
{
	class command_parser final
	{
	public:
		template <typename TCommandData>
		command_parser& add(std::function<void(TCommandData)> handler)
		{
			std::string commandName = TCommandData::Name;
			if (const auto it = _commands.find(commandName); it == _commands.end())
			{
				_commands.emplace(
					commandName,
					[handler = std::move(handler)](std::istream& stream)
					{
						TCommandData data;
						visitor visitor(stream);
						data.visit(visitor);
						handler(std::move(data));
					});
			}
			else
			{
				throw std::runtime_error("Command already exists: " + commandName);
			}
			return *this;
		}

		void parse(std::istream& stream);

	private:
		class visitor final
		{
		public:
			explicit visitor(std::istream& stream) :
					_stream(stream)
			{}

			template <class TField>
			void visit(const char*, TField& field)
			{
				_stream >> field;
			}

		private:
			std::istream& _stream;
		};

		std::unordered_map<std::string, std::function<void(std::istream&)>> _commands;
	};
}
