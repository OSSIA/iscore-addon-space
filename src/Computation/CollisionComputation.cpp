#include "CollisionComputation.hpp"

#include <src/Space/SpaceModel.hpp>
#include <src/Area/Generic/AreaComputer.hpp>
namespace Space
{

CollisionComputation::CollisionComputation(
        const CircleAreaModel& a1,
        const CircleAreaModel& a2,
        const SpaceModel& space,
        const Id<ComputationModel>& id,
        QObject* parent):
    ComputationModel{a1.id(), a2.id(), space, id, parent},
    m_fun{[&] {
    // TODO we should transform c2 into c1
    auto c1_val = CircleArea::mapToData(a1.currentMapping());
    auto c2_val = CircleArea::mapToData(a2.currentMapping());

    auto c1_poly = a1.invertedTransform().map(circleToPoly<12>(c1_val));
    auto c2_poly = a2.invertedTransform().map(circleToPoly<12>(c2_val));
    return !c1_poly.intersected(c2_poly).isEmpty();
}}
{

}

CollisionComputation::CollisionComputation(
        const CircleAreaModel& a1,
        const PointerAreaModel& a2,
        const SpaceModel& space,
        const Id<ComputationModel>& id,
        QObject* parent):
    ComputationModel{a1.id(), a2.id(), space, id, parent},
    m_fun{[&] {
    auto c_val = CircleArea::mapToData(a1.currentMapping());
    auto p_val = PointerArea::mapToData(a2.currentMapping());

    auto c_poly = a1.invertedTransform().map(circleToPoly<12>(c_val));
    return c_poly.contains(a2.invertedTransform().map(p_val.center));
}}
{

}

CollisionComputation::CollisionComputation(
        const PointerAreaModel& a1,
        const PointerAreaModel& a2,
        const SpaceModel& space,
        const Id<ComputationModel>& id,
        QObject* parent):
    ComputationModel{a1.id(), a2.id(), space, id, parent},
    m_fun{[&] {
    auto p1_val = PointerArea::mapToData(a1.currentMapping());
    auto p2_val = PointerArea::mapToData(a2.currentMapping());

    return a1.invertedTransform().map(p1_val.center) == a2.invertedTransform().map(p2_val.center);
}}
{

}

CollisionComputation::CollisionComputation(
        const AreaModel& a1,
        const AreaModel& a2,
        const SpaceModel& space,
        const Id<ComputationModel>& id,
        QObject* parent):
    ComputationModel{a1.id(), a2.id(), space, id, parent},
    m_cptr{new MatrixCollisionComputer},
    m_fun{[&] () -> bool {
            auto f1 = Computations::make_funs(a1.formula());
            auto f2 = Computations::make_funs(a2.formula());

            for(auto& f : f1)
            {
                for(auto val : a1.currentMapping())
                {
                    auto str = val.first.toStdString();
                    f.lhs->SetScalarVariableValue(str.c_str(), val.second);
                    f.rhs->SetScalarVariableValue(str.c_str(), val.second);
                }
            }

            for(auto& f : f2)
            {
                for(auto val : a2.currentMapping())
                {
                    auto str = val.first.toStdString();
                    f.lhs->SetScalarVariableValue(str.c_str(), val.second);
                    f.rhs->SetScalarVariableValue(str.c_str(), val.second);
                }
            }

            // Check if a1 == a2 for any value (TODO or more generally if the {f1, f2} system admits a solution)
            m_cptr->computeArea(
                        a1.space().bounds(),
                        a1.spaceMapping(),
                        a2.spaceMapping(),
                        f1, f2,
                        a1.transform(), a2.transform());
          return this->m_currentResult;
    }}
{

    connect(m_cptr, &MatrixCollisionComputer::ready, this,
            [=] (bool res) {
        this->m_currentResult = res;
    }, Qt::QueuedConnection);


}

State::Value CollisionComputation::result() const
{
    return State::Value::fromValue(m_fun());
}

UuidKey<ComputationFactory> CollisionComputation::concreteFactoryKey() const
{
    return "3b48d241-c11b-479e-b188-82cef922a0d9";
}

}
