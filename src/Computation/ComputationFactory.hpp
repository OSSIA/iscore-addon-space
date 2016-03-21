#pragma once
#include <iscore/tools/SettableIdentifier.hpp>
#include <iscore/plugins/customfactory/FactoryInterface.hpp>
#include <src/SpaceContext.hpp>
#include <src/Area/Circle/CircleAreaModel.hpp>
#include <src/Area/Pointer/PointerAreaModel.hpp>
#include <src/Area/Generic/GenericAreaModel.hpp>
#include <QObject>

namespace Space
{
class AreaModel;
class ComputationModel;
class ComputationFactory : public iscore::AbstractFactory<ComputationFactory>
{
            ISCORE_ABSTRACT_FACTORY_DECL(
                    ComputationFactory,
                    "60563b69-d8a2-40d4-93b8-284b142fbf4b")

    public:
        virtual ComputationModel* makeModel(
                const AreaModel& a1,
                const AreaModel& a2,
                const Space::Context& ctx,
                const Id<ComputationModel>& comp,
                QObject* parent) = 0;

        virtual ComputationModel* load(
                const VisitorVariant& data,
                const Space::Context& space,
                QObject* parent) = 0;

};



template<typename Computation_T>
auto make_computation(
        const AreaModel& a1,
        const AreaModel& a2,
        const Context& ctx,
        const Id<ComputationModel>& comp,
        QObject* parent)
{
    switch(a1.type())
    {
        case 1: // Circle
        {
            auto& circle = safe_cast<const CircleAreaModel&>(a1);
            switch(a2.type())
            {
                case 1:
                    return new Computation_T{
                        circle,
                        safe_cast<const CircleAreaModel&>(a2),
                        ctx, comp, parent};
                case 2:
                    return new Computation_T{
                        circle,
                        safe_cast<const PointerAreaModel&>(a2),
                        ctx, comp, parent};
                case 0:
                default:
                    return new Computation_T{a1, a2, ctx.space, comp, parent};
            }
        }
        case 2: // pointer
        {
            auto& pointer = safe_cast<const PointerAreaModel&>(a1);
            switch(a2.type())
            {
                case 1:
                    return new Computation_T{
                        safe_cast<const CircleAreaModel&>(a2),
                        pointer,
                        ctx, comp, parent};
                case 2:
                    return new Computation_T{
                        pointer,
                        safe_cast<const PointerAreaModel&>(a2),
                        ctx, comp, parent};
                case 0:
                default:
                    return new Computation_T{a1, a2, ctx.space, comp, parent};
            }
        }

        case 0: // Generic
        default:
            return new Computation_T{a1, a2, ctx.space, comp, parent};
    }

}

}
