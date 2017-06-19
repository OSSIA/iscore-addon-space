#pragma once
#include <iscore/command/Command.hpp>
#include <iscore/model/path/Path.hpp>
#include <Device/Address/AddressSettings.hpp>
#include <src/Commands/SpaceCommandFactory.hpp>
namespace Space
{
class ComputationFactory;
class ComputationModel;
class AreaModel;
class ProcessModel;
class AddComputation : public iscore::Command
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), AddComputation, "AddComputation")
    public:

          AddComputation(
            const Space::ProcessModel& spacProcess,
            UuidKey<ComputationFactory> type,
            const Id<AreaModel>& a1,
            const Id<AreaModel>& a2,
            const State::Address& addr);

        void undo(const iscore::DocumentContext& ctx) const override;
        void redo(const iscore::DocumentContext& ctx) const override;

    protected:
        void serializeImpl(DataStreamInput & s) const override;
        void deserializeImpl(DataStreamOutput & s) override;

    private:
        Path<Space::ProcessModel> m_path;
        Id<ComputationModel> m_createdCompId;

        UuidKey<ComputationFactory> m_type;
        Id<AreaModel> m_area1;
        Id<AreaModel> m_area2;

        State::Address m_addr;
};
}
