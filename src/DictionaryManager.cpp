#include "DictionaryManager.hpp"
#include <iostream>

bool DictionaryManager::create(const std::string& name) {
    if (dictionaries.find(name) != dictionaries.end()) return false;
    dictionaries[name] = Dictionary();
    return true;
}

bool DictionaryManager::drop(const std::string& name) {
    return dictionaries.erase(name) > 0;
}

bool DictionaryManager::rename(const std::string& old_name, const std::string& new_name) {
    if (dictionaries.find(old_name) == dictionaries.end()) return false;
    if (dictionaries.find(new_name) != dictionaries.end()) return false;

    dictionaries[new_name] = std::move(dictionaries[old_name]);
    dictionaries.erase(old_name);
    return true;
}

void DictionaryManager::list() const {
    for (const auto& [name, dict] : dictionaries) {
        std::cout << name << "\n";
    }
}

Dictionary* DictionaryManager::get(const std::string& name) {
    auto it = dictionaries.find(name);
    return (it != dictionaries.end()) ? &it->second : nullptr;
}