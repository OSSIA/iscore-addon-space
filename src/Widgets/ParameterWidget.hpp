#pragma once
#include <QWidget>
class QLineEdit;
class QDoubleSpinBox;
namespace Explorer
{
class DeviceExplorerModel;
class AddressEditWidget;
}
namespace State
{
struct Address;
struct Value;
}
namespace Space
{
class ParameterWidget : public QWidget
{
    public:
        ParameterWidget(Explorer::DeviceExplorerModel*, QWidget* parent);

        void setAddress(const State::Address& addr);
        void setValue(double val);

        State::Address address() const;
        ossia::value value() const;

    private:
        Explorer::AddressEditWidget* m_address{};
        QDoubleSpinBox* m_defaultValue{};
};
}
