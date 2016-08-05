#pragma once
#include <OSSIA/LocalTree/NameProperty.hpp>
#include <src/Computation/ComputationModel.hpp>

#include <iscore/tools/Metadata.hpp>
#include <iscore_plugin_space_export.h>

namespace Space
{
namespace LocalTree
{
class ISCORE_PLUGIN_SPACE_EXPORT ComputationComponent : public iscore::Component
{
    public:
        ComputationComponent(
                ossia::net::node_base& node,
                ComputationModel& Computation,
                const Id<iscore::Component>& id,
                const QString& name,
                QObject* parent);

        virtual ~ComputationComponent();

        ossia::net::node_base& node() const;

    protected:
        ossia::net::node_base& thisNode() const;

    private:
        MetadataNamePropertyWrapper m_thisNode;

    protected:
        std::vector<std::unique_ptr<Ossia::LocalTree::BaseProperty>> m_properties;
};

}
}

DEFAULT_MODEL_METADATA(Space::LocalTree::ComputationComponent, "Space local tree computation component")
