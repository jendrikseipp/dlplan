#include "element_factory.h"
#include "vocabulary_info.h"
#include "parser/parser.h"
#include "parser/expressions/expression.h"
#include "elements/booleans/empty.h"
#include "elements/booleans/inclusion.h"
#include "elements/booleans/nullary.h"
#include "elements/concepts/all.h"
#include "elements/concepts/bot.h"
#include "elements/concepts/and.h"
#include "elements/concepts/diff.h"
#include "elements/concepts/equal.h"
#include "elements/concepts/not.h"
#include "elements/concepts/one_of.h"
#include "elements/concepts/or.h"
#include "elements/concepts/projection.h"
#include "elements/concepts/primitive.h"
#include "elements/concepts/some.h"
#include "elements/concepts/subset.h"
#include "elements/concepts/top.h"
#include "elements/numericals/concept_distance.h"
#include "elements/numericals/count.h"
#include "elements/numericals/role_distance.h"
#include "elements/numericals/sum_concept_distance.h"
#include "elements/numericals/sum_role_distance.h"
#include "elements/roles/and.h"
#include "elements/roles/compose.h"
#include "elements/roles/diff.h"
#include "elements/roles/identity.h"
#include "elements/roles/inverse.h"
#include "elements/roles/not.h"
#include "elements/roles/or.h"
#include "elements/roles/primitive.h"
#include "elements/roles/restrict.h"
#include "elements/roles/top.h"
#include "elements/roles/transitive_closure.h"
#include "elements/roles/transitive_reflexive_closure.h"
#include "concept.h"
#include "role.h"
#include "numerical.h"
#include "boolean.h"


namespace dlplan::core {

SyntacticElementFactoryImpl::SyntacticElementFactoryImpl(std::shared_ptr<const VocabularyInfo> vocabulary_info)
    : m_vocabulary_info(vocabulary_info), m_caches(Caches()) {
}

Concept SyntacticElementFactoryImpl::parse_concept(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(*m_vocabulary_info, description);
    element::Concept_Ptr result_ptr = expression->parse_concept(*m_vocabulary_info, m_caches);
    return Concept(m_vocabulary_info, std::move(result_ptr));
}

Role SyntacticElementFactoryImpl::parse_role(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(*m_vocabulary_info, description);
    element::Role_Ptr result_ptr = expression->parse_role(*m_vocabulary_info, m_caches);
    return Role(m_vocabulary_info, std::move(result_ptr));
}

Numerical SyntacticElementFactoryImpl::parse_numerical(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(*m_vocabulary_info, description);
    element::Numerical_Ptr result_ptr = expression->parse_numerical(*m_vocabulary_info, m_caches);
    return Numerical(m_vocabulary_info, std::move(result_ptr));
}

Boolean SyntacticElementFactoryImpl::parse_boolean(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(*m_vocabulary_info, description);
    element::Boolean_Ptr result_ptr = expression->parse_boolean(*m_vocabulary_info, m_caches);
    return Boolean(m_vocabulary_info, std::move(result_ptr));
}

Boolean SyntacticElementFactoryImpl::make_empty_boolean(const Concept& concept) {
    element::Concept_Ptr concept_ptr = m_caches.m_concept_cache->at(concept.compute_repr());
    std::unique_ptr<element::Boolean> result_ptr = std::make_unique<element::EmptyBoolean<element::Concept_Ptr>>(*m_vocabulary_info, concept_ptr);
    return Boolean(m_vocabulary_info, m_caches.m_boolean_cache->insert(std::move(result_ptr)));
}

Boolean SyntacticElementFactoryImpl::make_empty_boolean(const Role& role) {
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    std::unique_ptr<element::Boolean> result_ptr = std::make_unique<element::EmptyBoolean<element::Role_Ptr>>(*m_vocabulary_info, role_ptr);
    return Boolean(m_vocabulary_info, m_caches.m_boolean_cache->insert(std::move(result_ptr)));
}

Boolean SyntacticElementFactoryImpl::make_concept_inclusion_boolean(const Concept& concept_left, const Concept& concept_right) {
    element::Concept_Ptr concept_left_ptr = m_caches.m_concept_cache->at(concept_left.compute_repr());
    element::Concept_Ptr concept_right_ptr = m_caches.m_concept_cache->at(concept_right.compute_repr());
    std::unique_ptr<element::Boolean> result_ptr = std::make_unique<element::ConceptInclusionBoolean>(*m_vocabulary_info, concept_left_ptr, concept_right_ptr);
    return Boolean(m_vocabulary_info, m_caches.m_boolean_cache->insert(std::move(result_ptr)));
}

Boolean SyntacticElementFactoryImpl::make_role_inclusion_boolean(const Role& role_left, const Role& role_right) {
    element::Role_Ptr role_left_ptr = m_caches.m_role_cache->at(role_left.compute_repr());
    element::Role_Ptr role_right_ptr = m_caches.m_role_cache->at(role_right.compute_repr());
    std::unique_ptr<element::Boolean> result_ptr = std::make_unique<element::RoleInclusionBoolean>(*m_vocabulary_info, role_left_ptr, role_right_ptr);
    return Boolean(m_vocabulary_info, m_caches.m_boolean_cache->insert(std::move(result_ptr)));
}

Boolean SyntacticElementFactoryImpl::make_nullary_boolean(const Predicate& predicate) {
    std::unique_ptr<element::Boolean> result_ptr = std::make_unique<element::NullaryBoolean>(*m_vocabulary_info, predicate);
    return Boolean(m_vocabulary_info, m_caches.m_boolean_cache->insert(std::move(result_ptr)));
}

Concept SyntacticElementFactoryImpl::make_all_concept(const Role& role, const Concept& concept) {
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    element::Concept_Ptr concept_ptr = m_caches.m_concept_cache->at(concept.compute_repr());
    std::unique_ptr<element::Concept> result_ptr = std::make_unique<element::AllConcept>(*m_vocabulary_info, role_ptr, concept_ptr);
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::move(result_ptr)));
}

