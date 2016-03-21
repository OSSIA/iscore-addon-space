#include "SpaceModel.hpp"
#include <algorithm>


template<>
void Visitor<Reader<DataStream>>::readFrom(
        const Space::SpaceModel& proc)
{
    // Dimensions
    m_stream << (int32_t)proc.m_dimensions.size();
    for(const auto& val : proc.m_dimensions)
    {
        readFrom(val);
    }

    // Viewports
    m_stream << (int32_t)proc.m_viewports.size();
    for(const auto& val : proc.m_viewports)
    {
        readFrom(val);
    }

    m_stream << proc.m_defaultViewport;
    m_stream << proc.m_precision;

    insertDelimiter();
}

template<>
void Visitor<Writer<DataStream>>::writeTo(
        Space::SpaceModel& proc)
{
    // Dimensions
    int32_t d_size;
    m_stream >> d_size;
    for(; d_size --> 0;)
    {
        proc.m_dimensions.insert(new Space::DimensionModel{*this, &proc});
    }

    // Viewports
    int32_t v_size;
    m_stream >> v_size;
    for(; v_size --> 0;)
    {
        proc.m_viewports.insert(new Space::ViewportModel{*this, &proc});
    }

    m_stream >> proc.m_defaultViewport;
    m_stream >> proc.m_precision;

    checkDelimiter();
}




template<>
void Visitor<Reader<JSONObject>>::readFrom(
        const Space::SpaceModel& proc)
{
    m_obj["Dimensions"] = toJsonArray(proc.m_dimensions);
    m_obj["Viewports"] = toJsonArray(proc.m_viewports);

    m_obj["DefaultViewport"] = toJsonValue(proc.m_defaultViewport);
    m_obj["Precision"] = proc.precision();
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(
        Space::SpaceModel& proc)
{
    for(const auto& json_vref : m_obj["Dimensions"].toArray())
    {
        auto dim = new Space::DimensionModel{
                Deserializer<JSONObject>{json_vref.toObject()},
                &proc};
        proc.m_dimensions.insert(dim);
    }

    for(const auto& json_vref : m_obj["Viewports"].toArray())
    {
        auto dim = new Space::DimensionModel{
                Deserializer<JSONObject>{json_vref.toObject()},
                &proc};
        proc.m_dimensions.insert(dim);
    }

    proc.m_precision = m_obj["Precision"].toInt();
    proc.m_defaultViewport = fromJsonValue<Id<Space::ViewportModel>>(m_obj["DefaultViewport"]);
}


namespace Space
{
SpaceModel::SpaceModel(
        const Id<SpaceModel> &id,
        QObject *parent):
    IdentifiedObject{id, staticMetaObject.className(), parent}
{
    emit spaceChanged();
}

Bounds SpaceModel::bounds() const
{
    const auto& x = dimension("x");
    const auto& y = dimension("y");
    return {
        x.min(),
        x.max(),
        y.min(),
        y.max(),
        double(m_precision)
    };
}

void SpaceModel::addDimension(DimensionModel *dim)
{
    m_dimensions.insert(dim);
    emit spaceChanged();
}

const DimensionModel& SpaceModel::dimension(const QString& name) const
{
    auto it = std::find_if(m_dimensions.begin(),
                           m_dimensions.end(),
                           [&] (const DimensionModel& dim) { return dim.name() == name; });

    ISCORE_ASSERT(it != m_dimensions.end());

    return *it;
}

const DimensionModel& SpaceModel::dimension(const Id<DimensionModel> &id) const
{
    return m_dimensions.at(id);
}

void SpaceModel::removeDimension(const QString &name)
{
    ISCORE_TODO;
    emit spaceChanged();
}


void SpaceModel::addViewport(ViewportModel* v)
{
    m_viewports.insert(v);
    if(!m_defaultViewport)
        m_defaultViewport = v->id();
    emit viewportAdded(*v);
}

void SpaceModel::removeViewport(const Id<ViewportModel>& vm)
{
    ISCORE_TODO;

    if(m_defaultViewport == vm)
    {
        if(!m_viewports.empty())
        {
            m_defaultViewport = (*m_viewports.begin()).id();
        }
        else
        {
            m_defaultViewport = Id<ViewportModel>{};
        }
    }
}
}
