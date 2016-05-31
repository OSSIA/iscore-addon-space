#include "iscore_plugin_space.hpp"

#include "SpaceProcessFactory.hpp"
#include "Area/AreaFactory.hpp"
#include "Area/SingletonAreaFactoryList.hpp"

#include "Area/Circle/CircleAreaFactory.hpp"
#include "Area/Pointer/PointerAreaFactory.hpp"
#include "Area/Generic/GenericAreaFactory.hpp"
#include "Area/Rectangle/Factory.hpp"
#include <src/SpaceProcess.hpp>
#include <src/LocalTree/GenericAreaComponentFactory.hpp>
#include <src/LocalTree/GenericComputationComponentFactory.hpp>
#include <src/LocalTree/ProcessComponentFactory.hpp>
#include <src/Executor/ProcessExecutor.hpp>
#include <iscore/plugins/customfactory/FactoryFamily.hpp>
#include <iscore/plugins/customfactory/FactorySetup.hpp>
#include <OSSIA/Executor/DocumentPlugin.hpp>
#include <src/ApplicationPlugin.hpp>
#include <src/Computation/ComputationFactoryList.hpp>
#include <src/Computation/CollisionComputationFactory.hpp>
#include <src/Computation/DistanceComputationFactory.hpp>

iscore_plugin_space::iscore_plugin_space() :
    QObject {}
{
    qRegisterMetaType<Space::ValMap>();
    qRegisterMetaType<Space::SpaceMap>();
    qRegisterMetaType<Space::ParameterMap>();
    qRegisterMetaType<Id<Space::ComputationModel>>();

    QMetaType::registerComparators<Id<Space::ComputationModel>>();
}

iscore_plugin_space::~iscore_plugin_space()
{

}

iscore::GUIApplicationContextPlugin *iscore_plugin_space::make_applicationPlugin(
        const iscore::GUIApplicationContext &app)
{
    return new Space::ApplicationPlugin{app};
}


std::vector<std::unique_ptr<iscore::FactoryInterfaceBase>> iscore_plugin_space::factories(
        const iscore::ApplicationContext& ctx,
        const iscore::AbstractFactoryKey& key) const
{

    return instantiate_factories<
            iscore::ApplicationContext,
    TL<
        FW<Process::ProcessFactory,
            Space::ProcessFactory>,
        FW<Space::AreaFactory,
            Space::GenericAreaFactory,
            Space::CircleAreaFactory,
            Space::PointerAreaFactory,
            Space::RectangleAreaFactory
            >,
        FW<Space::ComputationFactory,
            Space::CollisionComputationFactory,
            Space::DistanceComputationFactory>,
        FW<Ossia::LocalTree::ProcessComponentFactory,
            Space::LocalTree::ProcessLocalTreeFactory>,
        FW<RecreateOnPlay::ProcessComponentFactory,
            Space::Executor::ProcessComponentFactory>,
        FW<Space::LocalTree::AreaComponentFactory,
            Space::LocalTree::GenericAreaComponentFactory // Shall be last in the vector so must be first here, because of the recursion order of C++ templates in instantiate_factories
            >,
        FW<Space::LocalTree::ComputationComponentFactory,
            Space::LocalTree::GenericComputationComponentFactory>
      >
     >(ctx, key);
}


std::vector<std::unique_ptr<iscore::FactoryListInterface>> iscore_plugin_space::factoryFamilies()
{
    return make_ptr_vector<iscore::FactoryListInterface,
            Space::AreaFactoryList,
            Space::ComputationFactoryList,
            Space::LocalTree::AreaComponentFactoryList,
            Space::LocalTree::ComputationComponentFactoryList
            >();
}


#include <iscore/plugins/customfactory/StringFactoryKeySerialization.hpp>
#include <src/Commands/SpaceCommandFactory.hpp>

#include <iscore_plugin_space_commands_files.hpp>
#include <iscore/command/CommandGeneratorMap.hpp>
#include <iscore/command/SerializableCommand.hpp>

std::pair<const CommandParentFactoryKey, CommandGeneratorMap>
iscore_plugin_space::make_commands()
{
    using namespace Space;
    std::pair<const CommandParentFactoryKey, CommandGeneratorMap> cmds{
        CommandFactoryName(), CommandGeneratorMap{}};

    using Types = TypeList<
#include <iscore_plugin_space_commands.hpp>
      >;
    for_each_type<Types>(iscore::commands::FactoryInserter{cmds.second});

    return cmds;
}


iscore::Version iscore_plugin_space::version() const
{
    return iscore::Version{1};
}

UuidKey<iscore::Plugin> iscore_plugin_space::key() const
{
    return "02b77d0c-6c8f-45f5-bb28-b649b5c3c7e0";
}
