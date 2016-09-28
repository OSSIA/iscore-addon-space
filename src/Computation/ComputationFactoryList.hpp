#pragma once
#include <src/Computation/ComputationFactory.hpp>
#include <iscore/plugins/customfactory/FactoryFamily.hpp>
#include <iscore_plugin_space_export.h>
namespace Space
{
class ISCORE_PLUGIN_SPACE_EXPORT ComputationFactoryList final :
        public iscore::ConcreteFactoryList<ComputationFactory>
{
    public:
        using object_type = Space::ComputationModel;
        virtual ~ComputationFactoryList();
        object_type* loadMissing(
                const VisitorVariant& vis,
                const Space::Context& ctx,
                QObject* parent) const;
};
}
