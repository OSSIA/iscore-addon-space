#pragma once
#include <src/LocalTree/AreaComponent.hpp>
#include <iscore/component/ComponentFactory.hpp>
#include <src/LocalTree/GenericAreaComponent.hpp>

namespace Space
{
namespace LocalTree
{
class ISCORE_PLUGIN_SPACE_EXPORT AreaComponentFactory :
        public iscore::GenericComponentFactory<
        AreaModel,
            Ossia::LocalTree::DocumentPlugin,
            Space::LocalTree::AreaComponentFactory>
{
        ISCORE_ABSTRACT_FACTORY_DECL(
                Space::LocalTree::AreaComponentFactory,
                "b3fef66c-2d9e-4e5a-8023-d200bf122c90")
    public:
        virtual ~AreaComponentFactory();

        virtual AreaComponent* make(
                const Id<iscore::Component>&,
                OSSIA::Node& parent,
                AreaModel& proc,
                const Ossia::LocalTree::DocumentPlugin& doc,
                QObject* paren_objt) const = 0;
};



template<typename Area_T>
class AreaComponentFactory_T :
        public AreaComponentFactory
{
    public:
        AreaComponent* make(
                const Id<iscore::Component>& cmp,
                OSSIA::Node& parent,
                AreaModel& proc,
                const Ossia::LocalTree::DocumentPlugin& doc,
                QObject* paren_objt) const override
        {
            return new Area_T{cmp, parent, proc, doc, paren_objt};
        }

        bool matches(
                AreaModel& p,
                const Ossia::LocalTree::DocumentPlugin&) const override
        {
            return dynamic_cast<Area_T*>(&p);
        }
};

#define SPACE_LOCALTREE_AREA_COMPONENT_FACTORY(FactoryName, Uuid, Model) \
class FactoryName final : \
        public Space::LocalTree::AreaComponentFactory_T<Model> \
{ \
        ISCORE_CONCRETE_FACTORY_DECL(Uuid)  \
};


SPACE_LOCALTREE_AREA_COMPONENT_FACTORY(
        GenericAreaComponentFactory,
        "85f1131b-6c5e-4a77-ab47-66fee3ae64af",
        GenericAreaComponent)

using AreaComponentFactoryList =
    iscore::DefaultedGenericComponentFactoryList<
            AreaModel,
            Ossia::LocalTree::DocumentPlugin,
            Space::LocalTree::AreaComponentFactory,
            Space::LocalTree::GenericAreaComponentFactory>;

}
}
