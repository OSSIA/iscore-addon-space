#pragma once
#include <iscore/plugins/customfactory/FactoryFamily.hpp>
#include <src/Area/AreaFactory.hpp>
#include <iscore_plugin_space_export.h>

namespace Space
{
class ISCORE_PLUGIN_SPACE_EXPORT AreaFactoryList final :
        public iscore::ConcreteFactoryList<AreaFactory>
{
    public:
        using object_type = Space::AreaModel;
        virtual ~AreaFactoryList();
        object_type* loadMissing(
                const VisitorVariant& vis,
                const Space::Context& ctx,
                QObject* parent) const;

};

}
