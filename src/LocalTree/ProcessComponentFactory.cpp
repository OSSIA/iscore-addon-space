#include "ProcessComponentFactory.hpp"
#include <src/SpaceProcess.hpp>
#include <src/LocalTree/ProcessComponent.hpp>
namespace Space
{
namespace LocalTree
{

ProcessLocalTreeFactory::~ProcessLocalTreeFactory()
{

}

const ProcessLocalTreeFactory::ConcreteFactoryKey&
ProcessLocalTreeFactory::concreteFactoryKey() const
{
    static const ConcreteFactoryKey name{"91079e65-9ba5-4d31-809a-fb79dc393b71"};
    return name;

}

bool ProcessLocalTreeFactory::matches(
        Process::ProcessModel& p,
        const Ossia::LocalTree::DocumentPlugin&,
        const iscore::DocumentContext&) const
{
    return dynamic_cast<Space::ProcessModel*>(&p);
}


Ossia::LocalTree::ProcessComponent* ProcessLocalTreeFactory::make(
        const Id<iscore::Component>& id,
        OSSIA::Node& parent,
        Process::ProcessModel& proc,
        const Ossia::LocalTree::DocumentPlugin& doc,
        const iscore::DocumentContext& ctx,
        QObject* paren_objt) const
{
    return new ProcessLocalTree{
        id, parent,
                static_cast<Space::ProcessModel&>(proc),
                doc, ctx, paren_objt};
}
}
}