Concept SyntacticElementFactoryImpl::make_and_concept(const Concept& concept_left, const Concept& concept_right) {
    element::Concept_Ptr concept_left_ptr = m_caches.m_concept_cache->at(concept_left.compute_repr());
    element::Concept_Ptr concept_right_ptr = m_caches.m_concept_cache->at(concept_right.compute_repr());
    std::unique_ptr<element::Concept> result_ptr = std::make_unique<element::AndConcept>(*m_vocabulary_info, concept_left_ptr, concept_right_ptr);
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::move(result_ptr)));
}

Concept SyntacticElementFactoryImpl::make_bot_concept() {
    std::unique_ptr<element::Concept> result_ptr = std::make_unique<element::BotConcept>(*m_vocabulary_info);
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::move(result_ptr)));
}

Concept SyntacticElementFactoryImpl::make_diff_concept(const Concept& concept_left, const Concept& concept_right) {
    element::Concept_Ptr concept_left_ptr = m_caches.m_concept_cache->at(concept_left.compute_repr());
    element::Concept_Ptr concept_right_ptr = m_caches.m_concept_cache->at(concept_right.compute_repr());
    std::unique_ptr<element::Concept> result_ptr = std::make_unique<element::DiffConcept>(*m_vocabulary_info, concept_left_ptr, concept_right_ptr);
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::move(result_ptr)));
}

Concept SyntacticElementFactoryImpl::make_equal_concept(const Role& role_left, const Role& role_right) {
    element::Role_Ptr role_left_ptr = m_caches.m_role_cache->at(role_left.compute_repr());
    element::Role_Ptr role_right_ptr = m_caches.m_role_cache->at(role_right.compute_repr());
    std::unique_ptr<element::Concept> result_ptr = std::make_unique<element::EqualConcept>(*m_vocabulary_info, role_left_ptr, role_right_ptr);
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::move(result_ptr)));
}

Concept SyntacticElementFactoryImpl::make_not_concept(const Concept& concept) {
    element::Concept_Ptr concept_ptr = m_caches.m_concept_cache->at(concept.compute_repr());
    std::unique_ptr<element::Concept> result_ptr = std::make_unique<element::NotConcept>(*m_vocabulary_info, concept_ptr);
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::move(result_ptr)));
}

Concept SyntacticElementFactoryImpl::make_one_of_concept(const Constant& constant) {
    std::unique_ptr<element::Concept> result_ptr = std::make_unique<element::OneOfConcept>(*m_vocabulary_info, constant);
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::move(result_ptr)));
}

Concept SyntacticElementFactoryImpl::make_or_concept(const Concept& concept_left, const Concept& concept_right) {
    element::Concept_Ptr concept_left_ptr = m_caches.m_concept_cache->at(concept_left.compute_repr());
    element::Concept_Ptr concept_right_ptr = m_caches.m_concept_cache->at(concept_right.compute_repr());
    std::unique_ptr<element::Concept> result_ptr = std::make_unique<element::OrConcept>(*m_vocabulary_info, concept_left_ptr, concept_right_ptr);
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::move(result_ptr)));
}

Concept SyntacticElementFactoryImpl::make_projection_concept(const Role& role, int pos) {
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    std::unique_ptr<element::Concept> result_ptr = std::make_unique<element::ProjectionConcept>(*m_vocabulary_info, role_ptr, pos);
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::move(result_ptr)));
}

