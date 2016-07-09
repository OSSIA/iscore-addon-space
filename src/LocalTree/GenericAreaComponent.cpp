#include "GenericAreaComponent.hpp"

namespace Space
{
namespace LocalTree
{

GenericAreaComponent::GenericAreaComponent(
        const Id<iscore::Component>& cmp,
        OSSIA::Node& parent_node,
        AreaModel& area,
        const Ossia::LocalTree::DocumentPlugin& doc,
        QObject* paren_objt):
    AreaComponent{parent_node, area, cmp, "GenericAreaComponent", paren_objt}
{
    Ossia::LocalTree::make_metadata_node(area.metadata, *node(), m_properties, this);

    using namespace GiNaC;
    for(const auto& param : area.currentMapping())
    {
        constexpr auto t = Ossia::convert::MatchingType<double>::val;
        auto node_it = thisNode().emplace(
                           thisNode().children().end(),
                           param.first.toStdString(),
                           t,
                           OSSIA::AccessMode::BI);
        auto& node = *node_it;
        auto addr = node->getAddress();

        auto callback_it = addr->addCallback([=] (const OSSIA::Value& v)
        {
          auto val = State::convert::value<double>(Ossia::convert::ToValue(v));
          m_area.updateCurrentMapping(param.first, val);
        });

        auto wrap = std::make_unique<Ossia::LocalTree::BaseCallbackWrapper>(node, addr);
        wrap->callbackIt = callback_it;
        m_ginacProperties.insert(std::make_pair(param.first, std::move(wrap)));

        auto val = iscore::convert::toOSSIAValue(
                       State::Value::fromValue(ex_to<numeric>(param.second).to_double()));
        addr->setValue(*val);
    }

    // IF Not listening :

    QObject::connect(&m_area, &AreaModel::currentSymbolChanged,
                     this, [=] (QString sym, double val) {
        auto newVal = State::Value::fromValue(val);
        auto& addr = m_ginacProperties.at(sym)->addr;
        auto ossia_val = addr->cloneValue();
        if(newVal != Ossia::convert::ToValue(ossia_val))
        {
            auto new_ossia_val = iscore::convert::toOSSIAValue(newVal);
            addr->pushValue(*new_ossia_val);
        }
    },
    Qt::QueuedConnection);
}

}
}
