#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_REFLEXIVE_CLOSURE_H_

#include "../role.h"
#include "../utils.h"


namespace dlplan::core::element {

class TransitiveReflexiveClosureRole : public Role {
protected:
    const Role_Ptr m_role;

public:
    TransitiveReflexiveClosureRole(const VocabularyInfo& vocabulary, Role_Ptr role)
    : Role(vocabulary, "r_transitive_reflexive_closure"), m_role(role) {
        if (!role) {
            throw std::runtime_error("TransitiveReflexiveClosureRole::TransitiveReflexiveClosureRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        const RoleDenotation r_vec = m_role->evaluate(state);
        // 1. Compute an adjacency list from the newly mapped role denotations.
        utils::AdjList adj_list = utils::compute_adjacency_list(r_vec, state.get_instance_info()->get_num_objects());
        // 2. Compute pairwise distances
        utils::PairwiseDistances pairwise_distances = utils::compute_floyd_warshall(adj_list, true);
        // 3. Extract the transitive closure from the pairwise distances.
        return utils::compute_transitive_closure(pairwise_distances);
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
