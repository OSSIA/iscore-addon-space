#include "ApplicationPlugin.hpp"
#include <QToolBar>
#include <iscore/widgets/OrderedToolbar.hpp>
namespace Space
{

ApplicationPlugin::ApplicationPlugin(const iscore::ApplicationContext &app):
    iscore::GUIApplicationContextPlugin{app, "AudioApplicationPlugin", nullptr}
{
    m_actions = new QActionGroup{nullptr};
    m_actions->setExclusive(true);
    m_move = m_actions->addAction(tr("Move"));
    m_scale = m_actions->addAction(tr("Scale"));
    m_rotate = m_actions->addAction(tr("Rotate"));

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

std::vector<iscore::OrderedToolbar> ApplicationPlugin::makeToolbars()
{
    QToolBar* bar = new QToolBar;
    bar->addActions({m_move, m_scale, m_rotate});
    return std::vector<iscore::OrderedToolbar>{iscore::OrderedToolbar(5, bar)};
}

}
