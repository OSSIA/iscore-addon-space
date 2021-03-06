#include "UpdateDimension.hpp"
#include <src/Space/Dimension/DimensionModel.hpp>
#include <iscore/model/path/PathSerialization.hpp>
namespace Space
{

UpdateDimension::UpdateDimension(
        const DimensionModel& model,
            double newMin,
            double newMax):
    m_model{model},
    m_oldMin{model.min()},
    m_newMin{newMin},
    m_oldMax{model.max()},
    m_newMax{newMax}
{
}

void UpdateDimension::undo(const iscore::DocumentContext& ctx) const
{
    auto& dim = m_model.find(ctx);
    dim.setMin(m_oldMin);
    dim.setMax(m_oldMax);
}

void UpdateDimension::redo(const iscore::DocumentContext& ctx) const
{
    auto& dim = m_model.find(ctx);
    dim.setMin(m_newMin);
    dim.setMax(m_newMax);
}

void UpdateDimension::serializeImpl(DataStreamInput &s) const
{
    s << m_model << m_oldMin << m_oldMax << m_newMin << m_newMax;
}

void UpdateDimension::deserializeImpl(DataStreamOutput &s)
{
    s >> m_model >> m_oldMin >> m_oldMax >> m_newMin >> m_newMax;
}

}
