#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_SUM_ROLE_DISTANCE_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_SUM_ROLE_DISTANCE_H_

#include "../numerical.h"

#include "../../../core/elements/numericals/sum_role_distance.h"


namespace dlplan::generator::rules {

class SumRoleDistanceNumerical : public Numerical {
public:
    SumRoleDistanceNumerical() : Numerical() { }

    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        if (m_lookahead) target_complexity += 3;
        for (int i = 1; i <= target_complexity - 3; ++i) {
            for (int j = 1; j <= target_complexity - i - 2; ++j) {
                int k = target_complexity - i - j - 1;
                for (const auto& r1 : data.m_roles_by_iteration[i]) {
                    for (const auto& r2 : data.m_roles_by_iteration[j]) {
                        for (const auto& r3 : data.m_roles_by_iteration[k]) {
                            m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_sum_role_distance(r1, r2, r3))));
                        }
                    }
                }
            }
        }
    }

    std::string get_name() const override {
        return core::element::SumRoleDistanceNumerical::get_name();
    }
};

}

#endif
