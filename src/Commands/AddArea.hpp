#pragma once
#include <iscore/command/Command.hpp>
#include <iscore/model/path/Path.hpp>
#include <Device/Address/AddressSettings.hpp>
#include <src/Commands/SpaceCommandFactory.hpp>
namespace Space
{
class AreaFactory;
class AreaModel;
class DimensionModel;
class ProcessModel;
class ComputationModel;
class AddArea : public iscore::Command
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), AddArea, "AddArea")
    public:

          AddArea(const Space::ProcessModel& spacProcess,
            UuidKey<AreaFactory> type,
            const QStringList& area,
                  const QMap<Id<DimensionModel>, QString>& dimMap,
                  const QMap<QString, Device::FullAddressSettings>& addrMap);

        void undo(const iscore::DocumentContext& ctx) const override;
        void redo(const iscore::DocumentContext& ctx) const override;

    protected:
        void serializeImpl(DataStreamInput & s) const override;
        void deserializeImpl(DataStreamOutput & s) override;

    private:
        Path<Space::ProcessModel> m_path;
        Id<AreaModel> m_createdAreaId;

        UuidKey<AreaFactory> m_areaType;
        QStringList m_areaFormula;

        QMap<Id<DimensionModel>, QString> m_spaceMap;
        QMap<QString, Device::FullAddressSettings> m_symbolToAddressMap;
};
}
