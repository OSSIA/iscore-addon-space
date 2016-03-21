#pragma once
#include <QWidget>
#include <iscore/plugins/customfactory/UuidKey.hpp>
class QComboBox;
class QLineEdit;

namespace Space
{
class AreaFactory;
class AreaModel;
class AreaFactoryList;

class AreaSelectionWidget : public QWidget
{
        Q_OBJECT
    public:
        AreaSelectionWidget(
                const AreaFactoryList& fact,
                QWidget* parent);

        void setCurrentArea(const AreaModel& m);
        void setNoArea();

        UuidKey<AreaFactory> currentAreaKey() const;
        QStringList currentFormula() const;

        QLineEdit* lineEdit() const { return m_lineEdit; }

    signals:
        void lineEditChanged();

    private:
        QComboBox* m_comboBox{};
        QLineEdit* m_lineEdit{};
};
}
