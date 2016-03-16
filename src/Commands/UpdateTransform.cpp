#include "UpdateTransform.hpp"
#include <src/Area/AreaModel.hpp>
#include <iscore/tools/ModelPathSerialization.hpp>
namespace Space
{

UpdateTransform::UpdateTransform(
        const AreaModel &model,
        const QTransform &newTransf):
    m_model{model},
    m_old{model.transform()},
    m_new{newTransf}
{
}

void UpdateTransform::undo() const
{
    auto& area = m_model.find();
    area.setTransform(m_old);
}

void UpdateTransform::redo() const
{
    auto& area = m_model.find();
    area.setTransform(m_new);
}

void UpdateTransform::update(
        const Path<AreaModel>&,
        const QTransform &newTransf)
{
    m_new = newTransf;
}

void UpdateTransform::serializeImpl(DataStreamInput &s) const
{
    s << m_model << m_old << m_new;
}

void UpdateTransform::deserializeImpl(DataStreamOutput &s)
{
    s >> m_model >> m_old >> m_new;
}

}
