#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SOME_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SOME_H_

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class SomeConcept : public Concept {
protected:
    const Role_Ptr m_role;
    const Concept_Ptr m_concept;

public:
    SomeConcept(const VocabularyInfo& vocabulary, Role_Ptr role, Concept_Ptr concept)
    : Concept(vocabulary, "c_some"), m_role(role), m_concept(concept) {
        if (!(role && concept)) {
            throw std::runtime_error("SomeConcept::SomeConcept - at least one child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        const RoleDenotation r = m_role->evaluate(state);
        const ConceptDenotation c = m_concept->evaluate(state);
        // Find examples: (a,b) in R and b in C => add a
        ConceptDenotation result;
        for (const auto& x : r) {  // (a,b) in R
            if (c.find(x.second) != c.end()) {  // b in C
                result.insert(x.first);  // add a
            }
        }
        return result;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + m_concept->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_role->compute_repr() << "," << m_concept->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif
