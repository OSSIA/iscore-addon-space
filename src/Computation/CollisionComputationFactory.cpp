#include "CollisionComputationFactory.hpp"
#include "CollisionComputation.hpp"

namespace Space
{
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
    ISCORE_TODO;
    return nullptr;
}

}
