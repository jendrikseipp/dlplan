#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_NOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_NOT_H_

#include "../role.h"


namespace dlplan::core::element {

class NotRole : public Role {
protected:
    const Role_Ptr m_role;

public:
    NotRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary, "r_not"), m_role(role) {
        if (!role) {
            throw std::runtime_error("NotRole::NotRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation result = state.get_instance_info()->get_top_role_vec();
        const RoleDenotation r_vec = m_role->evaluate(state);
        for (const auto& r : r_vec) {
            result.erase(r);
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
