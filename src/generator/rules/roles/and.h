#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_AND_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_AND_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class AndRole : public Rule {
public:
    AndRole() : Rule("r_and") { }

    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& role_left : data.get_role_elements_by_complexity()[i]) {
                for (const auto& role_right : data.get_role_elements_by_complexity()[j]) {
                    if (data.reached_limit()) return;
                    else if (data.add_role(states, data.get_factory().make_and_role(role_left, role_right))) {
                        m_count_instantiations += 1;
                    }
                }
            }
        }
    }
};

}

#endif
