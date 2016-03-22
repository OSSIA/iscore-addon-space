#include "ComputationsTab.hpp"
#include "src/SpaceProcess.hpp"
#include <iscore/widgets/MarginLess.hpp>
#include "ComputationsWidget.hpp"
namespace Space
{
ComputationsTab::ComputationsTab(
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
    list_widg->layout()->addWidget(new QLabel{tr("Computations")});
    m_newComp = new QPushButton{tr("New Computation")};
    list_widg->layout()->addWidget(m_newComp);
    m_compList = new QListWidget;
    list_widg->layout()->addWidget(m_compList);
    lay->addWidget(list_widg, 0, 0);
    m_compWidget = new ComputationsWidget{ctx, space, this};
    lay->addWidget(m_compWidget, 0, 1);

    lay->setColumnMinimumWidth(0, 200);
    lay->setColumnStretch(1, 3);

    m_space.computations.added.connect<ComputationsTab, &ComputationsTab::on_compAdded>(this);
    m_space.computations.removed.connect<ComputationsTab, &ComputationsTab::on_compRemoved>(this);

    connect(m_compList, &QListWidget::currentRowChanged,
            this, [=] (int i ) {
        if(i == -1)
            m_compWidget->setActiveComputation(nullptr);
        else
            m_compWidget->setActiveComputation(&m_space.computations.at(m_compList->item(i)->data(Qt::UserRole).value<Id<ComputationModel>>()));
    });
    connect(m_newComp, &QPushButton::clicked,
            this, [=] () {
        m_compList->clearSelection();
        m_compWidget->setActiveComputation(nullptr);
    });
    rebuildList();
}

void ComputationsTab::rebuildList()
{
    m_compList->clear();

    for(const auto& comp : m_space.computations)
    {
        auto itm = new QListWidgetItem(QString::number(comp.id_val()), m_compList);
        itm->setData(Qt::UserRole, QVariant::fromValue(comp.id()));
    }

    m_compWidget->setActiveComputation(nullptr);
}

void ComputationsTab::on_compAdded(const ComputationModel&)
{
    //OPTIMIZEME
    rebuildList();
}

void ComputationsTab::on_compRemoved(const ComputationModel&)
{
    //OPTIMIZEME
    rebuildList();
}
}
