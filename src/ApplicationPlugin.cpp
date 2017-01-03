#include "ApplicationPlugin.hpp"
#include <QToolBar>
#include <iscore/widgets/OrderedToolbar.hpp>
namespace Space
{

ApplicationPlugin::ApplicationPlugin(const iscore::GUIApplicationContext &app):
    iscore::GUIApplicationPlugin{app}
{
    m_actions = new QActionGroup{nullptr};
    m_actions->setExclusive(true);
    m_move = m_actions->addAction(tr("Move"));
    m_scale = m_actions->addAction(tr("Scale"));
    m_rotate = m_actions->addAction(tr("Rotate"));

    m_move->setCheckable(true);
    m_scale->setCheckable(true);
    m_rotate->setCheckable(true);
    connect(m_move, &QAction::triggered,
            this, [=] (bool b) {
        if(b)
        {
            m_settings.setTool(Tool::Move);
        }
    });
    connect(m_scale, &QAction::triggered,
            this, [=] (bool b) {
        if(b)
        {
            m_settings.setTool(Tool::Scale);
        }
    });
    connect(m_rotate, &QAction::triggered,
            this, [=] (bool b) {
        if(b)
        {
            m_settings.setTool(Tool::Rotate);
        }
    });
}

iscore::GUIElements ApplicationPlugin::makeGUIElements()
{
    GUIElements e;

    QToolBar* bar = new QToolBar;
    bar->addActions({m_move, m_scale, m_rotate});
    e.toolbars.emplace_back(bar, StringKey<iscore::Toolbar>("Space"), 0, 5);

    return e;
}

}
