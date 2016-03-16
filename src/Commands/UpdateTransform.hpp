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
                const QTransform& newTransf);

        void undo() const override;
        void redo() const override;

        void update(const Path<AreaModel>&,
                    const QTransform& newTransf);

    protected:
        void serializeImpl(DataStreamInput & s) const override;
        void deserializeImpl(DataStreamOutput & s) override;

    private:
        Path<AreaModel> m_model;
        QTransform m_old, m_new;

};
}
