#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, RoleOr) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("role_1", 2);
    Predicate p1 = vocabulary->add_predicate("role_2", 2);
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary);
    // Add state atoms
    Atom a0 = instance->add_atom("role_1", {"A", "X"});
    Atom a1 = instance->add_atom("role_1", {"B", "Y"});

    Atom a2 = instance->add_atom("role_2", {"B", "Y"});
    Atom a3 = instance->add_atom("role_2", {"C", "Z"});

    State state(instance, {a0, a1, a2, a3});

    SyntacticElementFactory factory(vocabulary);

    Role role = factory.parse_role("r_or(r_primitive(role_1,0,1),r_primitive(role_2,0,1))");
    EXPECT_EQ(role.evaluate(state), dlplan::core::RoleDenotation({{0, 1}, {2, 3}, {4, 5}}));
}
