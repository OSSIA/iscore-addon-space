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
        ISCORE_ABSTRACT_FACTORY("b3fef66c-2d9e-4e5a-8023-d200bf122c90")
    public:
        virtual ~AreaComponentFactory();

        virtual AreaComponent* make(
                const Id<iscore::Component>&,
                OSSIA::Node& parent,
                AreaModel& proc,
                const Ossia::LocalTree::DocumentPlugin& doc,
                QObject* paren_objt) const = 0;
};



template<typename AreaComponent_T>
class AreaComponentFactory_T :
        public AreaComponentFactory
{
    public:
        using model_type = typename AreaComponent_T::model_type;
        using component_type = AreaComponent_T;

        static auto static_concreteFactoryKey()
        {
            return AreaComponent_T::static_key().impl();
        }

        ConcreteFactoryKey concreteFactoryKey() const final override
        {
            return AreaComponent_T::static_key().impl(); // Note : here there is a conversion between UuidKey<Component> and ConcreteFactoryKey
        }

        AreaComponent_T* make(
                const Id<iscore::Component>& cmp,
                OSSIA::Node& parent,
                AreaModel& proc,
                const Ossia::LocalTree::DocumentPlugin& doc,
                QObject* paren_objt) const override
        {
            return new AreaComponent_T{cmp, parent, static_cast<model_type&>(proc), doc, paren_objt};
        }

        bool matches(
                AreaModel& p,
                const Ossia::LocalTree::DocumentPlugin&) const override;
};

template<typename AreaComponent_T>
bool AreaComponentFactory_T<AreaComponent_T>::matches(
        AreaModel& p,
        const Ossia::LocalTree::DocumentPlugin&) const
{
    return dynamic_cast<model_type*>(&p);
}

template<>
inline bool AreaComponentFactory_T<GenericAreaComponent>::matches(
        AreaModel& p,
        const Ossia::LocalTree::DocumentPlugin&) const
{
    return false; // We want it to be the fall-back default case
}

using GenericAreaComponentFactory = AreaComponentFactory_T<GenericAreaComponent>;

using AreaComponentFactoryList =
    iscore::DefaultedGenericComponentFactoryList<
            AreaModel,
            Ossia::LocalTree::DocumentPlugin,
            Space::LocalTree::AreaComponentFactory,
            Space::LocalTree::GenericAreaComponentFactory>;

}
}
