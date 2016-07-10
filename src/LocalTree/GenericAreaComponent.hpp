#pragma once
#include <src/LocalTree/AreaComponent.hpp>

namespace Space
{
namespace LocalTree
{

class GenericAreaComponent final : public AreaComponent
{
        COMPONENT_METADATA("8699d312-3e7f-469b-a691-27e05a28e90e")
    public:
            using model_type = AreaModel;
        GenericAreaComponent(
                const Id<iscore::Component>& cmp,
                OSSIA::Node& parent_node,
                AreaModel& proc,
                const Ossia::LocalTree::DocumentPlugin& doc,
                QObject* paren_objt);
};
}
}
