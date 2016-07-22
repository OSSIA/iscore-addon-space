#pragma once
#include <Editor/TimeProcess.h>
#include <Editor/State.h>

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
        public OSSIA::TimeProcess
{
    public:
        ProcessExecutor(
                Space::ProcessModel& process,
                const Device::DeviceList& devices);

        ~ProcessExecutor();

        std::shared_ptr<OSSIA::StateElement> state(double);
        std::shared_ptr<OSSIA::StateElement> state() override;
        std::shared_ptr<OSSIA::StateElement> offset(OSSIA::TimeValue) override;

    private:
        Space::ProcessModel& m_process;
        const Device::DeviceList& m_devices;

        std::shared_ptr<OSSIA::State> m_start;
        std::shared_ptr<OSSIA::State> m_end;
};

class Component final :
        public ::RecreateOnPlay::ProcessComponent_T<Space::ProcessModel>
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
