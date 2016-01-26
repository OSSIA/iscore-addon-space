#pragma once
#include <src/LocalTree/AreaComponent.hpp>
#include <iscore/component/ComponentFactory.hpp>

namespace Space
{
namespace LocalTree
{
class ISCORE_PLUGIN_SPACE_EXPORT AreaComponentFactory :
        public iscore::GenericComponentFactory<
        AreaModel,
            Ossia::LocalTree::DocumentPlugin,
            Space::LocalTree::AreaComponent>
{
        ISCORE_ABSTRACT_FACTORY_DECL(
                Space::LocalTree::AreaComponent,
                "b3fef66c-2d9e-4e5a-8023-d200bf122c90")
    public:
        virtual ~AreaComponentFactory();

        virtual AreaComponent* make(
                const Id<iscore::Component>&,
                OSSIA::Node& parent,
                AreaModel& proc,
                const Ossia::LocalTree::DocumentPlugin& doc,
                const iscore::DocumentContext& ctx,
                QObject* paren_objt) const = 0;
};



// TODO return Generic by default
using AreaComponentFactoryList =
    iscore::GenericComponentFactoryList<
            AreaModel,
            Ossia::LocalTree::DocumentPlugin,
            Space::LocalTree::AreaComponent,
            Space::LocalTree::AreaComponentFactory>;

}
}
