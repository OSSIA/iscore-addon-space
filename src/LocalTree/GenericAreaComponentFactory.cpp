#include "GenericAreaComponentFactory.hpp"
#include <src/LocalTree/GenericAreaComponent.hpp>

namespace Space
{
namespace LocalTree
{
AreaComponent* GenericAreaComponentFactory::make(
        const Id<iscore::Component>& cmp,
        OSSIA::Node& parent,
        AreaModel& proc,
        const Ossia::LocalTree::DocumentPlugin& doc,
        const iscore::DocumentContext& ctx,
        QObject* paren_objt) const
{
    return new GenericAreaComponent{
        cmp, parent, proc, doc, ctx, paren_objt};
}

const GenericAreaComponentFactory::ConcreteFactoryKey& GenericAreaComponentFactory::concreteFactoryKey() const
{
    static const ConcreteFactoryKey name{"85f1131b-6c5e-4a77-ab47-66fee3ae64af"};
    return name;
}

bool GenericAreaComponentFactory::matches(
        AreaModel& p,
        const Ossia::LocalTree::DocumentPlugin&,
        const iscore::DocumentContext&) const
{
    return true;
}

}
}
