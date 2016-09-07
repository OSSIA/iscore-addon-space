#include "ComputationComponent.hpp"

namespace Space
{
namespace LocalTree
{

ComputationComponent::ComputationComponent(
        ossia::net::node_base& node,
        ComputationModel& computation,
        const Id<iscore::Component>& id,
        const QString& name,
        QObject* parent):
    Component{id, name, parent},
    m_thisNode{node, computation.metadata(), this}
{
}

ComputationComponent::~ComputationComponent()
{

}

ossia::net::node_base& ComputationComponent::node() const
{ return m_thisNode.node; }

ossia::net::node_base& ComputationComponent::thisNode() const
{ return node(); }

}
}

