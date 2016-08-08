#pragma once
#include <src/LocalTree/ComputationComponent.hpp>
#include <Engine/LocalTree/LocalTreeDocumentPlugin.hpp>
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
                ossia::net::node_base& parent_node,
                ComputationModel& proc,
                const Engine::LocalTree::DocumentPlugin& doc,
                QObject* paren_objt);
};

}
}
