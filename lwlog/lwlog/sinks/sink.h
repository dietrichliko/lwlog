#pragma once

#include <string>

#include "../core.h"

namespace lwlog::sinks
{
	class LWLOG_API sink
	{
	public:
		sink();
		virtual ~sink() = default;

		virtual void sink_it(std::string_view) = 0;

	public:
		void set_pattern(std::string_view pattern);
		std::string get_pattern() const;

	private:
		std::string m_pattern;
	};
}