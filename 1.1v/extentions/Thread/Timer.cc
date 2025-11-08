export module skbase.thread.Timer;

import skbase.base;

import std;

export namespace Threads{
	class Timer {
	public:
		using clock_t = std::chrono::steady_clock;
		Timer () = default;
		~Timer() {}

		template <typename Duration = ms_t> inline void play(long int time) {
			if (running)
				std::this_thread::sleep_for(std::chrono::duration_cast<ms_t>(Duration(time)));
		}

		inline void stop() { this->running = false; }

	private:
		std::atomic<bool> running{false};
	};
}
