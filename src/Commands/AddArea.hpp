#pragma once
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ModelPath.hpp>
#include <Device/Address/AddressSettings.hpp>
#include <src/Commands/SpaceCommandFactory.hpp>
#include <src/Area/AreaFactoryKey.hpp>

namespace Space
{
class AreaModel;
class DimensionModel;
class ProcessModel;
class ComputationModel;
class AddArea : public iscore::SerializableCommand
{
        ISCORE_COMMAND_DECL(CommandFactoryName(), AddArea, "AddArea")
    public:

          AddArea(Path<Space::ProcessModel>&& spacProcess,
            AreaFactoryKey type,
            const QStringList& area,
                  const QMap<Id<DimensionModel>, QString>& dimMap,
                  const QMap<QString, Device::FullAddressSettings>& addrMap);

        void undo() const override;
        void redo() const override;

    protected:
        void serializeImpl(DataStreamInput & s) const override;
        void deserializeImpl(DataStreamOutput & s) override;

    private:
        Path<Space::ProcessModel> m_path;
        Id<AreaModel> m_createdAreaId;

        AreaFactoryKey m_areaType{"Generic"};
        QStringList m_areaFormula;

        QMap<Id<DimensionModel>, QString> m_spaceMap;
        QMap<QString, Device::FullAddressSettings> m_symbolToAddressMap;

        std::vector<Id<ComputationModel>> m_createdComputations;
};
}
