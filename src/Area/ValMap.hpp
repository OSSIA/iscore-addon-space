#pragma once
#include <boost/container/flat_map.hpp>
#include <QMap>
#include <iscore/model/Identifier.hpp>
#include <Device/Address/AddressSettings.hpp>

#include <string>
namespace Space
{
class DimensionModel;
using ValMap = boost::container::flat_map<QString, double>;
using SpaceMap = QMap<Id<DimensionModel>, QString>;

using ParameterMap = QMap<QString, Device::FullAddressSettings>;
}

Q_DECLARE_METATYPE(Space::ValMap)
Q_DECLARE_METATYPE(Space::SpaceMap)
Q_DECLARE_METATYPE(Space::ParameterMap)
