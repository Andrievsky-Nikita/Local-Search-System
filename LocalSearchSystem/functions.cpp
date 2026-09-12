#include "functions.h"

template <typename Iterator>
void SnapshotFile::parseFileSpace(Iterator begin, Iterator end, SnapshotFile& obj)
{
	uint32_t indFile = 0;

	for (; begin != end; ++begin)
	{
		const auto& entry = *begin;

		if (entry.is_regular_file() && obj.parametrs.formatFiles.contains(entry.path().extension().string()))
		{
			obj.docIdToPath.push_back(entry.path());
			obj.pathToDocId.emplace(entry.path(), indFile);

			try
			{
				parseFile(entry.path(), obj, indFile);
				obj.badFiles.push_back(false);
			}
			catch (const std::ios_base::failure& ext)
			{
				obj.badFiles.push_back(true);

				for (auto& currPair : obj.invertedIndex)
				{
					if (currPair.second[currPair.second.size() - 1].docId == indFile)
					{
						currPair.second.pop_back();
					}
				}
			}
			catch (const std::runtime_error& ext)
			{
				obj.badFiles.push_back(true);
			}
			
			indFile++;	
		}
	}

	obj.isOutdated = std::vector<bool>(indFile, false);
}

void SnapshotFile::parseFile(const std::filesystem::path& filePath, SnapshotFile& obj, uint32_t indFile)
{
	std::ifstream myFile(filePath);

	if (!myFile.is_open())
	{
		throw std::runtime_error("Файл " + filePath.string() + " не был открыт успешно!");
	}

	myFile.exceptions(std::ios::failbit || std::ios::badbit);

	char c;
	std::string currStr;
	uint32_t countLines = 1;

	while (myFile.get(c))
	{
		if (!isSeparator(c))
		{
			currStr.push_back(c);
		}
		else
		{
			if (currStr.size() != 0)
			{
				if (obj.invertedIndex.contains(currStr))
				{
					if (obj.invertedIndex[currStr][obj.invertedIndex[currStr].size() - 1].docId == indFile)
					{
						std::size_t ind = obj.invertedIndex[currStr].size() - 1;
						obj.invertedIndex[currStr][ind].count++;
						obj.invertedIndex[currStr][ind].lines.push_back(countLines);
					}
					else
					{
						obj.invertedIndex[currStr].push_back(SnapshotFile::MatchInfo(indFile, countLines));
					}
				}
				else
				{
					obj.invertedIndex.emplace(currStr, std::vector<MatchInfo>{MatchInfo(indFile, countLines)});
				}

				currStr.clear();
			}

			if (c == '\n')
			{
				countLines++;
			}
		}
	}
}

SnapshotFile::SnapshotFile(const std::filesystem::path& path, const SearchParametr& param) : rootDir(path), parametrs(param)
{
	namespace fs = std::filesystem; 

	if (!fs::exists(rootDir))
	{
		throw std::invalid_argument("Указанный путь не существует: " + rootDir.string());
	}

	auto options = fs::directory_options::skip_permission_denied;

	if (parametrs.depLimith)
		parseFileSpace(fs::directory_iterator(rootDir, options), fs::directory_iterator{}, *this);
	else
		parseFileSpace(fs::recursive_directory_iterator(rootDir, options), fs::recursive_directory_iterator{}, *this);
}

bool isSeparator(char c)
{
	return c == ' ' ||
		c == '\n' ||
		c == '\t' ||
		c == '.' ||
		c == ',' ||
		c == '!' ||
		c == '?' ||
		c == ';' ||
		c == ':' ||
		c == '"';
}		