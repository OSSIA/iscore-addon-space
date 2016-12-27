#include "ViewportModel.hpp"

template <>
void DataStreamReader::read(
        const Space::ViewportModel& vp)
{
    m_stream
            << vp.name()
            << vp.transform()
            << vp.xDim()
            << vp.yDim()
            << vp.m_defaultValuesMap
            << vp.renderPrecision();

    insertDelimiter();
}

template <>
void DataStreamWriter::writeTo(
        Space::ViewportModel& vp)
{
    m_stream
            >> vp.m_name
            >> vp.m_transform
            >> vp.m_xDim
            >> vp.m_yDim
            >> vp.m_defaultValuesMap
            >> vp.m_renderPrecision;

    checkDelimiter();
}

template <>
void JSONObjectReader::read(
        const Space::ViewportModel& vp)
{
    obj[strings.Name] = vp.name();
    obj["Transform"] = toJsonValue(vp.m_transform);
    obj["xDim"] = toJsonValue(vp.xDim());
    obj["yDim"] = toJsonValue(vp.yDim());
    //obj[""] = toJsonValue(vp.yDim());
    obj["Precision"] = vp.renderPrecision();
}

template <>
void JSONObjectWriter::writeTo(
        Space::ViewportModel& vp)
{
    vp.m_name = obj[strings.Name].toString();
    vp.m_transform = fromJsonValue<QTransform>(obj["Transform"]);
    vp.m_xDim = fromJsonValue<Id<Space::DimensionModel>>(obj["xDim"]);
    vp.m_xDim = fromJsonValue<Id<Space::DimensionModel>>(obj["yDim"]);
    vp.m_renderPrecision = obj["Precision"].toInt();
}
