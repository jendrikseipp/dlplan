#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_DIFF_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_DIFF_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class DiffConcept : public Rule {
public:
    DiffConcept() : Rule("c_diff") { }

    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& concept_left : data.get_concept_elements_by_complexity()[i]) {
                for (const auto& concept_right : data.get_concept_elements_by_complexity()[j]) {
                    if (data.reached_limit()) return;
                    else if (data.add_concept(states, data.get_factory().make_diff_concept(concept_left, concept_right))) {
                        m_count_instantiations += 1;
                    }
                }
            }
        }
    }
};

}

#endif
