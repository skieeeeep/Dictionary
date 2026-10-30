#ifndef ROBIN_HOOD_TABLE_HPP
#define ROBIN_HOOD_TABLE_HPP

#include <vector>
#include <string>
#include <functional>
#include <utility>
#include <stdexcept>

template <typename Key, typename Value>
class RobinHoodTable {
private:
    struct Entry {
        Key key;
        Value value;
        size_t psl = 0;
        bool occupied = false;
    };

    std::vector<Entry> table;
    size_t count = 0;

    size_t hashKey(const Key& key) const {
        return std::hash<Key>{}(key);
    }

    void rehash() {
        std::vector<Entry> old_table = std::move(table);
        table.assign(old_table.size() * 2, Entry{});
        count = 0;

        for (auto& entry : old_table) {
            if (entry.occupied) {
                insert(entry.key, entry.value);
            }
        }
    }

public:
    explicit RobinHoodTable(size_t initial_capacity = 16) {
        table.resize(initial_capacity);
    }

    bool insert(const Key& key, const Value& value) {
        if (find(key) != nullptr) {
            return false;
        }

        if (static_cast<double>(count + 1) / table.size() > 0.7) {
            rehash();
        }

        Entry cur{key, value, 0, true};
        size_t idx = hashKey(cur.key) % table.size();

        while (true) {
            if (!table[idx].occupied) {
                table[idx] = cur;
                count++;
                return true;
            }

            if (cur.psl > table[idx].psl) {
                std::swap(cur, table[idx]);
            }

            cur.psl++;
            idx = (idx + 1) % table.size();
        }
    }

    Value* find(const Key& key) {
        if (table.empty()) return nullptr;

        size_t idx = hashKey(key) % table.size();
        size_t psl = 0;

        while (true) {
            if (!table[idx].occupied || psl > table[idx].psl) {
                return nullptr;
            }
            if (table[idx].key == key) {
                return &table[idx].value;
            }
            psl++;
            idx = (idx + 1) % table.size();
        }
    }

    const Value* find(const Key& key) const {
        return const_cast<RobinHoodTable*>(this)->find(key);
    }

    bool erase(const Key& key) {
        if (table.empty()) return false;

        size_t idx = hashKey(key) % table.size();
        size_t psl = 0;

        while (true) {
            if (!table[idx].occupied || psl > table[idx].psl) {
                return false;
            }
            if (table[idx].key == key) {
                size_t next_idx = (idx + 1) % table.size();
                while (table[next_idx].occupied && table[next_idx].psl > 0) {
                    table[idx] = table[next_idx];
                    table[idx].psl--;
                    idx = next_idx;
                    next_idx = (next_idx + 1) % table.size();
                }
                table[idx] = Entry{};
                count--;
                return true;
            }
            psl++;
            idx = (idx + 1) % table.size();
        }
    }

    size_t size() const { return count; }

    void clear() {
        table.assign(16, Entry{});
        count = 0;
    }

    std::vector<std::pair<Key, Value>> getAll() const {
        std::vector<std::pair<Key, Value>> res;
        for (const auto& entry : table) {
            if (entry.occupied) {
                res.emplace_back(entry.key, entry.value);
            }
        }
        return res;
    }
};

#endif