#ifndef DLPLAN_SRC_CORE_NUMERICAL_H_
#define DLPLAN_SRC_CORE_NUMERICAL_H_

#include "element.h"


namespace dlplan::core {

class NumericalImpl : public ElementImpl<int> {
public:
    NumericalImpl(const std::shared_ptr<element::Element<int>>& element)  {
        if (!element) {
            throw std::runtime_error("NumericalImpl::NumericalImpl - tried to construct Numerical from nullptr");
        }
    }
    ~NumericalImpl() override = default;

    int evaluate(const Element<int>* parent, const State& state) const override {
        if (state.get_instance_info()->get_vocabulary_info() != parent->get_vocabulary_info()) {
            throw std::runtime_error("NumericalImpl::evaluate - mismatched vocabularies of Numerical and State.");
        }
        return parent->get_element()->evaluate(state);
    }
};

}

#endif
