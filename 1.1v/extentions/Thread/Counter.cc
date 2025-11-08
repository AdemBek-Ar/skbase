export module skbase.thread.Counter;

import std;

export namespace Threads{
	class Counter {
	public : using clock_t = std::chrono::steady_clock;
		Counter(clock_t::time_point tp) : start_time(tp			) {}
		Counter(					  ) : start_time(clock_t::now()) {}

		inline void reset(clock_t::time_point tp) { start_time = tp			; }
		inline void reset(					  ) { start_time = clock_t::now(); }

		template <typename Duration = std::chrono::milliseconds>
		inline long int get_time() 
		  { return std::chrono::duration_cast<Duration>(clock_t::now() - start_time).count(); }
	private: clock_t::time_point start_time;
	};
}
