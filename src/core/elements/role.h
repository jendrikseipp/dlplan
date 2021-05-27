#ifndef DLP_SRC_CORE_ELEMENTS_ROLE_H_
#define DLP_SRC_CORE_ELEMENTS_ROLE_H_

#include "element.h"


namespace dlp {
namespace core {
namespace element {

class RoleElement : public Element<Roles> {
public:
    RoleElement(const InstanceInfoImpl& parent, const std::string& name) : Element<Roles>(parent, name) { }

    size_t size() const {
        return m_result.size();
    }
};

}
}
}

#endif
