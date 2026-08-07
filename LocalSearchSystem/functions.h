#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <initializer_list>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <cstdint>
#include <algorithm>

class SearchParametr
{
public:

	bool depLimith;
	bool ignoreSystem;
	std::unordered_set<std::string> formatFiles;

	SearchParametr() = default; 
    SearchParametr(bool lim, bool ignr, std::initializer_list<std::string> lst) : depLimith(lim), ignoreSystem(ignr), formatFiles(lst) {}
};

class SnapshotFile {
private:

    class MatchInfo;

    std::filesystem::path rootDir;
    SearchParametr parametrs;
    std::unordered_map<std::string, std::vector<MatchInfo>> invertedIndex;
    std::unordered_map<std::filesystem::path, uint32_t> pathToDocId;
    std::vector<std::filesystem::path> docIdToPath; 
    std::vector<bool> isDeleted; 

    static std::pair<std::string, std::vector<uint32_t>> parseFile(
        const std::filesystem::path& filePath
    );
public:

    class SearchResult;

    SnapshotFile(const std::filesystem::path& path, const SearchParametr& param);
};

class SnapshotFile::SearchResult
{
public:

    std::string filePath;
    uint32_t occurrences;
    std::vector<uint32_t> lines;
};

class SnapshotFile::MatchInfo 
{
public: 

    uint32_t docId;
    uint32_t count;
    std::vector<uint32_t> lines;
};