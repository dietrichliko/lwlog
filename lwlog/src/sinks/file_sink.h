﻿#pragma once

#include "sink.h"
#include "details/file_writer.h"

namespace lwlog::sinks
{
	template<typename ThreadingPolicy>
	class file_sink 
		: public sink<uncolored_policy, ThreadingPolicy>
		, private details::file_writer
	{
	private:
		using sink_t = sink<uncolored_policy, ThreadingPolicy>;

	public:
		file_sink() = default;
		explicit file_sink(std::string_view path);

	public:
		void sink_it(const details::record& record) override;
	};

	template<typename ThreadingPolicy>
	file_sink<ThreadingPolicy>::file_sink(std::string_view path)
		: details::file_writer(path) 
	{}

	template<typename ThreadingPolicy>
	void file_sink<ThreadingPolicy>::sink_it(const details::record& record)
	{
        sink_t::m_current_level = record.level;
		details::file_writer::write(sink_t::m_pattern.compile(record));
	}
}