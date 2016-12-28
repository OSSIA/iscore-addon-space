#include "SpaceModel.hpp"

template <>
void DataStreamReader::read(
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

template <>
void DataStreamWriter::write(
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


template <>
void JSONObjectReader::read(
        const Space::SpaceModel& proc)
{
    obj["Dimensions"] = toJsonArray(proc.m_dimensions);
    obj["Viewports"] = toJsonArray(proc.m_viewports);

    obj["DefaultViewport"] = toJsonValue(proc.m_defaultViewport);
    obj["Precision"] = proc.precision();
}

template <>
void JSONObjectWriter::write(
        Space::SpaceModel& proc)
{
    clear(proc);

    for(const auto& json_vref : obj["Dimensions"].toArray())
    {
        auto dim = new Space::DimensionModel{
                JSONObject::Deserializer{json_vref.toObject()},
                &proc};
        proc.m_dimensions.insert(dim);
    }

    for(const auto& json_vref : obj["Viewports"].toArray())
    {
        auto dim = new Space::ViewportModel{
                JSONObject::Deserializer{json_vref.toObject()},
                &proc};
        proc.m_viewports.insert(dim);
    }

    proc.m_precision = obj["Precision"].toInt();
    proc.m_defaultViewport = fromJsonValue<OptionalId<Space::ViewportModel>>(obj["DefaultViewport"]);
}
