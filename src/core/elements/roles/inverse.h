#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_INVERSE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_INVERSE_H_

#include "../role.h"


namespace dlplan::core::element {

class InverseRole : public Role {
protected:
    const Role_Ptr m_role;

public:
    InverseRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary, "r_inverse"), m_role(role) {
        if (!role) {
            throw std::runtime_error("InverseRole::InverseRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        const RoleDenotation r_vec = m_role->evaluate(state);
        RoleDenotation result;
        result.reserve(r_vec.size());
        for (const auto& r : r_vec) {
            result.emplace(r.second, r.first);
        }
        return result;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
