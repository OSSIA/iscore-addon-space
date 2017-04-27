#pragma once
#include <QObject>
#include <iscore/plugins/qt_interfaces/PluginRequirements_QtInterface.hpp>
#include <iscore/plugins/qt_interfaces/FactoryFamily_QtInterface.hpp>
#include <iscore/plugins/qt_interfaces/FactoryInterface_QtInterface.hpp>
#include <iscore/plugins/qt_interfaces/CommandFactory_QtInterface.hpp>
#include <iscore/plugins/qt_interfaces/GUIApplicationPlugin_QtInterface.hpp>

class iscore_plugin_space final :
        public QObject,
        public iscore::Plugin_QtInterface,
        public iscore::FactoryInterface_QtInterface,
        public iscore::FactoryList_QtInterface,
        public iscore::CommandFactory_QtInterface,
        public iscore::ApplicationPlugin_QtInterface
{
        Q_OBJECT
        Q_PLUGIN_METADATA(IID FactoryInterface_QtInterface_iid)
        Q_INTERFACES(
                iscore::Plugin_QtInterface
                iscore::FactoryInterface_QtInterface
                iscore::FactoryList_QtInterface
                iscore::CommandFactory_QtInterface
                iscore::ApplicationPlugin_QtInterface
                )
        ISCORE_PLUGIN_METADATA(1, "02b77d0c-6c8f-45f5-bb28-b649b5c3c7e0")
    public:
        iscore_plugin_space();
        virtual ~iscore_plugin_space();

    private:

        iscore::GUIApplicationPlugin* make_applicationPlugin(
                const iscore::GUIApplicationContext& app) override;


        std::vector<std::unique_ptr<iscore::InterfaceBase>> factories(
                const iscore::ApplicationContext& ctx,
                const iscore::InterfaceKey& matchingName) const override;

        std::vector<std::unique_ptr<iscore::InterfaceListBase>> factoryFamilies() override;

        std::pair<const CommandGroupKey, CommandGeneratorMap> make_commands() override;
};
