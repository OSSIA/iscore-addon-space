#include "SpaceModel.hpp"

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
    clear(proc);

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
    clear(proc);

    for(const auto& json_vref : m_obj["Dimensions"].toArray())
    {
        auto dim = new Space::DimensionModel{
                Deserializer<JSONObject>{json_vref.toObject()},
                &proc};
        proc.m_dimensions.insert(dim);
    }

    for(const auto& json_vref : m_obj["Viewports"].toArray())
    {
        auto dim = new Space::ViewportModel{
                Deserializer<JSONObject>{json_vref.toObject()},
                &proc};
        proc.m_viewports.insert(dim);
    }

    proc.m_precision = m_obj["Precision"].toInt();
    proc.m_defaultViewport = fromJsonValue<Id<Space::ViewportModel>>(m_obj["DefaultViewport"]);
}
