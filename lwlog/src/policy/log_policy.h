#pragma once

#include "level.h"
#include "sinks/sink.h"
#include "details/bounded_queue.h"
#include "details/argument_format.h"
#include "details/source_meta.h"
#include "details/topic_registry.h"

namespace lwlog
{
	inline constexpr std::uint64_t default_thread_affinity{ 0xFFFFFFFFFFFFFFFF };
	inline constexpr std::size_t default_async_queue_size{ 1024 };

	struct synchronous_policy
	{
		template<typename Config, typename ConcurrencyModelPolicy>
		struct backend
		{
			std::vector<sink_ptr> sink_storage;
		};

		template<typename Config, typename ConcurrencyModelPolicy>
		static void init(backend<Config, ConcurrencyModelPolicy>&) {}

		template<typename Config, typename ConcurrencyModelPolicy>
		static void log(backend<Config, ConcurrencyModelPolicy>& backend, 
			const details::topic_registry<typename Config::topic_t>& topic_registry, std::string_view message, 
			level log_level, const details::source_meta& meta, details::format_args_list args);
	};

	template<
		typename OverflowPolicy	= block_overflow_policy,
		std::size_t Capacity = default_async_queue_size,
		std::uint64_t ThreadAffinity = default_thread_affinity
	> 
	struct asynchronous_policy
	{
		template<typename Config, typename ConcurrencyModelPolicy>
		struct backend
		{
			~backend();

			std::vector<sink_ptr> sink_storage;

			std::atomic_bool shutdown;
			std::thread worker_thread;

			struct queue_item;
			details::bounded_queue<
				Capacity, 
				queue_item,
				OverflowPolicy,
				ConcurrencyModelPolicy
			> queue;
		};

		template<typename Config, typename ConcurrencyModelPolicy>
		static void init(backend<Config, ConcurrencyModelPolicy>& backend);

		template<typename Config, typename ConcurrencyModelPolicy>
		static void log(backend<Config, ConcurrencyModelPolicy>& backend, 
			const details::topic_registry<typename Config::topic_t>& topic_registry, std::string_view message, 
			level log_level, const details::source_meta& meta, details::format_args_list args);
	};
}

#include "log_policy_impl.h"