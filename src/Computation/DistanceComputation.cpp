#include "DistanceComputation.hpp"

#include <src/Space/SpaceModel.hpp>
#include <src/Area/Generic/AreaComputer.hpp>
#include <src/SpaceProcess.hpp>
#include <src/Computation/ComputationFactory.hpp>
static double distance(QPointF v1, QPointF v2)
{
    return std::sqrt(std::pow(v2.x() - v1.x(), 2) + std::pow(v2.y() - v1.y(), 2));
}

namespace Space
{

void DistanceComputation::setup(const CircleAreaModel& a1, const CircleAreaModel& a2)
{
    m_fun = [&] {
        auto c1_val = CircleArea::mapToData(a1.currentMapping());
        auto c2_val = CircleArea::mapToData(a2.currentMapping());

        auto c1_center = a1.invertedTransform().map(c1_val.center);
        auto c2_center = a2.invertedTransform().map(c2_val.center);

        return distance(c1_center, c2_center);
    };
}

void DistanceComputation::setup(const CircleAreaModel& a1, const PointerAreaModel& a2)
{
    m_fun = [&] {
        auto c_val = CircleArea::mapToData(a1.currentMapping());
        auto p_val = PointerArea::mapToData(a2.currentMapping());

        auto c_center = a1.invertedTransform().map(c_val.center);
        auto p_center = a2.invertedTransform().map(p_val.center);

        return distance(c_center, p_center);
    };
}

void DistanceComputation::setup(const CircleAreaModel& a1, const RectangleAreaModel& a2)
{
    m_fun = [&] {
        auto circle = CircleArea::mapToData(a1.currentMapping()).center;
        auto rect = RectangleArea::mapToData(a2.currentMapping()).rect.center();

        auto c_center = a1.invertedTransform().map(circle);
        auto r_center = a2.invertedTransform().map(rect);

        return distance(c_center, r_center);
    };

}

void DistanceComputation::setup(const PointerAreaModel& a1, const PointerAreaModel& a2)
{
    m_fun = [&] {
        auto p1_val = PointerArea::mapToData(a1.currentMapping());
        auto p2_val = PointerArea::mapToData(a2.currentMapping());

        auto p1_center = a1.invertedTransform().map(p1_val.center);
        auto p2_center = a2.invertedTransform().map(p2_val.center);

        return distance(p1_center, p2_center);
    };
}

void DistanceComputation::setup(const RectangleAreaModel& a1, const PointerAreaModel& a2)
{
    m_fun = [&] {
        auto r_val = RectangleArea::mapToData(a1.currentMapping());
        auto p_val = PointerArea::mapToData(a2.currentMapping());

        auto r_center = a1.invertedTransform().map(r_val.rect.center());
        auto p_center = a2.invertedTransform().map(p_val.center);

        return distance(r_center, p_center);
    };
}

void DistanceComputation::setup(const RectangleAreaModel& a1, const RectangleAreaModel& a2)
{
    m_fun = [&] {
        auto p1_val = RectangleArea::mapToData(a1.currentMapping()).rect;
        auto p2_val = RectangleArea::mapToData(a2.currentMapping()).rect;

        auto p1_center = a1.invertedTransform().map(p1_val.center());
        auto p2_center = a2.invertedTransform().map(p2_val.center());

        return distance(p1_center, p2_center);
    };

}

void DistanceComputation::setup(const AreaModel& a1, const AreaModel& a2)
{
     m_cptr = new MatrixDistanceComputer;
     m_fun = [&] () -> double {
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
           return m_currentResult;
     };

     connect(m_cptr, &MatrixDistanceComputer::ready, this,
             [=] (double res) {
         this->m_currentResult = res;
     }, Qt::QueuedConnection);
}


State::Value DistanceComputation::result() const
{
    return State::Value::fromValue(m_fun());
}

UuidKey<ComputationFactory> DistanceComputation::concreteFactoryKey() const
{
    return_uuid("2437bda3-6c38-46b5-bfac-9661bfcd5bec");
}

void DistanceComputation::loadAreas(const ProcessModel& p)
{
    auto& a1 = p.areas.at(this->m_a1);
    auto& a2 = p.areas.at(this->m_a2);

    load_computation(a1, a2, *this);
}
}
