#include "functions.h"



SnapshotFile::SnapshotFile(const std::filesystem::path& path, const SearchParametr& param) : rootDir(path), parametrs(param)
{
	namespace fs = std::filesystem; 

	if (!fs::exists(rootDir))
	{
		throw std::invalid_argument("Указанный путь не существует: " + rootDir.string());
	}

	for ()




}