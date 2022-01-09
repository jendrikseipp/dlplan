#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_ROLE_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_ROLE_H_

#include "rule.h"


namespace dlplan::generator::rules {

class Role : public Rule {
protected:
    std::deque<utils::threadpool::ThreadPool::TaskFuture<std::pair<core::Role,std::array<u_int32_t, 4>>>> m_tasks;

protected:
    virtual void parse_results_of_tasks_impl(int iteration, GeneratorData& data) override {
        /* Wait for the result and add it. */
        while (!m_tasks.empty()) {
            auto result = m_tasks.front().get();
            m_tasks.pop_front();
            if (data.m_role_hash_table.insert(std::move(result.second))) {
                data.m_reprs.push_back(result.first.compute_repr());
                data.m_roles_by_iteration[iteration+1].push_back(std::move(result.first));
                ++m_count;
            }
        }
    }

public:
    Role(const std::string& name) : Rule(name) { }
};

}

#endif
