#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include "RobinHood.hpp"
#include <string>
#include <vector>
#include <map>
#include <set>

struct WordDetails {
    std::map<std::string, std::vector<std::string>> pos_translations;
};

class Dictionary {
private:
    RobinHood<std::string, WordDetails> data;

public:
    bool addWord(const std::string& word, const std::string& pos, const std::vector<std::string>& translations);
    bool removeWord(const std::string& word, const std::string& pos = "");
    bool editWord(const std::string& word, const std::string& pos, const std::vector<std::string>& translations);

    void show(const std::string& pos = "") const;
    void search(const std::string& query, const std::string& pos = "") const;
    void reverseSearch(const std::string& russian_query) const;
    void stats(const std::string& dict_name) const;

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
};

#endif