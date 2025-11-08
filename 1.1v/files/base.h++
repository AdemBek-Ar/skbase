namespace file {
	std::ofstream  save        (const char*    filename, const char*    src);
	std::string    get_contents(const char*    filename                    );
}

std::ofstream file::save(const char* filename, const char* src) {
	std::ofstream ofs(filename, std::ios::out);
	ofs << src;
	ofs.close();
	return ofs;
}
