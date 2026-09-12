#pragma once
#include <iostream>
#include <filesystem>
#include <system_error>
#include <fstream>
#include <vector>
#include <string>
#include <initializer_list>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <cstdint>
#include <algorithm>

bool isSeparator(char c);

class SearchParametr
{
public:

	bool depLimith = false;
	std::unordered_set<std::string> formatFiles;

	SearchParametr() = default; 
    SearchParametr(bool lim, std::initializer_list<std::string> lst) : depLimith(lim), formatFiles(lst) {}
};

class SnapshotFile
{
private:

    class MatchInfo;

    std::filesystem::path rootDir;
    SearchParametr parametrs;
    std::unordered_map<std::string, std::vector<MatchInfo>> invertedIndex;
    std::unordered_map<std::filesystem::path, uint32_t> pathToDocId;
    std::vector<std::filesystem::path> docIdToPath; 
    std::vector<bool> badFiles;
    std::vector<bool> isOutdated; 

    template <typename Iterator>
    static void parseFileSpace(Iterator begin, Iterator end, SnapshotFile& obj);

    static void parseFile(const std::filesystem::path& filePath, SnapshotFile& obj, uint32_t indFile);

public:

    class SearchResult;

    SnapshotFile(const std::filesystem::path& path, const SearchParametr& param);
};

class SnapshotFile::MatchInfo 
{
public: 

    uint32_t docId;
    uint32_t count;
    std::vector<uint32_t> lines;

    MatchInfo(uint32_t doc, uint32_t line) : 
        docId(doc),
        count(1),
        lines(std::vector<uint32_t> {line})
    {}
};

class SnapshotFile::SearchResult
{
public:

    std::string filePath;
    uint32_t occurrences;
    std::vector<uint32_t> lines;
};
