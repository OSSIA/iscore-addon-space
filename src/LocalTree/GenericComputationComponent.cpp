#include "GenericComputationComponent.hpp"
#include <Engine/LocalTree/Scenario/MetadataParameters.hpp>

namespace Space
{
namespace LocalTree
{

GenericComputationComponent::GenericComputationComponent(
        const Id<iscore::Component>& cmp,
        ossia::net::node_base& parent_node,
        ComputationModel& computation,
        const Engine::LocalTree::DocumentPlugin& doc,
        QObject* paren_objt):
    ComputationComponent{parent_node, computation, cmp, "GenericComputationComponent", paren_objt}
{
    ISCORE_TODO;
    /*
    Ossia::LocalTree::make_metadata_node(computation.metadata(), *node(), m_properties, this);

    using namespace GiNaC;
    constexpr auto t = Ossia::convert::MatchingType<double>::val;
    auto node_it = thisNode().emplace(
                       thisNode().children().end(),
                       computation.metadata().name().toStdString(),
                       t,
                       OSSIA::AccessMode::GET);

    m_valueNode = *node_it;
    auto addr = m_valueNode->getAddress();
    addr->setValue(Engine::iscore_to_ossia::toOSSIAValue(
                       ossia::value::fromValue(computation.computation()())));
                       */

}

}
}
