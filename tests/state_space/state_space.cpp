#include <gtest/gtest.h>

#include "../include/dlplan/state_space.h"

using namespace dlplan::core;
using namespace dlplan::state_space;


TEST(DLPTests, StateSpaceTest) {
    StateSpaceGenerator().generate_state_space("domain.pddl", "instance.pddl");
    auto state_space = StateSpaceReader().read();
    auto distance_information = state_space.compute_goal_distance_information();
    state_space.print();
    ASSERT_EQ(distance_information.is_solvable(), true);
}
