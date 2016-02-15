#pragma once
#include <OSSIA/ProcessModel/TimeProcessWithConstraint.hpp>

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
        public TimeProcessWithConstraint
{
    public:
        ProcessExecutor(
                Space::ProcessModel& process,
                const Device::DeviceList& devices);


        std::shared_ptr<OSSIA::StateElement> state(
                const OSSIA::TimeValue&,
                const OSSIA::TimeValue&) override;

    private:
        Space::ProcessModel& m_process;
        const Device::DeviceList& m_devices;

        std::shared_ptr<OSSIA::State> m_start;
        std::shared_ptr<OSSIA::State> m_end;
};

class ProcessComponent final : public RecreateOnPlay::ProcessComponent
{
    public:
        ProcessComponent(
                RecreateOnPlay::ConstraintElement& parentConstraint,
                ProcessModel& element,
                const RecreateOnPlay::Context& ctx,
                const Id<iscore::Component>& id,
                QObject* parent);

    private:
        const Key &key() const override;
};


class ProcessComponentFactory final :
        public RecreateOnPlay::ProcessComponentFactory
{
    public:
        virtual ~ProcessComponentFactory();

        virtual RecreateOnPlay::ProcessComponent* make(
                RecreateOnPlay::ConstraintElement& cst,
                Process::ProcessModel& proc,
                const RecreateOnPlay::Context& ctx,
                const Id<iscore::Component>& id,
                QObject* parent) const override;

        const ConcreteFactoryKey& concreteFactoryKey() const override;

        bool matches(
                Process::ProcessModel& proc,
                const RecreateOnPlay::DocumentPlugin&,
                const iscore::DocumentContext &) const override;
};

}
}
