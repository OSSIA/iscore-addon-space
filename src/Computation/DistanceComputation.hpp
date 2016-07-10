#pragma once
#include <src/Computation/ComputationModel.hpp>
#include <src/Area/Circle/CircleAreaModel.hpp>
#include <src/Area/Pointer/PointerAreaModel.hpp>
#include <src/Area/Generic/GenericAreaModel.hpp>
#include <src/Area/Rectangle/Area.hpp>
#include <src/Computation/ComputationFactory.hpp>
#include <atomic>

namespace Space
{
class DistanceComputation;
}

COMPUTATION_METADATA(, Space::DistanceComputation, "2437bda3-6c38-46b5-bfac-9661bfcd5bec", "DistanceComputation", "Distance")
namespace Space
{
class MatrixDistanceComputer;
class DistanceComputation :
        public ComputationModel
{
    public:
        void setup(
                const CircleAreaModel& a1,
                const CircleAreaModel& a2);
        void setup(
                const CircleAreaModel& a1,
                const PointerAreaModel& a2);
        void setup(
                const CircleAreaModel& a1,
                const RectangleAreaModel& a2);
        void setup(
                const PointerAreaModel& a1,
                const PointerAreaModel& a2);
        void setup(
                const RectangleAreaModel& a1,
                const PointerAreaModel& a2);
        void setup(
                const RectangleAreaModel& a1,
                const RectangleAreaModel& a2);
        void setup(
                const AreaModel& a1,
                const AreaModel& a2);

        template<typename Area1, typename Area2>
        DistanceComputation(
                const Area1& a1,
                const Area2& a2,
                const SpaceModel& space,
                const Id<ComputationModel>& id,
                QObject* parent):
            ComputationModel{a1.id(), a2.id(), space, id, parent}
        {
            setup(a1, a2);
        }

        template<typename Deser>
        DistanceComputation(
                Deser& d,
                const Context& space,
                QObject* parent):
            ComputationModel{d, space.space, parent}
        {
            loadAreas(space.process);
        }

        DistanceComputation(
                const DistanceComputation& source,
                const Context& space,
                const Id<ComputationModel>& newId,
                QObject* parent):
            ComputationModel{source, space.space, newId, parent}
        {
            loadAreas(space.process);
        }

        DistanceComputation* clone(
                        const Context& space,
                        const Id<ComputationModel>& newId,
                        QObject* parent) const override
        {
            return new DistanceComputation{*this, space, newId, parent};
        }



        State::Value result() const override;
        UuidKey<ComputationFactory> concreteFactoryKey() const override;

    private:
        void loadAreas(const Space::ProcessModel&);
        MatrixDistanceComputer* m_cptr{};
        std::function<double()> m_fun;

        std::atomic<double> m_currentResult{false};
};
COMPUTATION_FACTORY(DistanceComputationFactory, "2437bda3-6c38-46b5-bfac-9661bfcd5bec", DistanceComputation)
}
