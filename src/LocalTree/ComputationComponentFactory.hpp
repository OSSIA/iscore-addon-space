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

template<typename ComputationComponent_T>
class ComputationComponentFactory_T :
        public ComputationComponentFactory
{
    public:
        using model_type = typename ComputationComponent_T::model_type;
        using component_type = ComputationComponent_T;

        static auto static_concreteFactoryKey()
        {
            return ComputationComponent_T::static_key().impl();
        }

        ConcreteFactoryKey concreteFactoryKey() const final override
        {
            return ComputationComponent_T::static_key().impl(); // Note : here there is a conversion between UuidKey<Component> and ConcreteFactoryKey
        }

        ComputationComponent* make(
                const Id<iscore::Component>& cmp,
                OSSIA::Node& parent,
                ComputationModel& proc,
                const Ossia::LocalTree::DocumentPlugin& doc,
                QObject* paren_objt) const override
        {
            return new ComputationComponent_T{cmp, parent, proc, doc, paren_objt};
        }

        bool matches(const ComputationModel& p) const override;
};

template<typename ComputationComponent_T>
bool ComputationComponentFactory_T<ComputationComponent_T>::matches(
        const ComputationModel& p) const
{
    return dynamic_cast<ComputationComponent_T*>(&p);
}

template<>
inline bool ComputationComponentFactory_T<GenericComputationComponent>::matches(
        const ComputationModel& p) const
{
    return false; // We want it to be the fall-back default case
}


using GenericComputationComponentFactory = ComputationComponentFactory_T<GenericComputationComponent>;

using ComputationComponentFactoryList =
    iscore::DefaultedGenericComponentFactoryList<
            ComputationModel,
            Ossia::LocalTree::DocumentPlugin,
            Space::LocalTree::ComputationComponentFactory,
            Space::LocalTree::GenericComputationComponentFactory>;
}
}
