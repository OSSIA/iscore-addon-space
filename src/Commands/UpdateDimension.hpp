#pragma once
#include <iscore/command/Command.hpp>
#include <src/Commands/SpaceCommandFactory.hpp>
#include <iscore/model/path/Path.hpp>

namespace Space
{
class DimensionModel;
struct UpdateDimension : public iscore::Command
{
            ISCORE_COMMAND_DECL(CommandFactoryName(), UpdateDimension, "Update Dimension")
    public:
        UpdateDimension(
                const DimensionModel& model,
                    double newMin,
                    double newMax);

        void undo(const iscore::DocumentContext& ctx) const override;
        void redo(const iscore::DocumentContext& ctx) const override;

    protected:
        void serializeImpl(DataStreamInput & s) const override;
        void deserializeImpl(DataStreamOutput & s) override;

    private:
        Path<DimensionModel> m_model;
        double m_oldMin, m_newMin;
        double m_oldMax, m_newMax;

};
}
