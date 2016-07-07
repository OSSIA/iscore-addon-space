#pragma once
#include <src/LocalTree/ComputationComponent.hpp>
#include <OSSIA/LocalTree/LocalTreeDocumentPlugin.hpp>
namespace Space
{
namespace LocalTree
{


class GenericComputationComponent final : public ComputationComponent
{
        COMPONENT_METADATA("b568d225-5ec2-41da-a050-cd5437ea85c4")
    public:
        GenericComputationComponent(
                const Id<iscore::Component>& cmp,
                OSSIA::Node& parent_node,
                ComputationModel& proc,
                const Ossia::LocalTree::DocumentPlugin& doc,
                const iscore::DocumentContext& ctx,
                QObject* paren_objt);
};

}
}