Concept SyntacticElementFactoryImpl::make_primitive_concept(const Predicate& predicate, int pos) {
    std::unique_ptr<element::Concept> result_ptr = std::make_unique<element::PrimitiveConcept>(*m_vocabulary_info, predicate, pos);
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::move(result_ptr)));
}

Concept SyntacticElementFactoryImpl::make_some_concept(const Role& role, const Concept& concept) {
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    element::Concept_Ptr concept_ptr = m_caches.m_concept_cache->at(concept.compute_repr());
    std::unique_ptr<element::Concept> result_ptr = std::make_unique<element::SomeConcept>(*m_vocabulary_info, role_ptr, concept_ptr);
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::move(result_ptr)));
}

Concept SyntacticElementFactoryImpl::make_subset_concept(const Role& role_left, const Role& role_right) {
    element::Role_Ptr role_left_ptr = m_caches.m_role_cache->at(role_left.compute_repr());
    element::Role_Ptr role_right_ptr = m_caches.m_role_cache->at(role_right.compute_repr());
    std::unique_ptr<element::Concept> result_ptr = std::make_unique<element::SubsetConcept>(*m_vocabulary_info, role_left_ptr, role_right_ptr);
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::move(result_ptr)));
}

Concept SyntacticElementFactoryImpl::make_top_concept() {
    std::unique_ptr<element::Concept> result_ptr = std::make_unique<element::TopConcept>(*m_vocabulary_info);
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::move(result_ptr)));
}

Numerical SyntacticElementFactoryImpl::make_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    element::Concept_Ptr concept_from_ptr = m_caches.m_concept_cache->at(concept_from.compute_repr());
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    element::Concept_Ptr concept_to_ptr = m_caches.m_concept_cache->at(concept_to.compute_repr());
    std::unique_ptr<element::Numerical> result_ptr = std::make_unique<element::ConceptDistanceNumerical>(*m_vocabulary_info, concept_from_ptr, role_ptr, concept_to_ptr);
    return Numerical(m_vocabulary_info, m_caches.m_numerical_cache->insert(std::move(result_ptr)));
}

Numerical SyntacticElementFactoryImpl::make_count(const Concept& concept) {
    element::Concept_Ptr concept_ptr = m_caches.m_concept_cache->at(concept.compute_repr());
    std::unique_ptr<element::Numerical> result_ptr = std::make_unique<element::CountNumerical<element::Concept_Ptr>>(*m_vocabulary_info, concept_ptr);
    return Numerical(m_vocabulary_info, m_caches.m_numerical_cache->insert(std::move(result_ptr)));
}

Numerical SyntacticElementFactoryImpl::make_count(const Role& role) {
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    std::unique_ptr<element::Numerical> result_ptr = std::make_unique<element::CountNumerical<element::Role_Ptr>>(*m_vocabulary_info, role_ptr);
    return Numerical(m_vocabulary_info, m_caches.m_numerical_cache->insert(std::move(result_ptr)));
}

Numerical SyntacticElementFactoryImpl::make_role_distance(const Role& role_from, const Role& role, const Role& role_to) {
    element::Role_Ptr role_from_ptr = m_caches.m_role_cache->at(role_from.compute_repr());
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    element::Role_Ptr role_to_ptr = m_caches.m_role_cache->at(role_to.compute_repr());
    std::unique_ptr<element::Numerical> result_ptr = std::make_unique<element::RoleDistanceNumerical>(*m_vocabulary_info, role_from_ptr, role_ptr, role_to_ptr);
    return Numerical(m_vocabulary_info, m_caches.m_numerical_cache->insert(std::move(result_ptr)));
}

Numerical SyntacticElementFactoryImpl::make_sum_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    element::Concept_Ptr concept_from_ptr = m_caches.m_concept_cache->at(concept_from.compute_repr());
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    element::Concept_Ptr concept_to_ptr = m_caches.m_concept_cache->at(concept_to.compute_repr());
    std::unique_ptr<element::Numerical> result_ptr = std::make_unique<element::SumConceptDistanceNumerical>(*m_vocabulary_info, concept_from_ptr, role_ptr, concept_to_ptr);
    return Numerical(m_vocabulary_info, m_caches.m_numerical_cache->insert(std::move(result_ptr)));
}

Numerical SyntacticElementFactoryImpl::make_sum_role_distance(const Role& role_from, const Role& role, const Role& role_to) {
    element::Role_Ptr role_from_ptr = m_caches.m_role_cache->at(role_from.compute_repr());
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    element::Role_Ptr role_to_ptr = m_caches.m_role_cache->at(role_to.compute_repr());
    std::unique_ptr<element::Numerical> result_ptr = std::make_unique<element::SumRoleDistanceNumerical>(*m_vocabulary_info, role_from_ptr, role_ptr, role_to_ptr);
    return Numerical(m_vocabulary_info, m_caches.m_numerical_cache->insert(std::move(result_ptr)));
}

