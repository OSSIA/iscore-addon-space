#pragma once
#include <Process/Process.hpp>
#include <iscore/tools/IdentifiedObjectMap.hpp>
#include "Area/AreaModel.hpp"
#include "Computation/ComputationModel.hpp"
#include <iscore/tools/NotifyingMap.hpp>
#include <src/Space/SpaceModel.hpp>

#include <Process/ProcessMetadata.hpp>
#include <OSSIA/LocalTree/Scenario/ProcessComponent.hpp>
#include <OSSIA/LocalTree/Scenario/MetadataParameters.hpp>
#include <iscore_plugin_space_export.h>

namespace Space
{
class ProcessModel;
}
PROCESS_METADATA(
        ISCORE_PLUGIN_SPACE_EXPORT,
        Space::ProcessModel,
        "ac4d616e-797d-4fdf-aaf0-0cb131aecc3d",
        "Space",
        "Space"
        )
namespace Space
{
class SpaceModel;
class LayerModel;
class ProcessModel;
namespace Executor { class ProcessExecutor; }

class ProcessModel : public Process::ProcessModel
{
        Q_OBJECT
        ISCORE_SERIALIZE_FRIENDS(Space::ProcessModel, DataStream)
        ISCORE_SERIALIZE_FRIENDS(Space::ProcessModel, JSONObject)

        friend
        Space::Context makeContext(const iscore::DocumentContext& doc, Space::ProcessModel& sp);

    public:
        ProcessModel(
                const iscore::DocumentContext& doc,
                const TimeValue &duration,
                const Id<Process::ProcessModel> &id,
                QObject *parent);

        ProcessModel(
                const ProcessModel& other,
                const iscore::DocumentContext& doc,
                const Id<Process::ProcessModel> &id,
                QObject *parent);

        template<typename Impl>
        ProcessModel(
                const iscore::DocumentContext& doc,
                Deserializer<Impl>& vis,
                QObject* parent) :
            Process::ProcessModel{vis, parent},
            m_space{new SpaceModel{
                    Id<SpaceModel>(0),
                    this}},
            m_context{makeContext(doc, *this)}
        {
            vis.writeTo(*this);
        }

        const SpaceModel& space() const
        { return *m_space; }
        const Space::Context& context() const
        { return m_context; }

        NotifyingMap<AreaModel> areas;
        NotifyingMap<ComputationModel> computations;


    private:
        SpaceModel& space()
        { return *m_space; }
        ProcessModel *clone(const Id<Process::ProcessModel> &newId, QObject *newParent) const override;

        UuidKey<Process::ProcessFactory> concreteFactoryKey() const override;
        QString prettyName() const override;

        void serialize_impl(const VisitorVariant &vis) const override;

        SpaceModel* m_space{};
        Space::Context m_context;
};
}
