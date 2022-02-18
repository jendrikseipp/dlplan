#include "../include/dlplan/policy.h"

#include "condition.h"
#include "effect.h"
#include "policy_builder.h"
#include "reader.h"
#include "writer.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {


PolicyRoot::PolicyRoot() { }

PolicyRoot::~PolicyRoot() { }

BooleanFeature::BooleanFeature(std::shared_ptr<const PolicyRoot> root, int index, core::Boolean&& boolean)
    : Feature<bool>(root, index), m_boolean(std::move(boolean)) { }

BooleanFeature::BooleanFeature(BooleanFeature&& other) = default;

BooleanFeature& BooleanFeature::operator=(BooleanFeature&& other) = default;

BooleanFeature::~BooleanFeature() = default;

bool BooleanFeature::evaluate(evaluator::EvaluationContext& context) const {
    return context.cache.retrieve_or_evaluate(get_index(), get_boolean(), context);
}

std::string BooleanFeature::compute_repr() const {
    return m_boolean.compute_repr();
}

std::string BooleanFeature::str() const {
    return compute_repr();
}

const core::Boolean& BooleanFeature::get_boolean() const {
    return m_boolean;
}

NumericalFeature::NumericalFeature(NumericalFeature&& other) = default;

NumericalFeature& NumericalFeature::operator=(NumericalFeature&& other) = default;

NumericalFeature::~NumericalFeature() = default;

NumericalFeature::NumericalFeature(std::shared_ptr<const PolicyRoot> root, int index, core::Numerical&& numerical)
    : Feature<int>(root, index), m_numerical(std::move(numerical)) { }

int NumericalFeature::evaluate(evaluator::EvaluationContext& context) const {
    return context.cache.retrieve_or_evaluate(get_index(), get_numerical(), context);
}

std::string NumericalFeature::compute_repr() const {
    return m_numerical.compute_repr();
}

std::string NumericalFeature::str() const {
    return compute_repr();
}

const core::Numerical& NumericalFeature::get_numerical() const {
    return m_numerical;
}

BaseCondition::BaseCondition(BaseCondition&& other) = default;

BaseCondition& BaseCondition::operator=(BaseCondition&& other) = default;

BaseCondition::~BaseCondition() = default;

std::shared_ptr<const PolicyRoot> BaseCondition::get_root() const {
    return m_root;
}

std::string BaseCondition::str() const {
    return compute_repr();
}

BaseEffect::BaseEffect(BaseEffect&& other) = default;

BaseEffect& BaseEffect::operator=(BaseEffect&& other) = default;

BaseEffect::~BaseEffect() = default;

std::shared_ptr<const PolicyRoot> BaseEffect::get_root() const {
    return m_root;
}

std::string BaseEffect::str() const {
    return compute_repr();
}


PolicyBuilder::PolicyBuilder() = default;

PolicyBuilder::PolicyBuilder(const PolicyBuilder& other)
    : m_pImpl(*other.m_pImpl) { }

PolicyBuilder& PolicyBuilder::operator=(const PolicyBuilder& other) {
    if (this != &other) {
        m_pImpl = *other.m_pImpl;
    }
    return *this;
}

PolicyBuilder::PolicyBuilder(PolicyBuilder&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

PolicyBuilder& PolicyBuilder::operator=(PolicyBuilder&& other) {
    if (this != &other) {
        m_pImpl = std::move(*other.m_pImpl);
    }
    return *this;
}

PolicyBuilder::~PolicyBuilder() = default;

std::shared_ptr<const BooleanFeature> PolicyBuilder::add_boolean_feature(core::Boolean boolean) {
    return m_pImpl->add_boolean_feature(boolean);
}

std::shared_ptr<const NumericalFeature> PolicyBuilder::add_numerical_feature(core::Numerical numerical) {
    return m_pImpl->add_numerical_feature(numerical);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_b_pos_condition(std::shared_ptr<const BooleanFeature> b) {
    return m_pImpl->add_b_pos_condition(b);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_b_neg_condition(std::shared_ptr<const BooleanFeature> b) {
    return m_pImpl->add_b_neg_condition(b);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_n_gt_condition(std::shared_ptr<const NumericalFeature> n) {
    return m_pImpl->add_n_gt_condition(n);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_n_eq_condition(std::shared_ptr<const NumericalFeature> n) {
    return m_pImpl->add_n_eq_condition(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_b_pos_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_pImpl->add_b_pos_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_b_neg_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_pImpl->add_b_neg_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_b_bot_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_pImpl->add_b_bot_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_n_inc_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_pImpl->add_n_inc_effect(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_n_dec_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_pImpl->add_n_dec_effect(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_n_bot_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_pImpl->add_n_bot_effect(n);
}

std::shared_ptr<const Rule> PolicyBuilder::add_rule(
    std::vector<std::shared_ptr<const BaseCondition>>&& conditions,
    std::vector<std::shared_ptr<const BaseEffect>>&& effects) {
    return m_pImpl->add_rule(std::move(conditions), std::move(effects));
}

Policy PolicyBuilder::get_result() {
    return m_pImpl->get_result();
}


PolicyReader::PolicyReader() = default;

PolicyReader::PolicyReader(const PolicyReader& other)
    : m_pImpl(*other.m_pImpl) { }

PolicyReader& PolicyReader::operator=(const PolicyReader& other) {
    if (this != &other) {
        m_pImpl = *other.m_pImpl;
    }
    return *this;
}

PolicyReader::PolicyReader(PolicyReader&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

PolicyReader& PolicyReader::operator=(PolicyReader&& other) {
    if (this != &other) {
        m_pImpl = std::move(*other.m_pImpl);
    }
    return *this;
}

PolicyReader::~PolicyReader() = default;

Policy PolicyReader::read(const std::string& data, core::SyntacticElementFactory factory) const {
    return m_pImpl->read(data, factory);
}


PolicyWriter::PolicyWriter() { }

PolicyWriter::PolicyWriter(const PolicyWriter& other)
    : m_pImpl(*other.m_pImpl) { }

PolicyWriter& PolicyWriter::operator=(const PolicyWriter& other) {
    if (this != &other) {
        m_pImpl = *other.m_pImpl;
    }
    return *this;
}

PolicyWriter::PolicyWriter(PolicyWriter&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

PolicyWriter& PolicyWriter::operator=(PolicyWriter&& other) {
    if (this != &other) {
        m_pImpl = std::move(*other.m_pImpl);
    }
    return *this;
}

PolicyWriter::~PolicyWriter() { }

std::string PolicyWriter::write(const Policy& policy) const {
    return m_pImpl->write(policy);
}

}
