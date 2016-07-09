#pragma once
#include <src/LocalTree/ComputationComponentFactory.hpp>

namespace Space
{
namespace LocalTree
{

// It must be last in the vector
class GenericComputationComponentFactory final
        : public ComputationComponentFactory
{
    private:
        ComputationComponent* make(
                        const Id<iscore::Component>& cmp,
                        OSSIA::Node& parent,
                        ComputationModel& proc,
                        const Ossia::LocalTree::DocumentPlugin& doc,
                        QObject* paren_objt) const override;

        const ConcreteFactoryKey& concreteFactoryKey() const override;

        bool matches(
                ComputationModel& p,
                const Ossia::LocalTree::DocumentPlugin&) const override;
};

}
}
