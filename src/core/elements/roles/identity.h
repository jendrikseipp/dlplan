#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_

#include "../role.h"
#include "../concept.h"


namespace dlplan::core::element {

class IdentityRole : public Role {
protected:
    const Concept_Ptr m_concept;

public:
    IdentityRole(const VocabularyInfo& vocabulary, Concept_Ptr concept)
    : Role(vocabulary, "r_identity"), m_concept(concept) {
        if (!concept) {
            throw std::runtime_error("IdentityRole::IdentityRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        const auto c = m_concept->evaluate(state);
        const auto& c_data = c.get_const_data();
        int num_objects = state.get_instance_info()->get_num_objects();
        RoleDenotation result(num_objects);
        auto& result_data = result.get_data();
        for (int i = 0; i < num_objects; ++i) {
            if (c_data.test(i)) {
                result_data.set(i * num_objects + i);
            }
        }
        return result;
    }

    int compute_complexity() const override {
        return m_concept->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_concept->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
