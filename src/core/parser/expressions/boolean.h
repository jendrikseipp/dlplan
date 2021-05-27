#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_H_

#include "expression.h"

#include "../../elements/boolean.h"


namespace dlp {
namespace core {
namespace parser {

class BooleanExpression : public Expression {
protected:
    /**
     * Construct the BooleanElement.
     */
    virtual element::BooleanElement_Ptr parse_boolean_element_impl(const InstanceInfoImpl& info, ElementCache &cache) const = 0;

public:
    BooleanExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the BooleanElement.
     */
    virtual element::BooleanElement_Ptr parse_boolean_element(const InstanceInfoImpl& info, ElementCache &cache) const {
        element::BooleanElement_Ptr value = parse_boolean_element_impl(info, cache);
        return cache.boolean_element_cache().insert_cache_and_retrieve(value->repr(), std::move(value));
    }
};

}
}
}

#endif
