#include "ComputationsWidget.hpp"
#include <iscore/document/DocumentContext.hpp>
#include <QComboBox>
#include <QPushButton>
#include <Explorer/Widgets/AddressEditWidget.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <src/Space/SpaceModel.hpp>
#include <src/SpaceContext.hpp>
#include <src/SpaceProcess.hpp>
#include <src/Computation/ComputationFactoryList.hpp>
#include <src/Commands/AddComputation.hpp>
#include <src/Area/AreaModel.hpp>
namespace Space
{

ComputationsWidget::ComputationsWidget(
        const iscore::DocumentContext& ctx,
        const ProcessModel& space,
        QWidget* parent):
    QWidget{parent},
    m_dispatcher{ctx.commandStack},
    m_space{space}
{
    auto lay = new QFormLayout;
    this->setLayout(lay);

    // UI :
    // select area1
    m_a1 = new QComboBox;

    // select area2
    m_a2 = new QComboBox;

    // select computation type
    m_type = new QComboBox;

    // select target address
    auto& dev_expl = m_space.context().devices.explorer();
    m_address = new Explorer::AddressEditWidget{dev_expl, this};

    lay->addRow(tr("First area"), m_a1);
    lay->addRow(tr("Second area"), m_a2);
    lay->addRow(tr("Type"), m_type);
    lay->addRow(tr("Result"), m_address);

    auto& comp_list = m_space.context().doc.app.interfaces<ComputationFactoryList>();
    for(auto& comp : comp_list)
    {
        m_type->addItem(comp.prettyName(), QVariant::fromValue(comp.concreteKey()));
    }


    QPushButton* val = new QPushButton{tr("Create / update")};
    lay->addWidget(val);

    connect(val, &QPushButton::pressed, this, &ComputationsWidget::validate);
    space.areas.added.connect<ComputationsWidget, &ComputationsWidget::areaAdded>(this);
    space.areas.removed.connect<ComputationsWidget, &ComputationsWidget::areaRemoved>(this);

    cleanup();
}

void ComputationsWidget::setActiveComputation(const ComputationModel* comp)
{
    cleanup();

    m_computation = comp;
    if(m_computation)
    {
        loadComputation(*m_computation);
    }
}

void ComputationsWidget::validate()
{
    if((m_a1->currentIndex() < 0) ||
       (m_a2->currentIndex() < 0) ||
       (m_type->currentIndex() < 0))
        return;

    auto a1 = m_a1->currentData().value<Id<AreaModel>>();
    auto a2 = m_a2->currentData().value<Id<AreaModel>>();
    auto t = m_type->currentData().value<UuidKey<ComputationFactory>>();
    auto addr = m_address->address();

    if(!m_computation)
    {
        // Create new
        auto cmd = new AddComputation{m_space, t, a1, a2, addr};
        m_dispatcher.submitCommand(cmd);
    }
    else
    {
        // Update
        ISCORE_TODO;
    }
}

void ComputationsWidget::cleanup()
{
    m_a1->clear();
    m_a2->clear();
    m_type->setCurrentIndex(0);
    m_address->setAddress({});

    for(AreaModel& area : m_space.areas)
    {
        m_a1->addItem(QString::number(area.id_val()), QVariant::fromValue(area.id()));
        m_a2->addItem(QString::number(area.id_val()), QVariant::fromValue(area.id()));
    }
}

// QComboBox::findData doesn't work :(
static int find_item(const QComboBox& cb, const Id<AreaModel>& id)
{
    for(int i = 0; i < cb.count(); i++)
    {
        auto res = cb.itemData(i).value<Id<AreaModel>>();
        if(res == id) {
            return i;
        }
    }
    return -1;
}

void ComputationsWidget::loadComputation(
        const ComputationModel& comp)
{
    auto a1_idx = find_item(*m_a1, comp.firstArea());
    auto a2_idx = find_item(*m_a2, comp.secondArea());
    ISCORE_ASSERT(a1_idx != -1);
    ISCORE_ASSERT(a2_idx != -1);

    m_a1->setCurrentIndex(a1_idx);
    m_a2->setCurrentIndex(a2_idx);

    auto t_idx = m_type->findData(QVariant::fromValue(comp.concreteKey()));
    ISCORE_ASSERT(t_idx != -1);
    m_type->setCurrentIndex(t_idx);

    m_address->setAddress(comp.address());
}

void ComputationsWidget::areaAdded(const AreaModel&)
{
    cleanup();
}

void ComputationsWidget::areaRemoved(const AreaModel&)
{
    cleanup();
}

}
