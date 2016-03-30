#include "ViewportModel.hpp"

template<>
void Visitor<Reader<DataStream>>::readFrom(
        const Space::ViewportModel& vp)
{
    readFrom(static_cast<const IdentifiedObject<Space::ViewportModel>&>(vp));
    m_stream
            << vp.name()
            << vp.transform()
            << vp.xDim()
            << vp.yDim()
            << vp.m_defaultValuesMap
            << vp.renderPrecision();

    insertDelimiter();
}

template<>
void Visitor<Writer<DataStream>>::writeTo(
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

template<>
void Visitor<Reader<JSONObject>>::readFrom(
        const Space::ViewportModel& vp)
{
    readFrom(static_cast<const IdentifiedObject<Space::ViewportModel>&>(vp));
    m_obj["Name"] = vp.name();
    m_obj["Transform"] = toJsonValue(vp.m_transform);
    m_obj["xDim"] = toJsonValue(vp.xDim());
    m_obj["yDim"] = toJsonValue(vp.yDim());
    //m_obj[""] = toJsonValue(vp.yDim());
    m_obj["Precision"] = vp.renderPrecision();
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(
        Space::ViewportModel& vp)
{
    vp.m_name = m_obj["Name"].toString();
    vp.m_transform = fromJsonValue<QTransform>(m_obj["Transform"]);
    vp.m_xDim = fromJsonValue<Id<Space::DimensionModel>>(m_obj["xDim"]);
    vp.m_xDim = fromJsonValue<Id<Space::DimensionModel>>(m_obj["yDim"]);
    vp.m_renderPrecision = m_obj["Precision"].toInt();
}
