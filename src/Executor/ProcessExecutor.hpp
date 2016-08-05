#pragma once
#include <ossia/editor/scenario/time_process.hpp>
#include <ossia/editor/state/state_element.hpp>

#include <OSSIA/Executor/ProcessElement.hpp>
#include <OSSIA/Executor/ExecutorContext.hpp>
#include <iscore/document/DocumentContext.hpp>
#include <iscore/document/DocumentInterface.hpp>


namespace Device
{
class DeviceList;
}
namespace Explorer
{
class DeviceDocumentPlugin;
}
namespace Space
{
class ProcessModel;
namespace Executor
{
class ProcessExecutor final :
        public ossia::time_process
{
    public:
        ProcessExecutor(
                Space::ProcessModel& process,
                const Device::DeviceList& devices);

        ~ProcessExecutor();

        ossia::state_element state(double);
        ossia::state_element state() override;
        ossia::state_element offset(ossia::time_value) override;

    private:
        Space::ProcessModel& m_process;
        const Device::DeviceList& m_devices;

        std::shared_ptr<ossia::state> m_start;
        std::shared_ptr<ossia::state> m_end;
};

class Component final :
        public ::RecreateOnPlay::ProcessComponent_T<Space::ProcessModel, Space::Executor::ProcessExecutor>
{
        COMPONENT_METADATA("49e581e8-00cf-4133-b645-66661f04425b")
    public:
        Component(
                ::RecreateOnPlay::ConstraintElement& parentConstraint,
                ProcessModel& element,
                const ::RecreateOnPlay::Context& ctx,
                const Id<iscore::Component>& id,
                QObject* parent);
};

using ComponentFactory = ::RecreateOnPlay::ProcessComponentFactory_T<Component>;

}
}
