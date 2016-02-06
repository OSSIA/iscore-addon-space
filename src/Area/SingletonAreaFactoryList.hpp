#pragma once
#include <iscore/plugins/customfactory/FactoryFamily.hpp>
#include <src/Area/AreaFactoryList.hpp>
#include <iscore_plugin_space_export.h>

namespace Space
{
class ISCORE_PLUGIN_SPACE_EXPORT SingletonAreaFactoryList final :
        public iscore::ConcreteFactoryList<AreaFactory>
{

};

}
