#pragma once
#include <src/LocalTree/ComputationComponent.hpp>
#include <OSSIA/LocalTree/LocalTreeDocumentPlugin.hpp>
#include <iscore/component/ComponentFactory.hpp>
#include <src/LocalTree/GenericComputationComponent.hpp>

namespace Space
{
namespace LocalTree
{

class ISCORE_PLUGIN_SPACE_EXPORT ComputationComponentFactory :
        public iscore::GenericComponentFactory<
            ComputationModel,
            Ossia::LocalTree::DocumentPlugin,
            Space::LocalTree::ComputationComponentFactory>
{
        ISCORE_ABSTRACT_FACTORY("7649bf56-5809-4b4c-9bfc-3d15a01ab90c")
    public:
        virtual ~ComputationComponentFactory();

        virtual ComputationComponent* make(
                const Id<iscore::Component>&,
                OSSIA::Node& parent,
                ComputationModel& proc,
                const Ossia::LocalTree::DocumentPlugin& doc,
                QObject* paren_objt) const = 0;
};

template<typename Computation_T>
class ComputationComponentFactory_T :
        public ComputationComponentFactory
{
    public:
        ComputationComponent* make(
                const Id<iscore::Component>& cmp,
                OSSIA::Node& parent,
                ComputationModel& proc,
                const Ossia::LocalTree::DocumentPlugin& doc,
                QObject* paren_objt) const override
        {
            return new Computation_T{cmp, parent, proc, doc, paren_objt};
        }

        bool matches(
                ComputationModel& p,
                const Ossia::LocalTree::DocumentPlugin&) const override
        {
            return dynamic_cast<Computation_T*>(&p);
        }
};

#define SPACE_LOCALTREE_COMPUTATION_COMPONENT_FACTORY(FactoryName, Uuid, Model) \
class FactoryName final : \
        public Space::LocalTree::ComputationComponentFactory_T<Model> \
{ \
        ISCORE_CONCRETE_FACTORY(Uuid)  \
};

SPACE_LOCALTREE_COMPUTATION_COMPONENT_FACTORY(GenericComputationComponentFactory, "133fdabd-bec5-4359-aab6-df0177b6761b", GenericComputationComponent)

using ComputationComponentFactoryList =
    iscore::DefaultedGenericComponentFactoryList<
            ComputationModel,
            Ossia::LocalTree::DocumentPlugin,
            Space::LocalTree::ComputationComponentFactory,
            Space::LocalTree::GenericComputationComponentFactory>;
}
}
