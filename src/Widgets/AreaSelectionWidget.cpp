#include "AreaSelectionWidget.hpp"
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <iscore/widgets/MarginLess.hpp>

#include "src/Area/Generic/GenericAreaModel.hpp"
#include "src/Area/AreaFactory.hpp"
#include "src/Area/SingletonAreaFactoryList.hpp"
#include "src/Area/AreaModel.hpp"
#include "src/Area/Circle/CircleAreaModel.hpp"

namespace Space
{
AreaSelectionWidget::AreaSelectionWidget(
        const SingletonAreaFactoryList& fact,
        QWidget* parent):
    QWidget{parent}
{
    auto lay = new iscore::MarginLess<QHBoxLayout>;
    this->setLayout(lay);

    m_comboBox = new QComboBox;
    m_lineEdit = new QLineEdit;
    lay->addWidget(m_comboBox);
    lay->addWidget(m_lineEdit);

    connect(m_lineEdit, &QLineEdit::editingFinished,
            this, &AreaSelectionWidget::lineEditChanged);

    for(auto& elt : fact)
    {
        m_comboBox->addItem(
                    elt.prettyName(),
                    QVariant::fromValue(elt.concreteFactoryKey()));
    }
    connect(m_comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [&] (int index) {
        if(index == -1)
            return;

        auto key = currentAreaKey();
        if(key == GenericAreaModel::static_concreteFactoryKey())
        {
            m_lineEdit->setEnabled(true);
        }
        else
        {
            auto formula = fact.get(key)->generic_formula();
            if(formula.size() > 0)
                m_lineEdit->setText(formula.join(';'));
            m_lineEdit->setEnabled(false);
            lineEditChanged();
        }
    });
}

void AreaSelectionWidget::setCurrentArea(const AreaModel& m)
{
    m_lineEdit->setText(m.toString());
    m_comboBox->setCurrentIndex(
                m_comboBox->findData(QVariant::fromValue(m.concreteFactoryKey())));
}

void AreaSelectionWidget::setNoArea()
{
    m_lineEdit->setText("");
    m_comboBox->setCurrentIndex(0);
}

UuidKey<AreaFactory> AreaSelectionWidget::currentAreaKey() const
{
    return m_comboBox->currentData().value<UuidKey<AreaFactory>>();
}

QStringList AreaSelectionWidget::currentFormula() const
{
    return m_lineEdit->text().split(';');
}
}
