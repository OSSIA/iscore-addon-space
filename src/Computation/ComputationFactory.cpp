#include "ComputationFactory.hpp"
#include "ComputationFactoryList.hpp"

namespace Space
{
ComputationFactory::~ComputationFactory() = default;

ComputationFactoryList::~ComputationFactoryList()
{

}


ComputationFactoryList::object_type* ComputationFactoryList::loadMissing(
        const VisitorVariant& vis,
        const Space::Context& ctx,
        QObject* parent) const
{
    ISCORE_TODO;
    return nullptr;
}
}
