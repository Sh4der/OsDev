/**
 *   @file: Map.h
 *
 *   @date: Jan 3, 2018
 * @author: Mateusz Midor
 */

#ifndef USER_USTD_SRC_MAP_H_
#define USER_USTD_SRC_MAP_H_

#include <utility>
#include <algorithm>
#include "Vector.h"

namespace ustd {

/**
 * @brief   Primitive and minimal Map with bad access time, to be replaced later
 */
template <class K, class V>
class Map {
    using KeyValue = std::pair<K, V>;
    using KeyValues = vector<KeyValue>;
    using Iterator = typename KeyValues::iterator;

public:
    V& operator[](const K& key) {
        auto it = find(key);
        if (it != key_values.end())
            return it->second;

        key_values.emplace_back(key, V{});
        return key_values.back().second;
    }

    Iterator find(const K& key) {
        auto pred = [&key] (const KeyValue& kv) { return kv.first == key;};
        return std::find_if(begin(), end(), pred);
    }

    Iterator begin() { return key_values.begin(); }
    Iterator end() { return key_values.end(); }

private:
    KeyValues key_values;
};

} /* namespace ustd */

#endif /* USER_USTD_SRC_MAP_H_ */
