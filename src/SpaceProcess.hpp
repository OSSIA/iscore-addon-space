#pragma once
#include <Process/Process.hpp>
#include <iscore/tools/IdentifiedObjectMap.hpp>
#include "Area/AreaModel.hpp"
#include "Computation/ComputationModel.hpp"
#include <iscore/tools/EntityMap.hpp>
#include <src/Space/SpaceModel.hpp>

#include <Process/ProcessMetadata.hpp>
#include <Engine/LocalTree/Scenario/ProcessComponent.hpp>
#include <Engine/LocalTree/Scenario/MetadataParameters.hpp>
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
class ProcessModel;
namespace Executor { class ProcessExecutor; }

class ProcessModel : public Process::ProcessModel
{
        Q_OBJECT
        ISCORE_SERIALIZE_FRIENDS(Space::ProcessModel, DataStream)
        ISCORE_SERIALIZE_FRIENDS(Space::ProcessModel, JSONObject)
        MODEL_METADATA_IMPL(Space::ProcessModel)


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

        // TODO delete components before !!!
        EntityMap<AreaModel> areas;
        EntityMap<ComputationModel> computations;


    private:
        SpaceModel& space()
        { return *m_space; }

        SpaceModel* m_space{};
        Space::Context m_context;
};
}
