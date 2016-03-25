#include "CollisionComputationFactory.hpp"
#include "CollisionComputation.hpp"

namespace Space
{
QString CollisionComputationFactory::prettyName() const
{
    return QObject::tr("Collision");
}

ComputationModel*CollisionComputationFactory::makeModel(
        const AreaModel& a1,
        const AreaModel& a2,
        const Context& ctx,
        const Id<ComputationModel>& comp,
        QObject* parent)
{
    return make_computation<CollisionComputation>(a1, a2, ctx, comp, parent);
}

ComputationModel*CollisionComputationFactory::load(
        const VisitorVariant& data,
        const Context& space,
        QObject* parent)
{
    return deserialize_dyn(data, [&] (auto&& deserializer)
    { return new CollisionComputation{deserializer, space, parent};} );
}

const UuidKey<ComputationFactory>&CollisionComputationFactory::concreteFactoryKey() const
{
    static const UuidKey<ComputationFactory> k{"3b48d241-c11b-479e-b188-82cef922a0d9"};
    return k;
}

}
