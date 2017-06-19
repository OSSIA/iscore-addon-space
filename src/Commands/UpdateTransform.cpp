#include "UpdateTransform.hpp"
#include <src/Area/AreaModel.hpp>
#include <iscore/model/path/PathSerialization.hpp>
namespace Space
{

UpdateTransform::UpdateTransform(
        const AreaModel &model,
        QPointF translate,
        QSizeF scale,
        qreal rotate):
    m_model{model},
    m_oldTranslate{model.translate()},
    m_newTranslate{translate},
    m_oldScale{model.scale()},
    m_newScale{scale},
    m_oldRot{model.rotate()},
    m_newRot{rotate}
{
}

void UpdateTransform::undo(const iscore::DocumentContext& ctx) const
{
    auto& area = m_model.find(ctx);
    area.setTranslate(m_oldTranslate);
    area.setScale(m_oldScale);
    area.setRotate(m_oldRot);
}

void UpdateTransform::redo(const iscore::DocumentContext& ctx) const
{
    auto& area = m_model.find(ctx);
    area.setTranslate(m_newTranslate);
    area.setScale(m_newScale);
    area.setRotate(m_newRot);
}

void UpdateTransform::update(
        const Path<AreaModel>&,
        QPointF translate,
        QSizeF scale,
        qreal rotate)
{
    m_newTranslate = translate;
    m_newScale = scale;
    m_newRot = rotate;
}

void UpdateTransform::serializeImpl(DataStreamInput &s) const
{
    s << m_model
      << m_oldTranslate << m_newTranslate
      << m_oldScale << m_newScale
      << m_oldRot << m_newRot
         ;
}

void UpdateTransform::deserializeImpl(DataStreamOutput &s)
{
    s >> m_model
      >> m_oldTranslate >> m_newTranslate
      >> m_oldScale >> m_newScale
      >> m_oldRot >> m_newRot
         ;
}

}
