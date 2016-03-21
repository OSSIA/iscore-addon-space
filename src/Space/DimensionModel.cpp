#include "DimensionModel.hpp"
#include <iscore/serialization/JSONValueVisitor.hpp>

template<>
void Visitor<Reader<DataStream>>::readFrom(
        const Space::DimensionModel& dim)
{
    m_stream << dim.name() << dim.value() << dim.min() << dim.max();
    insertDelimiter();
}

template<>
void Visitor<Writer<DataStream>>::writeTo(
        Space::DimensionModel& dim)
{
    m_stream >> dim.m_name >> dim.m_val >> dim.m_min >> dim.m_max;

    checkDelimiter();
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(
        const Space::DimensionModel& dim)
{
    m_obj["Name"] = dim.name();
    m_obj["Value"] = toJsonValue(dim.value());
    m_obj["Min"] = dim.min();
    m_obj["Max"] = dim.max();
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(
        Space::DimensionModel& dim)
{
    dim.m_name = m_obj["Name"].toString();
    dim.m_val = fromJsonValue<boost::optional<double>>(m_obj["Value"]);
    dim.m_min = m_obj["Min"].toDouble();
    dim.m_min = m_obj["Max"].toDouble();
}

namespace Space
{
DimensionModel::~DimensionModel()
{

}
}
