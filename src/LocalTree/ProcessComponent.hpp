#pragma once
#include <src/LocalTree/AreaComponentFactory.hpp>
#include <src/LocalTree/ComputationComponentFactory.hpp>

#include <OSSIA/LocalTree/Scenario/ProcessComponent.hpp>

#include <iscore_plugin_space_export.h>

namespace Space
{
namespace LocalTree
{
class ProcessLocalTree final :
        public Ossia::LocalTree::ProcessComponent_T<Space::ProcessModel>
{
        COMPONENT_METADATA("08313626-75a8-406e-86fe-b83985c26373")

        using hierarchy_t =
           SpaceProcessComponentHierarchyManager<
               ProcessLocalTree,
               AreaComponent,
               ComputationComponent,
               AreaComponentFactoryList,
               ComputationComponentFactoryList,
              Ossia::LocalTree::DocumentPlugin
        >;

     public:
        ProcessLocalTree(
                const Id<iscore::Component>& id,
                OSSIA::Node& parent,
                Space::ProcessModel& process,
                Ossia::LocalTree::DocumentPlugin& doc,
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

        std::shared_ptr<OSSIA::Node> m_areas;
        std::shared_ptr<OSSIA::Node> m_computations;

        std::vector<std::unique_ptr<Ossia::LocalTree::BaseProperty>> m_properties;

        hierarchy_t m_hm;
};

using ProcessLocalTreeFactory = Ossia::LocalTree::ProcessComponentFactory_T<ProcessLocalTree>;
}
}
