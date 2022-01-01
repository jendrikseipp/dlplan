#ifndef DLPLAN_INCLUDE_DLPLAN_TYPES_H_
#define DLPLAN_INCLUDE_DLPLAN_TYPES_H_

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

#include "dynamic_bitset.h"


namespace dlplan::core {

using ConceptDenotation = std::unordered_set<int>;

struct RoleDenotationHash {
    std::size_t operator()(const std::pair<int, int> &r) const {
        return r.first << ((sizeof(size_t) / 2) + r.second);
    }
};
using RoleDenotation = std::unordered_set<std::pair<int, int>, RoleDenotationHash>;

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;
using IndexPair_Vec = std::vector<std::pair<int, int>>;

}

#endif
