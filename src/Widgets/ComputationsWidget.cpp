#include "ComputationsWidget.hpp"
#include <iscore/document/DocumentContext.hpp>
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
    auto lay = new QGridLayout;
    this->setLayout(lay);

    /*
    m_selectionWidget = new AreaSelectionWidget{ctx.app.components.factory<SingletonAreaFactoryList>(), this};
    lay->addWidget(m_selectionWidget);
    connect(m_selectionWidget, &AreaSelectionWidget::lineEditChanged, this, &AreaWidget::on_formulaChanged);

    // This contains a list of comboboxes mapping each dimension of the space we're in,
    // to a parameter of the area
    {
        auto dimBox = new QGroupBox{tr("Dimensions to area parameter")};
        lay->addWidget(dimBox);
        m_spaceMappingLayout = new iscore::MarginLess<QFormLayout>;
        dimBox->setLayout(m_spaceMappingLayout);
        for(const DimensionModel& dim : m_space.space().dimensions())
        {
            auto cb = new QComboBox;
            m_spaceMappingLayout->addRow(dim.name(), cb);
            connect(cb,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &AreaWidget::on_dimensionMapped);
        }
    }

    {
        // The remaining, non-space-mapped parameters are here.
        auto paramBox = new QGroupBox{tr("Valued parameters")};
        lay->addWidget(paramBox);
        m_paramMappingLayout = new iscore::MarginLess<QFormLayout>;
        paramBox->setLayout(m_paramMappingLayout);
    }

    QPushButton* val = new QPushButton{"Create / update"};
    lay->addWidget(val);

    connect(val, &QPushButton::pressed, this, &AreaWidget::validate);
    */
}

void ComputationsWidget::setActiveComputation(const ComputationModel*)
{

}

}
