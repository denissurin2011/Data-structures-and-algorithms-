#include <iostream>
#include <list>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdexcept>

template <class KeyType, class ValueType, class Hash = std::hash<KeyType>>

class HashMap {
 public:
    using iterator = typename std::list<std::pair<const KeyType, ValueType>>::iterator;
    using const_iterator = typename std::list<std::pair<const KeyType, ValueType>>::const_iterator;

    HashMap(Hash hash = Hash()) : table(def_size), h(hash) {}

    template<typename input>
    HashMap(input first, input last, Hash hash = Hash()) : HashMap() {
        for (; first != last; ++first) {
            insert(*first);
        }
    }

    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> val, Hash hash = Hash())
            : HashMap() {
        for (const auto& i : val) {
            insert(i);
        }
    }
    HashMap(const HashMap& val) : HashMap(val.begin(), val.end()) {}
    HashMap& operator = (const HashMap& val) {
        if (this == &val) {
            return *this;
        }
        *this = HashMap(val.begin(), val.end());
        return *this;
    }
    HashMap(HashMap&& val) :
            num(std::move(val.num)),
            all(std::move(val.all)),
            table(std::move(val.table)),
            h(std::move(val.h))
    {}
    HashMap& operator = (HashMap&& val) {
        std::swap((*this).num, val.num);
        std::swap((*this).all, val.all);
        std::swap((*this).table, val.table);
        std::swap((*this).h, val.h);
        return *this;
    }
    size_t size() const {
        return num;
    }

    const bool empty() const {
        return (num == 0);
    }
    Hash hash_function() const {
        return h;
    }
    iterator  begin() {
        return all.begin();
    }
    const_iterator  begin() const {
        return all.begin();
    }
    iterator end() {
        return all.end();
    }
    const_iterator end() const {
        return all.end();
    }
    void erase(KeyType val) {
        if (find(val) != end()) {
            num--;
            size_t block = h(val) % def_size;
            size_t j = 0;
            for (auto i : table[block]) {
                if (i->first == val) {
                    std::swap(table[block][j], table[block].back());
                    table[block].pop_back();
                    all.erase(i);
                    break;
                }
                j++;
            }
        }
    }
    iterator insert(std::pair<KeyType, ValueType> val) {
        if (find(val.first) == end()) {
            auto i = all.insert(all.end(), val);
            size_t block = h(val.first) % def_size;
            table[block].push_back(i);
            num++;
            return i;
        } else {
            return find(val.first);
        }
    }
    iterator find(const KeyType& val) {
        size_t block = h(val) % def_size;
        for (auto i : table[block]) {
            if (i->first == val) {
                return i;
            }
        }
        return end();
    }
    const_iterator find(const KeyType& val) const {
        size_t block = h(val) % def_size;
        for (auto i : table[block]) {
            if (i->first == val) {
                return i;
            }
        }
        return end();
    }
    void clear() {
        while (!empty()) {
            erase(all.back().first);
        }
    }
    ValueType& operator[] (const KeyType& key) {
        auto iter = insert(std::make_pair(key, ValueType()));
        return iter->second;
    }
    const ValueType& at(const KeyType& key) const {
        auto iter = find(key);
        if (iter != end()) {
            return iter->second;
        } else {
            throw std::out_of_range("");
        }
    }

 private:
    size_t num = 0;
    const size_t def_size = 64747;
    std::list<std::pair<const KeyType, ValueType>> all;
    std::vector<std::vector<iterator>> table;
    Hash h;
};
