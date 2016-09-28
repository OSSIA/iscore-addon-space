#include "AreaFactory.hpp"
#include <src/Area/SingletonAreaFactoryList.hpp>

namespace Space
{
AreaFactory::~AreaFactory()
{

}


AreaFactoryList::~AreaFactoryList()
{

}

AreaFactoryList::object_type*AreaFactoryList::loadMissing(
        const VisitorVariant& vis,
        const Space::Context& ctx,
        QObject* parent) const
{
    ISCORE_TODO;
    return nullptr;
}
}
