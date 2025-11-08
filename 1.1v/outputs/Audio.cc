module;

#include <SFML/Audio.hpp>
#include <filesystem>
#include <chrono>
#include <memory>
#include <vector>
#include <stdexcept>

export module skbase.output.audio;

import skbase.base;
import std;

export namespace Audio{
	struct WavHeader {
		char riffID[4];
		std::uint32_t size;
		char waveID[4];
		char fmtID[4];
		std::uint32_t fmtSize;
		std::uint16_t format;
		std::uint16_t channels;
		std::uint32_t sampleRate;
		std::uint32_t byteRate;
		std::uint16_t blockAlign;
		std::uint16_t bitsPerSample;
		char dataID[4];
		std::uint32_t dataSize;
	};

	struct Wave_Info {
		std::size_t start_time;
		std::size_t end_time;
		bool loop;
		float volume;
		Wave_Info(std::size_t start = 0, std::size_t end = 0, bool loop = false, float volume = 100.0f)
			: start_time(start), end_time(end), loop(loop), volume(volume) {}
		inline void operator= (const Wave_Info& wave) {
			this->start_time = wave.start_time;
			this->end_time = wave.end_time;
			this->loop = wave.loop;
			this->volume = wave.volume;
		}
	};

	class AudioPlayer : public sf::Music, public Wave_Info {
	private:
		std::unique_ptr<sf::SoundBuffer> buffer;
		std::unique_ptr<sf::Sound> sound;
		bool is_range_playing;
		bool using_music;

	public:
		using sf::Music::Music;

		inline void sfplay() { static_cast<sf::Music*>(this)->play(); }

		AudioPlayer(const std::filesystem::path& file_path)
			: using_music(true), is_range_playing(false) {
			if (!this->openFromFile(file_path.string())) {
				std::println("\e[1;31mRun Time Error\e[0m: Faild to load data from file '"+file_path.string()+"'");
				return;
			}
		}
		
		AudioPlayer(const std::byte* data, std::size_t size) 
			: using_music(false), is_range_playing(false) {
			buffer = std::make_unique<sf::SoundBuffer>();
			const sf::Int16* samples = reinterpret_cast<const sf::Int16*>(data);
			std::size_t sample_count = size / sizeof(sf::Int16);
			
			if (!buffer->loadFromSamples(samples, sample_count, 1, 44100)) {
				std::println("\e[1;31mRun Time Error\e[0m: Faild to load data from memory ["+std::to_string(data)+"]");
				return;
			}
			sound = std::make_unique<sf::Sound>(*buffer);
		}

		template<bool set=true>
		void play() {
			is_range_playing = false;
			if (using_music) {
				if constexpr (set) {
					this->setPlayingOffset(sf::milliseconds(this->start_time));
					this->setVolume(this->volume);
					this->setLoop(this->loop);
				} sfplay();
			}
			else {
				if constexpr (set) {
					sound->setPlayingOffset(sf::milliseconds(this->start_time));
					sound->setVolume(this->volume);
					sound->setLoop(this->loop);
				} sound->play();
			}
		}

		template <typename time_unit_t = ms>
		void play(std::size_t start, std::size_t end) {
			this->start_time = start;
			this->end_time = end;
			this->play<true>();
		}

		void play(const Wave_Info& info) {
			this->operator=(info);
			is_range_playing = true;
			if (using_music) {
				this->setPlayingOffset(sf::milliseconds(this->start_time));
				this->setVolume(this->volume);
				this->setLoop(this->loop);
				sfplay();
			} else {
				sound->setPlayingOffset(sf::milliseconds(this->start_time));
				sound->setVolume(this->volume);
				sound->setLoop(this->loop);
				sound->play();
			}
		}

		void stop() {
			is_range_playing = false;
			if (using_music)
				this->stop();
			else
				sound->stop();
		}

		auto get_time() {
			if (using_music)
				return this->getPlayingOffset().asMilliseconds();
			else
				return sound->getPlayingOffset().asMilliseconds();
		}

		void update() {
			if (is_range_playing && current_info.end_time > 0) {
				auto current_time = get_time();
				
				if (current_time >= current_info.end_time) {
					if (current_info.loop) {
						if (using_music)
							this->setPlayingOffset(sf::milliseconds(current_info.start_time));
						else
							sound->setPlayingOffset(sf::milliseconds(current_info.start_time));
					} else stop();
				}
			}
		}

		bool is_playing() const {
			if (using_music)
				return this->getStatus() == sf::Music::Playing;
			else
				return sound->getStatus() == sf::Sound::Playing;
		}
		
		void pause() {
			if (using_music)
				this->pause();
			else
				sound->pause();
		}
		
		void set_volume(float volume)
			if (using_music)
				this->setVolume(volume);
			else
				sound->setVolume(volume);
		}
		float get_volume() const {
			if (using_music)
				return this->getVolume();
			else
				return sound->getVolume();
		}
		
		auto get_duration() {
			if (using_music)
				return this->getDuration().asMilliseconds();
			else
				return buffer->getDuration().asMilliseconds();
		}
	};
}
