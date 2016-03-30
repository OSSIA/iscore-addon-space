#pragma once
#include "src/Space/Dimension/DimensionModel.hpp"
#include <QtWidgets>
#include <iscore/command/Dispatchers/CommandDispatcher.hpp>
namespace Space
{
class DimensionEditWidget : public QWidget
{
    public:
        DimensionEditWidget(
                CommandDispatcher<>&,
                const DimensionModel& dim,
                QWidget* parent);

    private:
        CommandDispatcher<> m_dispatcher;
        const DimensionModel& m_dim;
        QLineEdit* m_name{};
        QDoubleSpinBox* m_minBound{};
        QDoubleSpinBox* m_maxBound{};
        QPushButton* m_remove{};
};
}
