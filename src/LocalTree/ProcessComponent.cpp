#include "ProcessComponent.hpp"

namespace Space
{
namespace LocalTree
{

ProcessLocalTree::ProcessLocalTree(
        const Id<iscore::Component>& id,
        ossia::net::node_base& parent,
        ProcessModel& process,
        Ossia::LocalTree::DocumentPlugin& doc,
        QObject* parent_obj):
    Ossia::LocalTree::ProcessComponent_T<Space::ProcessModel>{parent, process, doc, id, "SpaceComponent", parent_obj},
    m_areas{add_node(*node(), "areas")},
    m_computations{add_node(*node(), "computations")},
    m_hm{*this, process, doc, this}
{
    Ossia::LocalTree::make_metadata_node(process.metadata, *node(), m_properties, this);
}

template<>
AreaComponent* ProcessLocalTree::make<AreaComponent, AreaModel, AreaComponentFactory>(
        const Id<iscore::Component>& id,
        AreaComponentFactory& fact,
        AreaModel& elt)
{
    return fact.make(id, *m_areas, elt, system(), this);
}

template<>
ComputationComponent* ProcessLocalTree::make<ComputationComponent, ComputationModel, ComputationComponentFactory>(
        const Id<iscore::Component>& id,
        ComputationComponentFactory& fact,
        ComputationModel& elt)
{
    return fact.make(id, *m_computations, elt, system(), this);
}

void ProcessLocalTree::removing(const AreaModel& elt, const AreaComponent& comp)
{
    auto it = find_if(m_areas->children(), [&] (const auto& node)
    { return node == comp.node(); });
    ISCORE_ASSERT(it != m_areas->children().end());

    m_areas->erase(it);
}

void ProcessLocalTree::removing(const ComputationModel& elt, const ComputationComponent& comp)
{
    auto it = find_if(m_computations->children(), [&] (const auto& node)
    { return node == comp.node(); });
    ISCORE_ASSERT(it != m_computations->children().end());

    m_areas->erase(it);
}
}
}
