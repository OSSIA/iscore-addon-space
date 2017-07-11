#include "CollisionComputation.hpp"

#include <src/Space/SpaceModel.hpp>
#include <src/Area/Generic/AreaComputer.hpp>
#include <src/SpaceProcess.hpp>
#include <src/Computation/ComputationFactory.hpp>
namespace Space
{

void CollisionComputation::setup(const CircleAreaModel& a1, const CircleAreaModel& a2)
{
    m_fun = [&] {
        // TODO we should transform c2 into c1
        auto c1_val = CircleArea::mapToData(a1.currentMapping());
        auto c2_val = CircleArea::mapToData(a2.currentMapping());

        QPolygonF c1_poly = a1.invertedTransform().map(circleToPoly<12>(c1_val));
        QPolygonF c2_poly = a2.invertedTransform().map(circleToPoly<12>(c2_val));
        return !c1_poly.intersected(c2_poly).isEmpty();
    };
}

void CollisionComputation::setup(const CircleAreaModel& a1, const PointerAreaModel& a2)
{
    m_fun = [&] {
        auto c_val = CircleArea::mapToData(a1.currentMapping());
        auto p_val = PointerArea::mapToData(a2.currentMapping());

        auto c_poly = a1.invertedTransform().map(circleToPoly<12>(c_val));
        return c_poly.contains(a2.invertedTransform().map(p_val.center));
    };
}

void CollisionComputation::setup(const CircleAreaModel& a1, const RectangleAreaModel& a2)
{
    m_fun = [&] {
        auto c1_val = CircleArea::mapToData(a1.currentMapping());
        auto p2_rect = RectangleArea::mapToData(a2.currentMapping()).rect;

        auto c1_poly = a1.invertedTransform().map(circleToPoly<12>(c1_val));
        return !c1_poly.intersected(a2.invertedTransform().mapRect(p2_rect)).isEmpty();
    };

}

void CollisionComputation::setup(const PointerAreaModel& a1, const PointerAreaModel& a2)
{
    m_fun = [&] {
        auto p1_val = PointerArea::mapToData(a1.currentMapping());
        auto p2_val = PointerArea::mapToData(a2.currentMapping());

        return a1.invertedTransform().map(p1_val.center) == a2.invertedTransform().map(p2_val.center);
    };
}

void CollisionComputation::setup(const RectangleAreaModel& a1, const PointerAreaModel& a2)
{
    m_fun = [&] {
        auto rect = RectangleArea::mapToData(a1.currentMapping()).rect;
        auto pt = PointerArea::mapToData(a2.currentMapping());

        return a1.invertedTransform().mapRect(rect).contains(a2.invertedTransform().map(pt.center));
    };
}

void CollisionComputation::setup(const RectangleAreaModel& a1, const RectangleAreaModel& a2)
{
    m_fun = [&] {
        auto p1_rect = RectangleArea::mapToData(a1.currentMapping()).rect;
        auto p2_rect = RectangleArea::mapToData(a2.currentMapping()).rect;

        return a1.invertedTransform().mapRect(p1_rect).intersects(a2.invertedTransform().mapRect(p2_rect));
    };

}

void CollisionComputation::setup(const AreaModel& a1, const AreaModel& a2)
{
    m_cptr = new MatrixCollisionComputer;
    m_fun = [&] () -> bool {
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
};

    connect(m_cptr, &MatrixCollisionComputer::ready, this,
            [=] (bool res) {
        this->m_currentResult = res;
    }, Qt::QueuedConnection);
}


ossia::value CollisionComputation::result() const
{
    return ossia::value::fromValue(m_fun());
}

UuidKey<ComputationFactory> CollisionComputation::concreteKey() const
{
    return_uuid("3b48d241-c11b-479e-b188-82cef922a0d9");
}

void CollisionComputation::loadAreas(const ProcessModel& p)
{
    auto& a1 = p.areas.at(this->m_a1);
    auto& a2 = p.areas.at(this->m_a2);

    load_computation(a1, a2, *this);
}

}
