#pragma once
#include <src/Computation/ComputationModel.hpp>
#include <src/Area/Circle/CircleAreaModel.hpp>
#include <src/Area/Pointer/PointerAreaModel.hpp>
#include <src/Area/Generic/GenericAreaModel.hpp>
namespace Space
{
class MatrixDistanceComputer;
class DistanceComputation :
        public ComputationModel
{
    public:
        DistanceComputation(
                const CircleAreaModel& a1,
                const CircleAreaModel& a2,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent);

        DistanceComputation(
                const CircleAreaModel& a1,
                const PointerAreaModel& a2,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent);

        DistanceComputation(
                const PointerAreaModel& a1,
                const PointerAreaModel& a2,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent);

        DistanceComputation(
                const AreaModel& a1,
                const AreaModel& a2,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent);

        State::Value result() const override;
        UuidKey<ComputationFactory> concreteFactoryKey() const override;

    private:
        MatrixDistanceComputer* m_cptr{};
        std::function<double()> m_fun;

        std::atomic<double> m_currentResult{false};
};
}