Role SyntacticElementFactoryImpl::make_and_role(const Role& role_left, const Role& role_right) {
    element::Role_Ptr role_left_ptr = m_caches.m_role_cache->at(role_left.compute_repr());
    element::Role_Ptr role_right_ptr = m_caches.m_role_cache->at(role_right.compute_repr());
    std::unique_ptr<element::Role> result_ptr = std::make_unique<element::AndRole>(*m_vocabulary_info, role_left_ptr, role_right_ptr);
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(std::move(result_ptr)));
}

Role SyntacticElementFactoryImpl::make_compose_role(const Role& role_left, const Role& role_right) {
    element::Role_Ptr role_left_ptr = m_caches.m_role_cache->at(role_left.compute_repr());
    element::Role_Ptr role_right_ptr = m_caches.m_role_cache->at(role_right.compute_repr());
    std::unique_ptr<element::Role> result_ptr = std::make_unique<element::ComposeRole>(*m_vocabulary_info, role_left_ptr, role_right_ptr);
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(std::move(result_ptr)));
}

Role SyntacticElementFactoryImpl::make_diff_role(const Role& role_left, const Role& role_right) {
    element::Role_Ptr role_left_ptr = m_caches.m_role_cache->at(role_left.compute_repr());
    element::Role_Ptr role_right_ptr = m_caches.m_role_cache->at(role_right.compute_repr());
    std::unique_ptr<element::Role> result_ptr = std::make_unique<element::DiffRole>(*m_vocabulary_info, role_left_ptr, role_right_ptr);
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(std::move(result_ptr)));
}

Role SyntacticElementFactoryImpl::make_identity_role(const Concept& concept) {
    element::Concept_Ptr concept_ptr = m_caches.m_concept_cache->at(concept.compute_repr());
    std::unique_ptr<element::Role> result_ptr = std::make_unique<element::IdentityRole>(*m_vocabulary_info, concept_ptr);
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(std::move(result_ptr)));
}

Role SyntacticElementFactoryImpl::make_inverse_role(const Role& role) {
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    std::unique_ptr<element::Role> result_ptr = std::make_unique<element::InverseRole>(*m_vocabulary_info, role_ptr);
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(std::move(result_ptr)));
}

Role SyntacticElementFactoryImpl::make_not_role(const Role& role) {
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    std::unique_ptr<element::Role> result_ptr = std::make_unique<element::NotRole>(*m_vocabulary_info, role_ptr);
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(std::move(result_ptr)));
}

Role SyntacticElementFactoryImpl::make_or_role(const Role& role_left, const Role& role_right) {
    element::Role_Ptr role_left_ptr = m_caches.m_role_cache->at(role_left.compute_repr());
    element::Role_Ptr role_right_ptr = m_caches.m_role_cache->at(role_right.compute_repr());
    std::unique_ptr<element::Role> result_ptr = std::make_unique<element::OrRole>(*m_vocabulary_info, role_left_ptr, role_right_ptr);
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(std::move(result_ptr)));
}

Role SyntacticElementFactoryImpl::make_primitive_role(const Predicate& predicate, int pos_1, int pos_2) {
    std::unique_ptr<element::Role> result_ptr = std::make_unique<element::PrimitiveRole>(*m_vocabulary_info, predicate, pos_1, pos_2);
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(std::move(result_ptr)));
}

Role SyntacticElementFactoryImpl::make_restrict_role(const Role& role, const Concept& concept) {
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    element::Concept_Ptr concept_ptr = m_caches.m_concept_cache->at(concept.compute_repr());
    std::unique_ptr<element::Role> result_ptr = std::make_unique<element::RestrictRole>(*m_vocabulary_info, role_ptr, concept_ptr);
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(std::move(result_ptr)));
}

Role SyntacticElementFactoryImpl::make_top_role() {
    std::unique_ptr<element::Role> result_ptr = std::make_unique<element::TopRole>(*m_vocabulary_info);
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(std::move(result_ptr)));
}

Role SyntacticElementFactoryImpl::make_transitive_closure(const Role& role) {
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    std::unique_ptr<element::Role> result_ptr = std::make_unique<element::TransitiveClosureRole>(*m_vocabulary_info, role_ptr);
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(std::move(result_ptr)));
}

Role SyntacticElementFactoryImpl::make_transitive_reflexive_closure(const Role& role) {
    element::Role_Ptr role_ptr = m_caches.m_role_cache->at(role.compute_repr());
    std::unique_ptr<element::Role> result_ptr = std::make_unique<element::TransitiveReflexiveClosureRole>(*m_vocabulary_info, role_ptr);
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(std::move(result_ptr)));
}

const VocabularyInfo* SyntacticElementFactoryImpl::get_vocabulary_info() const {
    return m_vocabulary_info.get();
}

}
