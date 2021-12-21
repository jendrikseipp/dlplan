#ifndef DLPLAN_SRC_UTILS_COLLECTIONS_H_
#define DLPLAN_SRC_UTILS_COLLECTIONS_H_

/**
 * Partially taken from fast-downward.org
 */

#include <unordered_set>
#include <vector>


namespace dlplan::utils {

template<class T, class H = std::hash<T>>
bool is_subset(const std::unordered_set<T, H>& l, const std::unordered_set<T, H>& r) {
    if (l.size() > r.size()) {
        return false;
    }
    for (const auto& x : l) {
        if (r.find(x) == r.end()) {
            return false;
        }
    }
    return true;
}

template<class T, class H = std::hash<T>>
std::unordered_set<T, H> intersect(const std::unordered_set<T, H>& l, const std::unordered_set<T, H>& r) {
    std::unordered_set<T, H> result;
    for (const auto& x : l) {
        if (r.find(x) != r.end()) {
            result.insert(x);
        }
    }
    return result;
}

template<class T, class H = std::hash<T>>
std::unordered_set<T, H> merge(const std::unordered_set<T, H>& l, const std::unordered_set<T, H>& r) {
    std::unordered_set<T, H> result;
    for (const auto& x : l) result.insert(x);
    for (const auto& x : r) result.insert(x);
    return result;
}

template<class T, class H = std::hash<T>>
std::unordered_set<T, H> minus(const std::unordered_set<T, H>& l, const std::unordered_set<T, H>& r) {
    std::unordered_set<T, H> result = l;
    for (const auto& x : l) {
        if (r.find(x) != r.end()) {
            result.erase(x);
        }
    }
    return result;
}

template<class T>
bool in_bounds(int index, const T &container) {
    return index >= 0 && static_cast<size_t>(index) < container.size();
}

template<class T>
bool in_bounds(long index, const T &container) {
    return index >= 0 && static_cast<size_t>(index) < container.size();
}

template<class T>
bool in_bounds(size_t index, const T &container) {
    return index < container.size();
}

}

#endif
