#pragma once
#include <iscore/plugins/customfactory/UuidKey.hpp>

namespace Space
{
class AreaFactory;
template<typename T, typename Parent_T>
class AreaMetadata_T : public Parent_T
{
    protected:
        using Parent_T::Parent_T;

    private:
        int type() const override { return T::type(); }

        decltype(std::declval<Parent_T>().concreteFactoryKey()) concreteFactoryKey() const override
        { return T::uuid(); }

        QString prettyName() const override
        { return T::prettyName(); }
};
}
