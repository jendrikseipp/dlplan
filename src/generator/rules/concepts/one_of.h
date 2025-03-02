#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class OneOfConcept : public Rule {
public:
    OneOfConcept() : Rule("c_one_of") { }

    virtual void generate_impl(const States& states, int, FeatureGeneratorData& data) override {
        for (const auto& constant : data.get_factory().get_vocabulary_info()->get_constants()) {
            if (data.add_concept(states, data.get_factory().make_one_of_concept(constant))) {
                m_count_instantiations += 1;
            }
        }
    }
};

}

#endif
