#include "boolean.h"


namespace dlplan::generator::rules {

std::function<BooleanTaskResult(const States&, const core::Boolean&, core::element::GeneratorEvaluationCaches&)> Boolean::m_task =
[](const States& states, const core::Boolean& element, core::element::GeneratorEvaluationCaches& cache) {
    return BooleanTaskResult(
        core::Boolean(element),
        element.compute_complexity(),
        element.compute_repr(),
        compute_hash(evaluate_boolean(*element.get_element(), states, cache)),
        false);
};

void Boolean::parse_results_of_tasks_impl(GeneratorData& data) {
    /* Wait for the result and add it. */
    while (!m_tasks.empty()) {
        if (data.reached_resource_limit()) return;
        auto result = m_tasks.front().get();
        m_tasks.pop_front();
        ++data.m_num_generated_features;
        if (result.prune) {
            continue;
        }
        if (data.m_boolean_and_numerical_hash_table.insert(std::move(result.hash)).second) {
            ++data.m_num_novel_features;
            ++m_count;
            data.m_reprs.push_back(std::move(result.repr));
            data.m_booleans_by_iteration[result.complexity].push_back(std::move(result.boolean));
        }
    }
}

void Boolean::cleanup() {
    m_tasks.clear();
}

}
