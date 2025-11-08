module;

#include <cstdio>

export module skbase.files.Folder;

import skbase.memory.array;
import skbase.str.String;
import skbase.files.File;
import std;

namespace fs = std::filesystem;

export namespace file{
	class Folder {
	public:
		fs::path path;

		Folder(str_t p)	   : path(std::move(p).c_str()) {}
		Folder(fs::path p) : path(std::move(p)) {}

		operator bool() const noexcept
			{ return fs::exists(path) && fs::is_directory(path); }

		fs::path full_path() const { return fs::absolute(path); }
		const fs::path& get_path() noexcept { return path; }

		bool mk() const {
			std::error_code ec;
			fs::create_directories(path, ec);
			return !ec;
		}

		bool rm() const {
			std::error_code ec;
			fs::remove_all(path, ec);
			return !ec;
		}

		bool mv(const fs::path& new_path) {
			std::error_code ec;
			fs::rename(path, new_path, ec);
			if (!ec) path = new_path;
			return !ec;
		}

		std::pair<bool, Folder> cp(const fs::path& new_path) const {
			std::error_code ec;
			fs::copy(path, new_path, fs::copy_options::recursive | fs::copy_options::overwrite_existing, ec);
			return {!ec, Folder(new_path)};
		}

		std::size_t files_count() const {
			std::size_t count = 0;
			std::error_code ec;
			for (auto& entry : fs::directory_iterator(path, ec))
				if (entry.is_regular_file()) ++count;
			return count;
		}
				std::size_t folders_count() const {
			std::size_t count = 0;
			std::error_code ec;
			for (auto& entry : fs::directory_iterator(path, ec))
				if (entry.is_directory()) ++count;
			return count;
		}

		std::uintmax_t total_size() const {
			std::uintmax_t total = 0;
			std::error_code ec;
			for (auto& entry : fs::recursive_directory_iterator(path, ec))
				if (entry.is_regular_file())
					total += fs::file_size(entry.path(), ec);
			return total;
		}

		Arrays::Array<File> get_files(bool recursive = false) const {
			Arrays::Array<File> files;
			std::error_code ec;
						if (recursive) {
				for (auto& entry : fs::recursive_directory_iterator(path, ec))
					if (entry.is_regular_file())
						files.emplace_back(entry.path());
			} else
				for (auto& entry : fs::directory_iterator(path, ec))
					if (entry.is_regular_file())
						files.emplace_back(entry.path());
			return files;
		}

		Arrays::Array<str_t> get_files_names(bool full_path = false, bool recursive = false) const {
			Arrays::Array<str_t> result;
			std::error_code ec;
						auto process = [&](const fs::directory_entry& entry) {
				if (entry.is_regular_file())
					if (full_path)
						result.push_back(entry.path().string());
					else
						result.push_back(entry.path().filename().string());
			};

			if (recursive)
				for (auto& entry : fs::recursive_directory_iterator(path, ec))
					process(entry);
			else
				for (auto& entry : fs::directory_iterator(path, ec))
					process(entry);
			return result;
		}

		Arrays::Array<Folder> get_folders() const {
			Arrays::Array<Folder> folders;
			std::error_code ec;
			for (auto& entry : fs::directory_iterator(path, ec))
				if (entry.is_directory())
					folders.emplace_back(entry.path());
			return folders;
		}

		Arrays::Array<File> find_by_extension(const str_t& ext, bool recursive = false) const {
			Arrays::Array<File> result;
			std::error_code ec;
			auto process = [&](const fs::directory_entry& entry) {
				if (entry.is_regular_file() && entry.path().extension() == (std::string)ext)
					result.emplace_back(entry.path());
			};
			if (recursive)
				for (auto& entry : fs::recursive_directory_iterator(path, ec))
					process(entry);
			else
				for (auto& entry : fs::directory_iterator(path, ec))
					process(entry);
			return result;
		}

		Arrays::Array<File> find_by_name(const str_t& pattern, bool recursive = false) const {
			Arrays::Array<File> result;
			std::error_code ec;
						auto process = [&](const fs::directory_entry& entry) {
				if (entry.is_regular_file()) {
					auto filename = entry.path().filename().string();
					if (filename.find(pattern) != str_t::npos)
						result.emplace_back(entry.path());
				}
			};
			if (recursive)
				for (auto& entry : fs::recursive_directory_iterator(path, ec))
					process(entry);
			else
				for (auto& entry : fs::directory_iterator(path, ec))
					process(entry);
			return result;
		}

		str_t extension() const
			{ return path.extension().string(); }
		str_t name() const
			{ return path.filename().string(); }
		str_t stem() const
			{ return path.stem().string(); }

		template <typename str_T=str_t> requires (
			std::is_same_v<str_T, str_t> or
			std::is_same_v<str_T, const char*> or
			std::is_same_v<str_T, std::string> or
			std::is_same_v<str_T, fs::path>)
		File   operator[](const str_T& name) const
			{ return File  (path / (std::string)name); }
		template <typename str_T=str_t> requires (
			std::is_same_v<str_T, str_t> or
			std::is_same_v<str_T, const char*> or
			std::is_same_v<str_T, std::string> or
			std::is_same_v<str_T, fs::path>)
		Folder operator/ (const str_t& name) const
			{ return Folder(path / (std::string)name); }


		bool clear() const {
			std::error_code ec;
			for (auto& entry : fs::directory_iterator(path, ec)) {
				fs::remove_all(entry.path(), ec);
				if (ec) return false;
			} return true;
		}

		static Folder app_dir()
			{ return rust::get_this_app_dir(); }
	};
}
