#include "AreaTab.hpp"
#include "AreaWidget.hpp"
#include "src/SpaceProcess.hpp"
#include <iscore/widgets/MarginLess.hpp>

namespace Space
{
AreaTab::AreaTab(
        const iscore::DocumentContext& ctx,
        const Space::ProcessModel &space,
        QWidget *parent):
    QWidget{parent},
    m_space{space}
{
    auto lay = new iscore::MarginLess<QGridLayout>;
    this->setLayout(lay);

    auto list_widg = new QWidget;
    list_widg->setLayout(new QVBoxLayout);
    list_widg->layout()->addWidget(new QLabel{tr("Areas")});
    list_widg->layout()->addWidget(new QPushButton{tr("New Area")});
    m_listWidget = new QListWidget;
    list_widg->layout()->addWidget(m_listWidget);
    lay->addWidget(list_widg, 0, 0);
    m_areaWidget = new AreaWidget{ctx, space, this};
    lay->addWidget(m_areaWidget, 0, 1);

    lay->setColumnMinimumWidth(0, 200);
    lay->setColumnStretch(1, 3);

    m_space.areas.added.connect<AreaTab, &AreaTab::on_areaAdded>(this);
    m_space.areas.removed.connect<AreaTab, &AreaTab::on_areaRemoved>(this);

    connect(m_listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(updateDisplayedArea(int)));

    rebuildList();
}

void AreaTab::updateDisplayedArea(int i)
{
    m_areaWidget->setActiveArea(&m_space.areas.at(m_listWidget->item(i)->data(Qt::UserRole).value<Id<AreaModel>>()));
}

void AreaTab::newArea()
{
    m_areaWidget->setActiveArea(nullptr);
}

void AreaTab::rebuildList()
{
    m_listWidget->clear();

    for(const auto& area : m_space.areas)
    {
        auto itm = new QListWidgetItem(QString::number(area.id_val()), m_listWidget);
        itm->setData(Qt::UserRole, QVariant::fromValue(area.id()));
    }
}

void AreaTab::on_areaAdded(const AreaModel&)
{
    //OPTIMIZEME
    rebuildList();
}

void AreaTab::on_areaRemoved(const AreaModel&)
{
    //OPTIMIZEME
    rebuildList();
}
}
