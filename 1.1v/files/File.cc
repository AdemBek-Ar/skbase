module;

#include <cstdio>

export module skbase.files.File;

import skbase.memory.array;
import skbase.str.String;
import std;

export
#include "base.h++"

namespace fs = std::filesystem;

extern "C" {
	const char* ___ucc_rust_get_executable_dir___();
	void free_rust_string(const char*);
}

export namespace rust {
	fs::path get_this_app_dir() {
		auto path_txt = ___ucc_rust_get_executable_dir___();
		fs::path ret(std::move(path_txt));
		free_rust_string(path_txt);
		return ret;
	};
}

std::string file::get_contents(const char* filename) {
	std::ifstream ifs;
	ifs.open(filename, std::ios::in);
	str_t ret = "", line = "";
	while (std::getline(ifs, line)) ret += line + '\n';
	ifs.close();
	return ret;
}



#include "../defines.h++"



export namespace file{
//template<typename file_t> concept is_filetype_v


	class File {
	protected:
		mutable FILE* handle_ = nullptr;
		mutable str_t mode_;

		bool open(const char* mode = "rb") {
			close();
			handle_ = std::fopen(path.string().c_str(), mode);
			if (handle_) mode_ = mode;
			return this->is_open();
		}
		void close() noexcept {
			if (handle_) { 
				std::fclose(handle_); 
				handle_ = nullptr;
				mode_.clear();
			}
		}
	public :
		std::filesystem::path path;

		File(str_t p)    : path(std::move(p).c_str()) { this->open(); }
		File(fs::path p) : path(std::move(p))         { this->open(); }
		inline ~File()                         { close(); }

		File(File&& other) noexcept 
			: path(std::move(other.path)),
			handle_(other.handle_),
			mode_(std::move(other.mode_))
		{
			other.handle_ = nullptr;
			this->open();
		}

		File& operator=(const File&) = delete;
		File(const File&) = delete;
		File& operator=(File&& other) noexcept {
			if (this != &other) {
				close();
				path = std::move(other.path);
				handle_ = other.handle_;
				mode_ = std::move(other.mode_);
				other.handle_ = nullptr;
			} return *this;
		}

		operator bool() const { return fs::exists(path); }
		inline bool is_open() { return bool(handle_); }

		str_t extension() const
			{ return path.extension().string(); }
		str_t name() const
			{ return path.filename().string(); }
		str_t stem() const
			{ return path.stem().string(); }

		bool write(const str_t& text, bool append = false) {
			auto mode = append ? std::ios::app : std::ios::trunc;
			std::ofstream file(path, mode | std::ios::binary);
			if (!file) return false;

			file.write(text.data(), text.size());
			return file.good();
		}

		inline File& operator<<(const str_t& text) {
			write(text);
			return *this;
		}
		inline bool append(const str_t& text)
			{ return write(text, true); }

		std::optional<str_t> txt() const {
			std::ifstream file(path, std::ios::binary | std::ios::ate);
			if (!file) return std::nullopt;

			auto size = file.tellg();
			if (size < 0) return std::nullopt;

			str_t data(static_cast<size_t>(size), '\0');
			file.seekg(0);
			file.read(data.data(), size);

			return data;
		}

		std::optional<Arrays::Array<str_t>> lines() const {
			std::ifstream file(path);
			if (!file) return std::nullopt;

			Arrays::Array<str_t> result;
			str_t line;
			while (std::getline(file, line)) {
				result.push_back(std::move(line));
			}
			return result;
		}
		std::optional<str_t> line(unsigned int i=1) const {
			std::ifstream file(path);
			if (!file) return std::nullopt;

			unsigned int line_in=0;
			str_t line;
			while (std::getline(file, line))
				if (line_in++ >= i) break;
			return line;
		}

		bool rm() {
			close();
			std::error_code ec;
			fs::remove(path, ec);
			return !ec;
		}

		bool mv(const fs::path& new_path) {
			close();
			std::error_code ec;
			fs::rename(path, new_path, ec);
			if (!ec) path = new_path;
			return !ec;
		}

		std::uintmax_t len() const noexcept {
			std::error_code ec;
			return fs::file_size(path, ec);
		}

		std::pair<bool, File> cp(const fs::path& new_path) const {
			std::error_code ec;
			fs::copy_file(path, new_path, fs::copy_options::overwrite_existing, ec);
			return {!ec, File(new_path)};
		}

		void print() const {
			if (auto data = this->txt()) {
				std::cout.write(data->data(), data->size());
				std::cout.flush();
			}
		}

		std::optional<str_t> slice(std::size_t start_pos, std::size_t end_pos) const {
			if (start_pos >= end_pos) return std::nullopt;

			std::ifstream file(path, std::ios::binary);
			if (!file) return std::nullopt;

			file.seekg(start_pos);
			if (!file) return std::nullopt;

			std::size_t size = end_pos - start_pos;
			str_t part(size, '\0');
			file.read(part.data(), size);

			return part;
		}

		template <typename str_T=str_t> requires (
			std::is_same_v<str_T, str_t> or
			std::is_same_v<str_T, const char*> or
			std::is_same_v<str_T, std::string> or
			std::is_same_v<str_T, fs::path>)
		File operator/ (const str_T& name)
			{ return File(this->this_folder_path() / (std::string)name); }

		std::optional<str_t> operator()(std::size_t start_pos, std::size_t end_pos) const
			{ return slice(start_pos, end_pos); }
		std::optional<std::chrono::system_clock::time_point> last_modified() const {
			std::error_code ec;
			auto ftime = fs::last_write_time(path, ec);
			if (ec) return std::nullopt;
			auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
			ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
			);
			return sctp;
		}

		const fs::path& get_path() const noexcept { return this->path; }
		      fs::path full_path() const  { return fs::absolute(path); }

		static File in_app_dir(const fs::path& relative)
			{ return rust::get_this_app_dir() / relative; }

	private:
		fs::path this_folder_path()
			{ return fs::path(str_t(this->path.string()).without<'e', -1>("/")); }
	};
}
