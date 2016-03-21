#include "ViewportModel.hpp"

template<>
void Visitor<Reader<DataStream>>::readFrom(
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
namespace Space
{
const Id<DimensionModel>& ViewportModel::xDim() const
{
    return m_xDim;
}

void ViewportModel::setXDim(const Id<DimensionModel>& xDim)
{
    m_xDim = xDim;
}

const Id<DimensionModel>& ViewportModel::yDim() const
{
    return m_yDim;
}

void ViewportModel::setYDim(const Id<DimensionModel>& yDim)
{
    m_yDim = yDim;
}
/*
const QMap<Id<DimensionModel>, double>& ViewportModel::defaultValuesMap() const
{
    return m_defaultValuesMap;
}

void ViewportModel::setDefaultValuesMap(const QMap<Id<DimensionModel>, double>& defaultValuesMap)
{
    m_defaultValuesMap = defaultValuesMap;
}
*/

const QString& ViewportModel::name() const
{
    return m_name;
}

void ViewportModel::setName(const QString& name)
{
    m_name = name;
}
}
