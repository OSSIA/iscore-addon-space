#pragma once
#include <src/Computation/ComputationFactory.hpp>
namespace Space
{

class CollisionComputationFactory : public ComputationFactory
{
    public:
        ComputationModel* makeModel(
                const AreaModel& a1,
                const AreaModel& a2,
                const Space::Context& ctx,
                const Id<ComputationModel>& comp,
                QObject* parent) override;

        ComputationModel* load(
                const VisitorVariant& data,
                const Space::Context& space,
                QObject* parent) override;

        const UuidKey<ComputationFactory>& concreteFactoryKey() const override;


};
}
