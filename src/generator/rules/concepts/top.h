#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class TopConcept : public Rule {
public:
    TopConcept() : Rule("c_top") { }

    virtual void generate_impl(const States& states, int, FeatureGeneratorData& data) override {
        if (data.add_concept(states, data.get_factory().make_top_concept())) {
            m_count_instantiations += 1;
        }
    }
};

}

#endif
