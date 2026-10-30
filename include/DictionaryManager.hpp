#ifndef DICTIONARY_MANAGER_HPP
#define DICTIONARY_MANAGER_HPP

#include "Dictionary.hpp"
#include <unordered_map>
#include <string>

class DictionaryManager {
private:
    std::unordered_map<std::string, Dictionary> dictionaries;

public:
    bool create(const std::string& name);
    bool drop(const std::string& name);
    bool rename(const std::string& old_name, const std::string& new_name);
    void list() const;

    Dictionary* get(const std::string& name);
};

#endif