#include "Dictionary.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

bool Dictionary::addWord(const std::string& word, const std::string& pos, const std::vector<std::string>& translations) {
    WordDetails* details = data.find(word);
    if (!details) {
        WordDetails new_details;
        new_details.pos_translations[pos] = translations;
        return data.insert(word, new_details);
    }

    auto& target_trans = details->pos_translations[pos];
    for (const auto& tr : translations) {
        if (std::find(target_trans.begin(), target_trans.end(), tr) == target_trans.end()) {
            target_trans.push_back(tr);
        }
    }
    return true;
}

bool Dictionary::removeWord(const std::string& word, const std::string& pos) {
    WordDetails* details = data.find(word);
    if (!details) return false;

    if (pos.empty()) {
        return data.erase(word);
    }

    auto it = details->pos_translations.find(pos);
    if (it == details->pos_translations.end()) return false;

    details->pos_translations.erase(it);
    if (details->pos_translations.empty()) {
        data.erase(word);
    }
    return true;
}

bool Dictionary::editWord(const std::string& word, const std::string& pos, const std::vector<std::string>& translations) {
    WordDetails* details = data.find(word);
    if (!details) return false;

    auto it = details->pos_translations.find(pos);
    if (it == details->pos_translations.end()) return false;

    it->second = translations;
    return true;
}

void Dictionary::show(const std::string& pos) const {
    auto all_entries = data.getAll();
    std::sort(all_entries.begin(), all_entries.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    for (const auto& [word, details] : all_entries) {
        for (const auto& [part_of_speech, translations] : details.pos_translations) {
            if (pos.empty() || part_of_speech == pos) {
                std::cout << word << " [" << part_of_speech << "]: ";
                for (size_t i = 0; i < translations.size(); ++i) {
                    std::cout << translations[i] << (i + 1 < translations.size() ? ", " : "");
                }
                std::cout << "\n";
            }
        }
    }
}

void Dictionary::search(const std::string& query, const std::string& pos) const {
    auto all_entries = data.getAll();
    std::sort(all_entries.begin(), all_entries.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    for (const auto& [word, details] : all_entries) {
        if (word.find(query) != std::string::npos) {
            for (const auto& [part_of_speech, translations] : details.pos_translations) {
                if (pos.empty() || part_of_speech == pos) {
                    std::cout << word << " [" << part_of_speech << "]: ";
                    for (size_t i = 0; i < translations.size(); ++i) {
                        std::cout << translations[i] << (i + 1 < translations.size() ? ", " : "");
                    }
                    std::cout << "\n";
                }
            }
        }
    }
}

void Dictionary::reverseSearch(const std::string& russian_query) const {
    auto all_entries = data.getAll();
    std::sort(all_entries.begin(), all_entries.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    for (const auto& [word, details] : all_entries) {
        for (const auto& [part_of_speech, translations] : details.pos_translations) {
            bool found = false;
            for (const auto& tr : translations) {
                if (tr.find(russian_query) != std::string::npos) {
                    found = true;
                    break;
                }
            }
            if (found) {
                std::cout << word << " [" << part_of_speech << "]\n";
            }
        }
    }
}

void Dictionary::stats(const std::string& dict_name) const {
    auto all_entries = data.getAll();
    std::map<std::string, size_t> pos_counts;
    size_t total_words = all_entries.size();

    for (const auto& [word, details] : all_entries) {
        for (const auto& [pos, translations] : details.pos_translations) {
            pos_counts[pos]++;
        }
    }

    std::cout << "Dictionary: " << dict_name << "\n";
    std::cout << "Total words: " << total_words << "\n";
    for (const auto& [pos, count] : pos_counts) {
        std::string formatted_pos = pos;
        if (!formatted_pos.empty()) {
            formatted_pos[0] = std::toupper(formatted_pos[0]);
        }
        std::cout << formatted_pos << ": " << count << "\n";
    }
}

bool Dictionary::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    for (const auto& [word, details] : data.getAll()) {
        for (const auto& [pos, translations] : details.pos_translations) {
            out << word << ";" << pos;
            for (const auto& tr : translations) {
                out << ";" << tr;
            }
            out << "\n";
        }
    }
    return true;
}

bool Dictionary::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    data.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string word, pos, tr;

        if (std::getline(ss, word, ';') && std::getline(ss, pos, ';')) {
            std::vector<std::string> translations;
            while (std::getline(ss, tr, ';')) {
                translations.push_back(tr);
            }
            addWord(word, pos, translations);
        }
    }
    return true;
}