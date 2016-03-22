#pragma once
#include <src/Computation/ComputationModel.hpp>
#include <src/Area/Circle/CircleAreaModel.hpp>
#include <src/Area/Pointer/PointerAreaModel.hpp>
#include <src/Area/Generic/GenericAreaModel.hpp>
namespace Space
{
class MatrixCollisionComputer;
class CollisionComputation :
        public ComputationModel
{
    public:
        CollisionComputation(
                const CircleAreaModel& a1,
                const CircleAreaModel& a2,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent);

        CollisionComputation(
                const CircleAreaModel& a1,
                const PointerAreaModel& a2,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent);

        CollisionComputation(
                const PointerAreaModel& a1,
                const PointerAreaModel& a2,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent);

        CollisionComputation(
                const AreaModel& a1,
                const AreaModel& a2,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent);

        double result() const override;
        UuidKey<ComputationFactory> concreteFactoryKey() const override;

    private:
        MatrixCollisionComputer* m_cptr{};
        std::function<double()> m_fun;

        std::atomic_bool m_currentResult{false};
};
}
