#pragma once
#include <src/LocalTree/ComputationComponent.hpp>
#include <OSSIA/LocalTree/LocalTreeDocumentPlugin.hpp>
#include <iscore/component/ComponentFactory.hpp>

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
        ISCORE_ABSTRACT_FACTORY_DECL(
                Space::LocalTree::ComputationComponentFactory,
                "7649bf56-5809-4b4c-9bfc-3d15a01ab90c")
    public:
        virtual ~ComputationComponentFactory();

        virtual ComputationComponent* make(
                const Id<iscore::Component>&,
                OSSIA::Node& parent,
                ComputationModel& proc,
                const Ossia::LocalTree::DocumentPlugin& doc,
                const iscore::DocumentContext& ctx,
                QObject* paren_objt) const = 0;
};

// TODO return Generic by default
using ComputationComponentFactoryList =
    iscore::GenericComponentFactoryList<
            ComputationModel,
            Ossia::LocalTree::DocumentPlugin,
            Space::LocalTree::ComputationComponentFactory>;

}
}
