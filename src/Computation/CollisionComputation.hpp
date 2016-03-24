#pragma once
#include <src/Computation/ComputationModel.hpp>
#include <src/Area/Circle/CircleAreaModel.hpp>
#include <src/Area/Pointer/PointerAreaModel.hpp>
#include <src/Area/Rectangle/Area.hpp>
#include <src/Area/Generic/GenericAreaModel.hpp>
#include <atomic>
namespace Space
{
class MatrixCollisionComputer;
class CollisionComputation :
        public ComputationModel
{
    public:
        // Circle -> ...
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
                const CircleAreaModel& a1,
                const RectangleAreaModel& a2,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent);

        // Pointer -> ...
        CollisionComputation(
                const PointerAreaModel& a1,
                const PointerAreaModel& a2,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent);

        // Rectangle -> ...
        CollisionComputation(
                const RectangleAreaModel& a2,
                const PointerAreaModel& a1,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent);

        CollisionComputation(
                const RectangleAreaModel& a1,
                const RectangleAreaModel& a2,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent);

        // Generic
        CollisionComputation(
                const AreaModel& a1,
                const AreaModel& a2,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent);

        State::Value result() const override;
        UuidKey<ComputationFactory> concreteFactoryKey() const override;

    private:
        MatrixCollisionComputer* m_cptr{};
        std::function<bool()> m_fun;

        std::atomic_bool m_currentResult{false};
};
}
