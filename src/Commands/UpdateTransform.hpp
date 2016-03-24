#pragma once
#include <iscore/command/SerializableCommand.hpp>
#include <src/Commands/SpaceCommandFactory.hpp>
#include <iscore/tools/ModelPath.hpp>

namespace Space
{
class AreaModel;
struct UpdateTransform : public iscore::SerializableCommand
{
         ISCORE_COMMAND_DECL(CommandFactoryName(), UpdateTransform, "Update Area")
    public:
        UpdateTransform(
                const AreaModel& model,
                QPointF translate,
                QSizeF scale,
                qreal rotate);

        void undo() const override;
        void redo() const override;

        void update(const Path<AreaModel>&,
                    QPointF translate,
                    QSizeF scale,
                    qreal rotate);

    protected:
        void serializeImpl(DataStreamInput & s) const override;
        void deserializeImpl(DataStreamOutput & s) override;

    private:
        Path<AreaModel> m_model;
        QPointF m_oldTranslate, m_newTranslate;
        QSizeF m_oldScale, m_newScale;
        qreal m_oldRot, m_newRot;

};
}
