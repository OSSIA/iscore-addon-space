#include "DimensionModel.hpp"
#include <iscore/serialization/JSONValueVisitor.hpp>

template <>
void DataStreamReader::read(
        const Space::DimensionModel& dim)
{
    m_stream << dim.name() << dim.value() << dim.min() << dim.max();
    insertDelimiter();
}

template <>
void DataStreamWriter::writeTo(
        Space::DimensionModel& dim)
{
    m_stream >> dim.m_name >> dim.m_val >> dim.m_min >> dim.m_max;

    checkDelimiter();
}

template <>
void JSONObjectReader::read(
        const Space::DimensionModel& dim)
{
    obj[strings.Name] = dim.name();
    obj["Value"] = toJsonValue(dim.value());
    obj["Min"] = dim.min();
    obj["Max"] = dim.max();
}

template <>
void JSONObjectWriter::writeTo(
        Space::DimensionModel& dim)
{
    dim.m_name = obj[strings.Name].toString();
    dim.m_val = fromJsonValue<optional<double>>(obj["Value"]);
    dim.m_min = obj["Min"].toDouble();
    dim.m_max = obj["Max"].toDouble();
}
