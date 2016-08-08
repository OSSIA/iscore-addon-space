#pragma once
#include <src/LocalTree/AreaComponentFactory.hpp>
#include <src/LocalTree/ComputationComponentFactory.hpp>

#include <Engine/LocalTree/Scenario/ProcessComponent.hpp>

#include <iscore_plugin_space_export.h>

namespace Space
{
namespace LocalTree
{
class ProcessLocalTree final :
        public Engine::LocalTree::ProcessComponent_T<Space::ProcessModel>
{
        COMPONENT_METADATA("08313626-75a8-406e-86fe-b83985c26373")

        using hierarchy_t =
           SpaceProcessComponentHierarchyManager<
               ProcessLocalTree,
               AreaComponent,
               ComputationComponent,
               AreaComponentFactoryList,
               ComputationComponentFactoryList,
              Engine::LocalTree::DocumentPlugin
        >;

     public:
        ProcessLocalTree(
                const Id<iscore::Component>& id,
                ossia::net::node_base& parent,
                Space::ProcessModel& process,
                Engine::LocalTree::DocumentPlugin& doc,
                QObject* parent_obj);

        template<typename Component_T, typename Element_T, typename Factory_T>
        Component_T* make(
                const Id<iscore::Component>& id,
                Factory_T&,
                Element_T& elt);

        void removing(
                const AreaModel& elt,
                const AreaComponent& comp);
        void removing(
                const ComputationModel& elt,
                const ComputationComponent& comp);

        ossia::net::node_base& m_areas;
        ossia::net::node_base& m_computations;

        std::vector<std::unique_ptr<Engine::LocalTree::BaseProperty>> m_properties;

        hierarchy_t m_hm;
};

using ProcessLocalTreeFactory = Engine::LocalTree::ProcessComponentFactory_T<ProcessLocalTree>;
}
}
