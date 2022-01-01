#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_

#include "../concept.h"


namespace dlplan::core::element {

class OneOfConcept : public Concept {
protected:
    const Constant m_constant;

public:
    OneOfConcept(const VocabularyInfo& vocabulary, const Constant& constant)
    : Concept(vocabulary, "c_one_of"), m_constant(constant) {
    }

    ConceptDenotation evaluate(const State& state) const override {
        // TODO(dominik): We might want to allow for not crashing if there is no object of the constant and instead add a dummy concept.
        if (!state.get_instance_info()->exists_object(m_constant.get_name())) {
            throw std::runtime_error("OneOfConcept::evaluate - no object with name of constant exists in instance: (" + m_constant.get_name() + ")");
        }
        return ConceptDenotation({ state.get_instance_info()->get_object_idx(m_constant.get_name()) });
    }

    int compute_complexity() const override {
        return 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_constant.get_name() << ")";
        return ss.str();
    }
};

}

#endif
