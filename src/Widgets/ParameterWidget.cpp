#include "ParameterWidget.hpp"
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <iscore/widgets/MarginLess.hpp>
#include <iscore/widgets/SpinBoxes.hpp>

#include <Explorer/Widgets/AddressEditWidget.hpp>
#include <State/Value.hpp>
namespace Space
{
ParameterWidget::ParameterWidget(
        Explorer::DeviceExplorerModel* model,
        QWidget* parent):
    QWidget{parent}
{
    auto lay = new iscore::MarginLess<QHBoxLayout>;
    this->setLayout(lay);

    m_address = new Explorer::AddressEditWidget{*model, this};
    m_defaultValue = new iscore::SpinBox<float>{this};
    lay->addWidget(m_address);
    lay->addWidget(m_defaultValue);
}

State::Address ParameterWidget::address() const
{
    return m_address->address();
}

void ParameterWidget::setAddress(const State::Address& addr)
{
    m_address->setAddress(addr);
}

void ParameterWidget::setValue(double val)
{
    m_defaultValue->setValue(val);
}

State::Value ParameterWidget::value() const
{
    return State::Value::fromValue(m_defaultValue->value());
}
}
