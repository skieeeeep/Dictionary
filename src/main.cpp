#include "DictionaryManager.hpp"
#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::string> parseTokens(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    DictionaryManager manager;
    std::string line;

    while (std::getline(std::cin, line)) {
        auto tokens = parseTokens(line);
        if (tokens.empty()) continue;

        const std::string& cmd = tokens[0];

        if (cmd == "create" && tokens.size() == 2) {
            if (manager.create(tokens[1])) std::cout << "OK\n";
            else std::cout << "INVALID COMMAND\n";
        }
        else if (cmd == "drop" && tokens.size() == 2) {
            if (manager.drop(tokens[1])) std::cout << "OK\n";
            else std::cout << "INVALID COMMAND\n";
        }
        else if (cmd == "rename" && tokens.size() == 3) {
            if (manager.rename(tokens[1], tokens[2])) std::cout << "OK\n";
            else std::cout << "INVALID COMMAND\n";
        }
        else if (cmd == "list" && tokens.size() == 1) {
            manager.list();
        }
        else if (cmd == "add" && tokens.size() >= 5) {
            auto* dict = manager.get(tokens[1]);
            if (dict) {
                std::vector<std::string> translations(tokens.begin() + 4, tokens.end());
                if (dict->addWord(tokens[2], tokens[3], translations)) std::cout << "OK\n";
                else std::cout << "INVALID COMMAND\n";
            } else std::cout << "INVALID COMMAND\n";
        }
        else if (cmd == "remove" && (tokens.size() == 3 || tokens.size() == 4)) {
            auto* dict = manager.get(tokens[1]);
            std::string pos = (tokens.size() == 4) ? tokens[3] : "";
            if (dict && dict->removeWord(tokens[2], pos)) std::cout << "OK\n";
            else std::cout << "INVALID COMMAND\n";
        }
        else if (cmd == "edit" && tokens.size() >= 5) {
            auto* dict = manager.get(tokens[1]);
            if (dict) {
                std::vector<std::string> translations(tokens.begin() + 4, tokens.end());
                if (dict->editWord(tokens[2], tokens[3], translations)) std::cout << "OK\n";
                else std::cout << "INVALID COMMAND\n";
            } else std::cout << "INVALID COMMAND\n";
        }
        else if (cmd == "show" && (tokens.size() == 2 || tokens.size() == 3)) {
            auto* dict = manager.get(tokens[1]);
            if (dict) {
                std::string pos = (tokens.size() == 3) ? tokens[2] : "";
                dict->show(pos);
            } else std::cout << "INVALID COMMAND\n";
        }
        else if (cmd == "search" && (tokens.size() == 3 || tokens.size() == 4)) {
            auto* dict = manager.get(tokens[1]);
            if (dict) {
                std::string pos = (tokens.size() == 4) ? tokens[3] : "";
                dict->search(tokens[2], pos);
            } else std::cout << "INVALID COMMAND\n";
        }
        else if (cmd == "reverse-search" && tokens.size() == 3) {
            auto* dict = manager.get(tokens[1]);
            if (dict) {
                dict->reverseSearch(tokens[2]);
            } else std::cout << "INVALID COMMAND\n";
        }
        else if (cmd == "stats" && tokens.size() == 2) {
            auto* dict = manager.get(tokens[1]);
            if (dict) {
                dict->stats(tokens[1]);
            } else std::cout << "INVALID COMMAND\n";
        }
        else if (cmd == "save" && tokens.size() == 3) {
            auto* dict = manager.get(tokens[1]);
            if (dict && dict->saveToFile(tokens[2])) std::cout << "OK\n";
            else std::cout << "AN ERROR HAS OCCURED\n";
        }
        else if (cmd == "load" && tokens.size() == 3) {
            auto* dict = manager.get(tokens[1]);
            if (dict && dict->loadFromFile(tokens[2])) std::cout << "OK\n";
            else std::cout << "AN ERROR HAS OCCURED\n";
        }
        else {
            std::cout << "INVALID COMMAND\n";
        }
    }

    return 0;
}