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
        auto node = thisNode().createChild(param.first.toStdString());
        auto addr = node->createAddress(t);
        addr->setAccessMode(ossia::access_mode::BI);

        auto callback_it = addr->add_callback([=] (const ossia::value& v)
        {
          auto val = State::convert::value<double>(Engine::ossia_to_iscore::ToValue(v));
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
        auto ossia_val = addr.cloneValue();
        if(newVal != Engine::ossia_to_iscore::ToValue(ossia_val))
        {
            addr.pushValue(Engine::iscore_to_ossia::toOSSIAValue(newVal));
        }
    },
    Qt::QueuedConnection);
}

}
}
