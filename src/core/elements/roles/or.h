#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_OR_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_OR_H_

#include "../role.h"


namespace dlplan::core::element {

class OrRole : public Role {
protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

public:
    OrRole(const VocabularyInfo& vocabulary, Role_Ptr role_1, Role_Ptr role_2)
    : Role(vocabulary, "r_or"),
      m_role_left(role_1->compute_repr() < role_2->compute_repr() ? role_1 : role_2),
      m_role_right(role_1->compute_repr() < role_2->compute_repr() ? role_2 : role_1) {
        if (!(role_1 && role_2)) {
            throw std::runtime_error("OrRole::OrRole - at least one child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        const RoleDenotation l = m_role_left->evaluate(state);
        const RoleDenotation r = m_role_right->evaluate(state);
        return dlplan::utils::merge(l, r);
    }

    int compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role_left->compute_repr() << "," << m_role_right->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
