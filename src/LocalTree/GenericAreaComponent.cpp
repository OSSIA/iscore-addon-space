#include "GenericAreaComponent.hpp"

namespace Space
{
namespace LocalTree
{

GenericAreaComponent::GenericAreaComponent(
        const Id<iscore::Component>& cmp,
        ossia::net::node_base& parent_node,
        AreaModel& area,
        const Engine::LocalTree::DocumentPlugin& doc,
        QObject* paren_objt):
    AreaComponent{parent_node, area, cmp, "GenericAreaComponent", paren_objt}
{
    Engine::LocalTree::make_metadata_node(area.metadata(), node(), m_properties, this);

    using namespace GiNaC;
    for(const auto& param : area.currentMapping())
    {
        constexpr auto t = Engine::ossia_to_iscore::MatchingType<double>::val;
        auto node = thisNode().create_child(param.first.toStdString());
        auto addr = node->create_address(t);
        addr->set_access(ossia::access_mode::BI);

        auto callback_it = addr->add_callback([=] (const ossia::value& v)
        {
          auto val = State::convert::value<double>(State::fromOSSIAValue(v));
          m_area.updateCurrentMapping(param.first, val);
        });

        auto wrap = std::make_unique<Engine::LocalTree::BaseCallbackWrapper>(*node, *addr);
        wrap->callbackIt = callback_it;
        m_ginacProperties.insert(std::make_pair(param.first, std::move(wrap)));

        addr->setValue(
                    Engine::iscore_to_ossia::toOSSIAValue(
                        State::Value::fromValue(
                            ex_to<numeric>(param.second).to_double())));
    }

    // IF Not listening :

    QObject::connect(&m_area, &AreaModel::currentSymbolChanged,
                     this, [=] (QString sym, double val) {
        auto newVal = State::Value::fromValue(val);
        auto& addr = m_ginacProperties.at(sym)->addr;
        auto ossia_val = addr.value();
        if(newVal != State::fromOSSIAValue(ossia_val))
        {
            addr.push_value(Engine::iscore_to_ossia::toOSSIAValue(newVal));
        }
    },
    Qt::QueuedConnection);
}

}
}
