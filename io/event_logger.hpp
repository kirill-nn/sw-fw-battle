#pragma once

#include <iosfwd>
#include <iostream>

namespace sw::io
{
	class event_logger final
	{
	public:
		template <class TEvent>
		void log(const uint64_t tick, TEvent&& event)
		{
			using EventType = std::decay_t<TEvent>;
			std::cout << "[" << tick << "] " << EventType::Name << " ";
			stream_visitor visitor(std::cout);
			event.visit(visitor);
			std::cout << std::endl;
		}

	private:
		class stream_visitor final
		{
		public:
			explicit stream_visitor(std::ostream& stream) :
					_stream(stream)
			{}

			template <typename T>
			void visit(const char* name, const T& value)
			{
				_stream << name << "=" << value << ' ';
			}

		private:
			std::ostream& _stream;
		};
	};
}
