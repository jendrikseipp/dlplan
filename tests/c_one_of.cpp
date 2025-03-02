#include <gtest/gtest.h>

#include "../include/dlplan/core.h"

using namespace dlplan::core;


TEST(DLPTests, ConceptOneOf) {
    // Add predicates
    std::shared_ptr<VocabularyInfo> vocabulary = std::make_shared<VocabularyInfo>();
    Predicate p0 = vocabulary->add_predicate("concept", 1);
    Constant c0 = vocabulary->add_constant("A");
    std::shared_ptr<InstanceInfo> instance = std::make_shared<InstanceInfo>(vocabulary);
    // Add state atoms
    Atom a0 = instance->add_atom("concept", {"A"});
    Atom a1 = instance->add_atom("concept", {"B"});

    State state(instance, {a0, a1});

    SyntacticElementFactory factory(vocabulary);

    Concept concept = factory.parse_concept("c_one_of(A)");
    EXPECT_EQ(concept.evaluate(state).to_vector(), Index_Vec({0}));
}
