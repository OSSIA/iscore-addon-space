#include "GenericComputationComponentFactory.hpp"
#include <src/LocalTree/GenericComputationComponent.hpp>
namespace Space
{
namespace LocalTree
{

ComputationComponent*
GenericComputationComponentFactory::make(
        const Id<iscore::Component>& cmp,
        OSSIA::Node& parent,
        ComputationModel& proc,
        const Ossia::LocalTree::DocumentPlugin& doc,
        const iscore::DocumentContext& ctx,
        QObject* paren_objt) const
{
    return new GenericComputationComponent{cmp, parent, proc, doc, ctx, paren_objt};
}

const GenericComputationComponentFactory::ConcreteFactoryKey&
GenericComputationComponentFactory::concreteFactoryKey() const
{
    static const ConcreteFactoryKey name{"133fdabd-bec5-4359-aab6-df0177b6761b"};
    return name;
}

bool GenericComputationComponentFactory::matches(
        ComputationModel& p,
        const Ossia::LocalTree::DocumentPlugin&,
        const iscore::DocumentContext&) const
{
    return true;
}

}
}
