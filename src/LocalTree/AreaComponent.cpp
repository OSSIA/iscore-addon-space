#include "AreaComponent.hpp"

namespace Space
{
namespace LocalTree
{

AreaComponent::AreaComponent(
        ossia::net::node_base& node,
        AreaModel& area,
        const Id<iscore::Component>& id,
        const QString& name,
        QObject* parent):
    Component{id, name, parent},
    m_thisNode{node, area.metadata(), this},
    m_area{area}
{
}

AreaComponent::~AreaComponent()
{

}

ossia::net::node_base& AreaComponent::node() const
{ return m_thisNode.node; }

// TODO removeme
ossia::net::node_base& AreaComponent::thisNode() const
{ return node(); }

}

}
