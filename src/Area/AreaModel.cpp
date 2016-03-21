#include "AreaModel.hpp"
#include "AreaPresenter.hpp"
#include <sstream>

#include <Device/Node/DeviceNode.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>


template<>
struct TSerializer<
        DataStream,
        void,
        Space::ValMap>
{
        static void readFrom(
                DataStream::Serializer& s,
                const Space::ValMap& vec)
        {
            ISCORE_TODO;
            /*
            s.stream() << (int32_t)vec.size();
            for(const auto& elt : vec)
                s.stream() << elt;

            s.insertDelimiter();
            */
        }

        static void writeTo(
                DataStream::Deserializer& s,
                Space::ValMap& vec)
        {
            ISCORE_TODO;
            /*
            int32_t n = 0;
            s.stream() >> n;

            vec.clear();
            vec.resize(n);
            for(int i = 0; i < n; i++)
            {
                s.stream() >> vec[i];
            }

            s.checkDelimiter();
            */
        }
};

template<>
struct TSerializer<
        JSONValue,
        Space::SpaceMap>
{
        static void readFrom(
                JSONValue::Serializer& s,
                const Space::SpaceMap& vec)
        {
            ISCORE_TODO;
            /*
            s.stream() << (int32_t)vec.size();
            for(const auto& elt : vec)
                s.stream() << elt;

            s.insertDelimiter();
            */
        }

        static void writeTo(
                JSONValue::Deserializer& s,
                Space::SpaceMap& vec)
        {
            ISCORE_TODO;
            /*
            int32_t n = 0;
            s.stream() >> n;

            vec.clear();
            vec.resize(n);
            for(int i = 0; i < n; i++)
            {
                s.stream() >> vec[i];
            }

            s.checkDelimiter();
            */
        }
};
template<>
struct TSerializer<
        JSONValue,
        Space::ParameterMap>
{
        static void readFrom(
                JSONValue::Serializer& s,
                const Space::ParameterMap& vec)
        {
            ISCORE_TODO;
            /*
            s.stream() << (int32_t)vec.size();
            for(const auto& elt : vec)
                s.stream() << elt;

            s.insertDelimiter();
            */
        }

        static void writeTo(
                JSONValue::Deserializer& s,
                Space::ParameterMap& vec)
        {
            ISCORE_TODO;
            /*
            int32_t n = 0;
            s.stream() >> n;

            vec.clear();
            vec.resize(n);
            for(int i = 0; i < n; i++)
            {
                s.stream() >> vec[i];
            }

            s.checkDelimiter();
            */
        }
};

template<>
struct TSerializer<
        JSONValue,
        Space::ValMap>
{
        static void readFrom(
                JSONValue::Serializer& s,
                const Space::ValMap& vec)
        {
            ISCORE_TODO;
            /*
            s.stream() << (int32_t)vec.size();
            for(const auto& elt : vec)
                s.stream() << elt;

            s.insertDelimiter();
            */
        }

        static void writeTo(
                JSONValue::Deserializer& s,
                Space::ValMap& vec)
        {
            ISCORE_TODO;
            /*
            int32_t n = 0;
            s.stream() >> n;

            vec.clear();
            vec.resize(n);
            for(int i = 0; i < n; i++)
            {
                s.stream() >> vec[i];
            }

            s.checkDelimiter();
            */
        }
};
template<>
void Visitor<Reader<DataStream>>::readFrom(
        const Space::AreaModel& area)
{
    // Save the parent class
    readFrom(static_cast<const IdentifiedObject<Space::AreaModel>&>(area));

    // Save this class
    m_stream << area.m_transform
             << area.m_formula
             << area.m_spaceMap
             << area.m_parameterMap
             << area.m_currentParameterMap;
}

template<>
void Visitor<Writer<DataStream>>::writeTo(
        Space::AreaModel& area)
{
    m_stream >> area.m_transform
             >> area.m_formula
             >> area.m_spaceMap
             >> area.m_parameterMap
             >> area.m_currentParameterMap;
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(
        const Space::AreaModel& area)
{
    // Save the parent class
    readFrom(static_cast<const IdentifiedObject<Space::AreaModel>&>(area));

    // Save this class
    m_obj["Transform"] = toJsonValue(area.m_transform);
    m_obj["Formula"] = QJsonArray::fromStringList(area.m_formula);
    m_obj["SpaceMap"] = toJsonValue(area.m_spaceMap);
    m_obj["ParameterMap"] = toJsonValue(area.m_parameterMap);
    m_obj["CurrentParamMap"] = toJsonValue(area.m_currentParameterMap);
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(
        Space::AreaModel& area)
{
    area.m_transform = fromJsonValue<QTransform>(m_obj["Transform"]);
    fromJsonArray(m_obj["Formula"].toArray(), area.m_formula);
    area.m_spaceMap = fromJsonValue<Space::SpaceMap>(m_obj["SpaceMap"]);
    area.m_parameterMap = fromJsonValue<Space::ParameterMap>(m_obj["ParameterMap"]);
    area.m_currentParameterMap = fromJsonValue<Space::ValMap>(m_obj["CurrentParamMap"]);
}

namespace Space
{
AreaModel::AreaModel(
        const QStringList& form,
        const Space::Context& space,
        const Id<AreaModel> & id,
        QObject *parent):
    IdentifiedObject{id, staticMetaObject.className(), parent},
    m_context{space},
    m_formula{form}
{
    metadata.setName(QString("Area.%1").arg(*this->id().val()));
}


void AreaModel::setSpaceMapping(const SpaceMap& mapping)
{
    m_spaceMap = mapping;
    emit areaChanged(m_currentParameterMap);
}

void AreaModel::setParameterMapping(const ParameterMap &parameter_mapping)
{
    using namespace GiNaC;
    m_parameterMap = parameter_mapping;

    ValMap mapping;
    for(const auto& elt : m_parameterMap.keys())
    {
        std::string name = elt.toStdString();
        auto& val = m_parameterMap[elt];
        if(val.address.device.isEmpty()) // We use the value
        {
            mapping.insert(
                        std::make_pair(
                            name,
                            State::convert::value<double>(val.value)));
        }
        else // We fetch it from the device tree
        {
            Device::Node* n = Device::try_getNodeFromAddress(m_context.devices.rootNode(), val.address);
            if(n)
            {
                mapping.insert(
                            std::make_pair(
                                name,
                                State::convert::value<double>(n->get<Device::AddressSettings>().value)));
            }
            else
            {
                mapping.insert(
                            std::make_pair(
                                name,
                                State::convert::value<double>(val.value)));
            }
        }
    }
    setCurrentMapping(mapping);
}

void AreaModel::setCurrentMapping(const ValMap& map)
{
    using namespace GiNaC;

    m_currentParameterMap = map;
    for(auto sym : map)
    {
        emit currentSymbolChanged(sym.first, sym.second);
    }

    emit areaChanged(map);
}

void AreaModel::updateCurrentMapping(
        std::string sym,
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
