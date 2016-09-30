#include "AreaModel.hpp"
#include "AreaPresenter.hpp"
#include <sstream>

#include <Device/Node/DeviceNode.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>



namespace Space
{
AreaModel::AreaModel(
        const QStringList& form,
        const Space::Context& space,
        const Id<AreaModel> & id,
        QObject *parent):
    Entity{id, staticMetaObject.className(), parent},
    m_context{space},
    m_formula{form}
{
    metadata().setInstanceName(*this);
}

AreaModel::AreaModel(
        const AreaModel& source,
        const Context& space,
        const Id<AreaModel>& id,
        QObject* parent):
    Entity{source, id, staticMetaObject.className(), parent},
    m_translate{source.m_translate},
    m_scale{source.m_scale},
    m_rotate{source.m_rotate},
    m_transform{source.m_transform},
    m_inverted{source.m_inverted},
    m_context{space},
    m_formula{source.m_formula},
    m_spaceMap{source.m_spaceMap},
    m_parameterMap{source.m_parameterMap},
    m_currentParameterMap{source.m_currentParameterMap}
{
    metadata().setInstanceName(*this);
}


void AreaModel::setSpaceMapping(const SpaceMap& mapping)
{
    m_spaceMap = mapping;
    emit areaChanged(m_currentParameterMap);
}

void AreaModel::setParameterMapping(const ParameterMap &parameter_mapping)
{
    m_parameterMap = parameter_mapping;

    ValMap mapping;
    for(const auto& elt : m_parameterMap.keys())
    {
        auto& val = m_parameterMap[elt];
        if(val.address.device.isEmpty()) // We use the value
        {
            mapping.insert(
                        std::make_pair(
                            elt,
                            State::convert::value<double>(val.value)));
        }
        else // We fetch it from the device tree
        {
            Device::Node* n = Device::try_getNodeFromAddress(m_context.devices.rootNode(), val.address);
            if(n)
            {
                mapping.insert(
                            std::make_pair(
                                elt,
                                State::convert::value<double>(n->get<Device::AddressSettings>().value)));
            }
            else
            {
                mapping.insert(
                            std::make_pair(
                                elt,
                                State::convert::value<double>(val.value)));
            }
        }
    }
    setCurrentMapping(mapping);
}

void AreaModel::setCurrentMapping(const ValMap& map)
{
    m_currentParameterMap = map;
    for(auto sym : map)
    {
        emit currentSymbolChanged(sym.first, sym.second);
    }

    emit areaChanged(map);
}

void AreaModel::updateCurrentMapping(
        QString sym,
        double value)
{
    m_currentParameterMap.at(sym) = value;
    emit currentSymbolChanged(sym, value);
    emit areaChanged(m_currentParameterMap);
}

QString AreaModel::toString() const
{
    return m_formula.join(";\n");
}
}
