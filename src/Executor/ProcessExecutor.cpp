#include "ProcessExecutor.hpp"
#include <src/SpaceProcess.hpp>

#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <src/LocalTree/AreaComponent.hpp>
#include <src/LocalTree/ComputationComponent.hpp>
#include <Engine/Executor/DocumentPlugin.hpp>
#include <ossia/editor/scenario/time_constraint.hpp>
#include <ossia/editor/state/state_element.hpp>
#include <Engine/iscore2OSSIA.hpp>
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

ossia::state_element ProcessExecutor::state()
{
    return state(parent->getPosition());
}

ossia::state_element ProcessExecutor::state(double t)
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


    ossia::state state;
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

        state.add(Engine::iscore_to_ossia::message(mess, m_devices));
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

ossia::state_element ProcessExecutor::offset(ossia::time_value off)
{
    return state(off / parent->getDurationNominal());
}





Component::Component(
        ::Engine::Execution::ConstraintElement& parentConstraint,
        Space::ProcessModel& element,
        const ::Engine::Execution::Context& ctx,
        const Id<iscore::Component>& id,
        QObject* parent):
    ::Engine::Execution::ProcessComponent_T<Space::ProcessModel, Space::Executor::ProcessExecutor>{parentConstraint, element, ctx, id, "SpaceComponent", parent}
{
    m_ossia_process = new ProcessExecutor(element, ctx.devices.list());
}

}
}
