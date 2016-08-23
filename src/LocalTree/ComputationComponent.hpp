#pragma once
#include <Engine/LocalTree/NameProperty.hpp>
#include <src/Computation/ComputationModel.hpp>

#include <iscore/tools/Metadata.hpp>
#include <iscore_plugin_space_export.h>

namespace Space
{
namespace LocalTree
{
class ISCORE_PLUGIN_SPACE_EXPORT ComputationComponent : public iscore::Component
{
        ABSTRACT_COMPONENT_METADATA(ComputationComponent, "7649bf56-5809-4b4c-9bfc-3d15a01ab90c")
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
        std::vector<std::unique_ptr<Engine::LocalTree::BaseProperty>> m_properties;
};

}
}

DEFAULT_MODEL_METADATA(Space::LocalTree::ComputationComponent, "Space local tree computation component")
