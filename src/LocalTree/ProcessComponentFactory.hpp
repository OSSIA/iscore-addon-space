#pragma once
#include <OSSIA/LocalTree/Scenario/ProcessComponent.hpp>

namespace Space
{
namespace LocalTree
{

class ProcessLocalTreeFactory final :
        public Ossia::LocalTree::ProcessComponentFactory
{
    public:
        virtual ~ProcessLocalTreeFactory();
        const ConcreteFactoryKey& concreteFactoryKey() const override;

        bool matches(
                Process::ProcessModel& p,
                const Ossia::LocalTree::DocumentPlugin&) const override;

        Ossia::LocalTree::ProcessComponent* make(
                const Id<iscore::Component>& id,
                OSSIA::Node& parent,
                Process::ProcessModel& proc,
                Ossia::LocalTree::DocumentPlugin& doc,
                QObject* paren_objt) const override;
};

}
}
