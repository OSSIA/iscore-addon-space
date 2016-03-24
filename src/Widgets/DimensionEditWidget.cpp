#include "DimensionEditWidget.hpp"

#include <iscore/widgets/MarginLess.hpp>
#include <iscore/widgets/SpinBoxes.hpp>

#include <src/Commands/UpdateDimension.hpp>
namespace Space
{
DimensionEditWidget::DimensionEditWidget(
        CommandDispatcher<>& disp,
        const DimensionModel& dim,
        QWidget* parent):
    QWidget{parent},
    m_dispatcher{disp},
    m_dim{dim}
{
    auto lay = new iscore::MarginLess<QHBoxLayout>;

    m_name = new QLineEdit{m_dim.name()};
    lay->addWidget(m_name);

    m_minBound = new iscore::SpinBox<float>;
    m_minBound->setValue(m_dim.min());
    lay->addWidget(m_minBound);

    m_maxBound = new iscore::SpinBox<float>;
    m_maxBound->setValue(m_dim.max());
    lay->addWidget(m_maxBound);

    m_remove = new QPushButton{tr("Remove")};
    lay->addWidget(m_remove);

    auto upd = new QPushButton{tr("Update")};
    lay->addWidget(upd);
    connect(upd, &QPushButton::clicked,
            this, [&] () {
        auto cmd = new UpdateDimension{dim, m_minBound->value(), m_maxBound->value()};
        m_dispatcher.submitCommand(cmd);
    });

    this->setLayout(lay);
}
}
