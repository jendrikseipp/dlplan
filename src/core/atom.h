#ifndef DLPLAN_SRC_CORE_ATOM_H_
#define DLPLAN_SRC_CORE_ATOM_H_

#include <string>
#include <vector>

#include "../../include/dlplan/core.h"


namespace dlplan::core {
class Object;

class AtomImpl {
private:
    const std::string m_name;
    const int m_index;
    const Predicate m_predicate;
    const std::vector<Object> m_objects;
    const bool m_is_static;

public:
    AtomImpl(
        const std::string& name,
        int index,
        const Predicate& predicate,
        const std::vector<Object> &objects,
        bool is_static);
    ~AtomImpl() = default;

    const std::string& get_name() const;
    int get_index() const;
    const Predicate& get_predicate() const;
    const std::vector<Object>& get_objects() const;
    const Object& get_object(int pos) const;
    bool get_is_static() const;
    std::string str() const;
};

}

#endif
