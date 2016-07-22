#include "ProcessExecutor.hpp"
#include <src/SpaceProcess.hpp>

#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <src/LocalTree/AreaComponent.hpp>
#include <src/LocalTree/ComputationComponent.hpp>
#include <OSSIA/Executor/DocumentPlugin.hpp>
#include <Editor/TimeConstraint.h>
#include <Editor/Message.h>
#include <OSSIA/iscore2OSSIA.hpp>
namespace Space
{
namespace Executor
{
ProcessExecutor::ProcessExecutor(
        ProcessModel& process,
        const Device::DeviceList& devices):
    m_process{process},
    m_devices{devices}
{
    for(AreaModel& area : m_process.areas)
    {
        const auto& parameter_map = area.parameterMapping();
        for(const auto& elt : parameter_map.keys())
        {
            auto& val = parameter_map[elt];

            auto& addr = val.address;
            if(!addr.device.isEmpty())
            {
                if(addr.device == "parent" && addr.path == QStringList{"t"})
                {
                    continue;
                }
                else
                {
                    // We enable listening
                    auto dev_it = m_devices.find(addr.device);
                    if(dev_it != m_devices.devices().end())
                    {
                        (*dev_it)->setListening(addr, true);
                    }
                }
            }
        }
    }

}

ProcessExecutor::~ProcessExecutor()
{

    for(AreaModel& area : m_process.areas)
    {
        const auto& parameter_map = area.parameterMapping();
        for(const auto& elt : parameter_map.keys())
        {
            auto& val = parameter_map[elt];

            auto& addr = val.address;
            if(!addr.device.isEmpty())
            {
                if(addr.device == "parent" && addr.path == QStringList{"t"})
                {
                    continue;
                }
                else
                {
                    // We disable listening
                    auto dev_it = m_devices.find(addr.device);
                    if(dev_it != m_devices.devices().end())
                    {
                        (*dev_it)->setListening(addr, false);
                    }
                }
            }
        }
    }
}

std::shared_ptr<OSSIA::StateElement> ProcessExecutor::state()
{
    return state(parent->getPosition());
}

std::shared_ptr<OSSIA::StateElement> ProcessExecutor::state(double t)
{
    using namespace GiNaC;

    // For each area whose parameters depend on an address,
    // get the current area value and update it.
    for(AreaModel& area : m_process.areas)
    {
        // Handle time
        const auto& parameter_map = area.parameterMapping();
        ValMap mapping;

        for(const auto& elt : parameter_map.keys())
        {
            auto& val = parameter_map[elt];
            // We always set the default value just in case.
            auto it_pair = mapping.insert(
                               std::make_pair(
                                   elt,
                                   State::convert::value<double>(val.value)
                                   )
                               );

            auto& addr = val.address;
            if(!addr.device.isEmpty())
            {
                if(addr.device == "parent" && addr.path == QStringList{"t"})
                {
                    it_pair.first->second = t;
                }
                else
                {
                    // We fetch it from the device tree
                    auto dev_it = m_devices.find(addr.device);
                    if(dev_it != m_devices.devices().end())
                    {
                        auto val = (*dev_it)->refresh(addr);
                        if(val)
                        {
                           it_pair.first->second = State::convert::value<double>(*val);
                           qDebug() << State::convert::value<double>(*val);
                        }
                    }
                }
            }
        }

        area.setCurrentMapping(mapping);
    }


    auto state = OSSIA::State::create();
    // State of each area
    // Shall be done in the "tree" component.
    /*
    for(const AreaModel& area : m_process.areas())
    {
        makeState(*state, area);
    }
    */



    // Shall be done either here, or in the tree component : choose between reactive, and state mode.
    // Same problem for "mapping" plug-in : react to changes or return state ?
    // "Reactive" execution component (has to be enabled / disabled on start / end)
    // vs "state" execution component
    // Handle computations / collisions
    for(const ComputationModel& computation : m_process.computations)
    {
        State::Message mess{computation.address(), computation.result()};
        auto ossia_mess = iscore::convert::message(mess, m_devices);
        if(ossia_mess)
            state->stateElements().push_back(ossia_mess);
        /*
        // We look for its tree component
        auto compo_it = find_if(
                            computation.components,
                            [] (iscore::Component& comp)
        { return dynamic_cast<LocalTree::ComputationComponent*>(&comp); });

        if(compo_it != computation.components.end())
        {
            auto& compo = static_cast<LocalTree::ComputationComponent&>(*compo_it);
            ISCORE_ASSERT(compo.valueNode()->getAddress().get());
            auto res = computation.computation()();
            auto mess = OSSIA::Message::create(
                            compo.valueNode()->getAddress(),
                            new OSSIA::Float(res));
            qDebug() << "Writing to " << compo.valueNode()->getName().c_str() << " => " << res;
            state->stateElements().push_back(std::move(mess));
        }
        */

    }

    // Send the parameters of each area
    // (variables's value ? default computations (like diameter, etc. ?))
    // For each computation, send the new state.

    return state;
}

std::shared_ptr<OSSIA::StateElement> ProcessExecutor::offset(OSSIA::TimeValue off)
{
    return state(off / parent->getDurationNominal());
}





Component::Component(
        ::RecreateOnPlay::ConstraintElement& parentConstraint,
        Space::ProcessModel& element,
        const ::RecreateOnPlay::Context& ctx,
        const Id<iscore::Component>& id,
        QObject* parent):
    ::RecreateOnPlay::ProcessComponent_T<Space::ProcessModel>{parentConstraint, element, ctx, id, "SpaceComponent", parent}
{
    m_ossia_process = std::make_shared<ProcessExecutor>(element, ctx.devices.list());
}

}
}
