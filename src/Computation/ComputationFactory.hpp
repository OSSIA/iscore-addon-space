#pragma once
#include <iscore/tools/SettableIdentifier.hpp>
#include <iscore/plugins/customfactory/FactoryInterface.hpp>
#include <src/SpaceContext.hpp>
#include <src/Area/Circle/CircleAreaModel.hpp>
#include <src/Area/Pointer/PointerAreaModel.hpp>
#include <src/Area/Rectangle/Area.hpp>
#include <src/Area/Generic/GenericAreaModel.hpp>
#include <iscore/plugins/customfactory/ModelFactory.hpp>
#include <QObject>

namespace Space
{
class AreaModel;
class ComputationModel;
class ComputationFactory :
   public iscore::AbstractFactory<ComputationFactory>,
   public iscore::GenericModelFactory<
        ComputationModel,
        iscore::MakeArgs<const AreaModel&, const AreaModel&, const Space::Context&, const Id<ComputationModel>&, QObject*>,
        iscore::LoadArgs<const VisitorVariant&, const Space::Context&, QObject*>>
{
        ISCORE_ABSTRACT_FACTORY("60563b69-d8a2-40d4-93b8-284b142fbf4b")
    public:
        virtual ~ComputationFactory();
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
                        ctx.space, comp, parent};
                case 2:
                    return new Computation_T{
                        circle,
                        safe_cast<const PointerAreaModel&>(a2),
                        ctx.space, comp, parent};
                case 3:
                    return new Computation_T{
                        circle,
                        safe_cast<const RectangleAreaModel&>(a2),
                        ctx.space, comp, parent};
                case 0:
                default:
                    return new Computation_T{circle, a2, ctx.space, comp, parent};
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
                        ctx.space, comp, parent};
                case 2:
                    return new Computation_T{
                        pointer,
                        safe_cast<const PointerAreaModel&>(a2),
                        ctx.space, comp, parent};
                case 3:
                    return new Computation_T{
                        safe_cast<const RectangleAreaModel&>(a2),
                        pointer,
                        ctx.space, comp, parent};
                case 0:
                default:
                    return new Computation_T{a1, a2, ctx.space, comp, parent};
            }
        }
        case 3: // rectangle
        {
            auto& rect = safe_cast<const RectangleAreaModel&>(a1);
            switch(a2.type())
            {
                case 1:
                    return new Computation_T{
                        safe_cast<const CircleAreaModel&>(a2),
                        rect,
                        ctx.space, comp, parent};
                case 2:
                    return new Computation_T{
                        rect,
                        safe_cast<const PointerAreaModel&>(a2),
                        ctx.space, comp, parent};
                case 3:
                    return new Computation_T{
                        safe_cast<const RectangleAreaModel&>(a2),
                        rect,
                        ctx.space, comp, parent};
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

template<typename Computation_T>
void load_computation(
        const AreaModel& a1,
        const AreaModel& a2,
        Computation_T& comp)
{
    switch(a1.type())
    {
        case 1: // Circle
        {
            auto& circle = safe_cast<const CircleAreaModel&>(a1);
            switch(a2.type())
            {
                case 1:
                    return comp.setup(
                                circle,
                                safe_cast<const CircleAreaModel&>(a2));
                case 2:
                    return comp.setup(
                                circle,
                                safe_cast<const PointerAreaModel&>(a2));
                case 3:
                    return comp.setup(
                                circle,
                                safe_cast<const RectangleAreaModel&>(a2));
                case 0:
                default:
                    return comp.setup(circle, a2);
            }
        }
        case 2: // pointer
        {
            auto& pointer = safe_cast<const PointerAreaModel&>(a1);
            switch(a2.type())
            {
                case 1:
                    return comp.setup(
                                safe_cast<const CircleAreaModel&>(a2),
                                pointer);
                case 2:
                    return comp.setup(
                                safe_cast<const PointerAreaModel&>(a2),
                                pointer);
                case 3:
                    return comp.setup(
                                safe_cast<const RectangleAreaModel&>(a2),
                                pointer);
                case 0:
                default:
                    return comp.setup(pointer, a2);
            }
        }
        case 3: // rectangle
        {
            auto& rect = safe_cast<const RectangleAreaModel&>(a1);
            switch(a2.type())
            {
                case 1:
                    return comp.setup(
                                safe_cast<const CircleAreaModel&>(a2),
                                rect);
                case 2:
                    return comp.setup(
                                rect,
                                safe_cast<const PointerAreaModel&>(a2));
                case 3:
                    return comp.setup(
                                safe_cast<const RectangleAreaModel&>(a2),
                                rect);
                case 0:
                default:
                    return comp.setup(rect, a2);
            }
        }

        case 0: // Generic
        default:
            return comp.setup(a1, a2);
    }

}


template<typename Model_T>
class ComputationFactory_T : public ComputationFactory
{
    public:
        QString prettyName() const override
        { return Metadata<PrettyName_k, Model_T>::get(); }

        UuidKey<Space::ComputationFactory> concreteFactoryKey() const override
        { return Metadata<ConcreteFactoryKey_k, Model_T>::get(); }

        Model_T* make(
                const AreaModel& a1,
                const AreaModel& a2,
                const Space::Context& ctx,
                const Id<ComputationModel>& comp,
                QObject* parent) override
        {
            return make_computation<Model_T>(a1, a2, ctx, comp, parent);
        }

        Model_T* load(
                const VisitorVariant& vis,
                const Context& space,
                QObject* parent) override
        {
            return deserialize_dyn(vis, [&] (auto&& deserializer)
            { return new Model_T{deserializer, space, parent};} );
        }
};

}
#define COMPUTATION_FACTORY(FactoryName, Uuid, Model) \
class FactoryName final : \
        public Space::ComputationFactory_T<Model> \
{ \
        ISCORE_CONCRETE_FACTORY(Uuid)  \
};

Q_DECLARE_METATYPE(UuidKey<Space::ComputationFactory>)
