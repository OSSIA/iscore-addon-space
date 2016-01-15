#include "AreaWidget.hpp"
#include <QFormLayout>
#include "src/Space/SpaceModel.hpp"
#include "src/SpaceProcess.hpp"
#include "src/Area/AreaModel.hpp"
#include "src/Area/AreaParser.hpp"

#include "src/Commands/AddArea.hpp"
#include "AreaSelectionWidget.hpp"
#include "ParameterWidget.hpp"
#include <iscore/widgets/MarginLess.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <iscore/document/DocumentContext.hpp>
#include <src/Area/SingletonAreaFactoryList.hpp>
#include <src/Space/SpaceModel.hpp>
#include <src/Space/DimensionModel.hpp>
namespace Space
{
AreaWidget::AreaWidget(
        const iscore::DocumentContext& ctx,
        const Space::ProcessModel& space,
        QWidget *parent):
    QWidget{parent},
    m_dispatcher{ctx.commandStack},
    m_space{space}
{
    auto lay = new QGridLayout;
    this->setLayout(lay);

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
}

void AreaWidget::setActiveArea(const AreaModel *area)
{
    m_area = area;

    if(m_area)
    {
        m_selectionWidget->lineEdit()->setText(m_area->toString());
        m_selectionWidget->comboBox()->setCurrentIndex(
                    m_selectionWidget->comboBox()->findData(m_area->type()));
    }
    else
    {
        m_selectionWidget->lineEdit()->clear();
    }
    on_formulaChanged();

    if(m_area)
    {
        // Load all the data
        const auto& dim_map = area->spaceMapping();
        for(int symb_i = 0; symb_i < m_spaceMappingLayout->rowCount(); symb_i++)
        {
            auto label = static_cast<QLabel*>(m_spaceMappingLayout->itemAt(symb_i, QFormLayout::ItemRole::LabelRole)->widget());
            ISCORE_ASSERT(label);
            auto cb = static_cast<QComboBox*>(m_spaceMappingLayout->itemAt(symb_i, QFormLayout::ItemRole::FieldRole)->widget());
            ISCORE_ASSERT(cb);

            auto keys = dim_map.keys();
            auto& dims = m_space.space().dimensions();
            auto dim_map_it = find_if(keys,
                                     [&] (const Id<DimensionModel>& exp) {

                auto dim_it = dims.find(exp);
                if(dim_it != dims.end())
                    return dim_it->name() == label->text();
                return false;
            });
            if(dim_map_it == keys.end())
            {
                // Error : the requested dimension does not seem to exist in our space.
                continue;
            }
            // Find it in the combobox

            cb->setCurrentIndex(cb->findText(dim_map[*dim_map_it]));
        }



        const auto& param_map = area->parameterMapping();

        for(int param_i = 0; param_i < m_paramMappingLayout->rowCount(); param_i++)
        {
            auto it = m_paramMappingLayout->itemAt(param_i, QFormLayout::ItemRole::LabelRole);
            if(!it)
                continue;

            auto widg = it->widget();
            if(!widg)
                continue;

            auto label = qobject_cast<QLabel*>(widg);
            ISCORE_ASSERT(label);
            auto param_widg = static_cast<ParameterWidget*>(m_paramMappingLayout->itemAt(param_i, QFormLayout::ItemRole::FieldRole)->widget());
            ISCORE_ASSERT(param_widg);
            param_widg->setEnabled(false);

            auto param_it = param_map.find(label->text());
            if(param_it == param_map.end())
            {
                // The parameter is not mapped
                continue;
            }

            // Storing a value
            param_widg->defaultValue()->setValue(State::convert::value<double>((*param_it).value));

            // Storing an address
            param_widg->address()->setText((*param_it).address.toString());

            param_widg->setEnabled(true);
        }

    }
}



void AreaWidget::on_formulaChanged()
{
    cleanup();

    auto formulas = m_selectionWidget->lineEdit()->text().split(';');
    AreaParser parser{formulas};
    if(!parser.check())
    {
        return;
    }

    auto area = parser.result();
    for(int i = 0; i < m_spaceMappingLayout->rowCount(); i++)
    {
        auto cb = qobject_cast<QComboBox*>(m_spaceMappingLayout->itemAt(i, QFormLayout::ItemRole::FieldRole)->widget());
        cb->addItem(""); // What happens if left blank ? e.g. x=5 in 2D space ? Should just do the correct stuff.
        for(const auto& sym: area->symbols())
        {
            cb->addItem(QString::fromStdString(sym.get_name()));
        }
    }

    for(const auto& sym: area->symbols())
    {
        m_paramMappingLayout->addRow(QString::fromStdString(sym.get_name()), new ParameterWidget);
    }
}


void AreaWidget::on_dimensionMapped(int)
{
    // Disable the line corresponding to the mapped dimensions in the m_paramMappingLayout.
    for(int param_i = 0; param_i < m_paramMappingLayout->rowCount(); param_i++)
    {
        auto it = m_paramMappingLayout->itemAt(param_i, QFormLayout::ItemRole::LabelRole);
        if(!it)
            continue;

        auto widg = it->widget();
        if(!widg)
            continue;

        auto label = qobject_cast<QLabel*>(widg);
        auto field = m_paramMappingLayout->itemAt(param_i, QFormLayout::ItemRole::FieldRole)->widget();
        field->setEnabled(true);

        auto param_name = label->text();
        for(int symb_i = 0; symb_i < m_spaceMappingLayout->rowCount(); symb_i++)
        {
            auto cb = static_cast<QComboBox*>(m_spaceMappingLayout->itemAt(symb_i, QFormLayout::ItemRole::FieldRole)->widget());
            if(cb->currentText() == param_name)
            {
                field->setEnabled(false);
            }
        }
    }
}

void AreaWidget::validate()
{
    // Make the dimension map
    QMap<Id<DimensionModel>, QString> dim_map;
    for(int symb_i = 0; symb_i < m_spaceMappingLayout->rowCount(); symb_i++)
    {
        auto label = static_cast<QLabel*>(m_spaceMappingLayout->itemAt(symb_i, QFormLayout::ItemRole::LabelRole)->widget());
        ISCORE_ASSERT(label);
        auto cb = static_cast<QComboBox*>(m_spaceMappingLayout->itemAt(symb_i, QFormLayout::ItemRole::FieldRole)->widget());
        ISCORE_ASSERT(cb);

        if(cb->currentText() != "")
        {
            // TODO find a way to save the dim id directly instead.
            dim_map.insert(m_space.space().dimension(label->text()).id(), cb->currentText());
        }
    }


    QMap<QString, Device::FullAddressSettings> param_map;
    for(int param_i = 0; param_i < m_paramMappingLayout->rowCount(); param_i++)
    {
        auto it = m_paramMappingLayout->itemAt(param_i, QFormLayout::ItemRole::LabelRole);
        if(!it)
            continue;

        auto widg = it->widget();
        if(!widg)
            continue;

        auto label = qobject_cast<QLabel*>(widg);
        ISCORE_ASSERT(label);
        auto param_widg = static_cast<ParameterWidget*>(m_paramMappingLayout->itemAt(param_i, QFormLayout::ItemRole::FieldRole)->widget());
        ISCORE_ASSERT(param_widg);

        if(!param_widg->isEnabled())
            continue;

        Device::FullAddressSettings addr;
        if(State::Address::validateString(param_widg->address()->text()))
        {
            addr.address = State::Address::fromString(param_widg->address()->text());
        }
        addr.value = State::convert::toValue(param_widg->defaultValue()->value());
        param_map.insert(label->text(), addr);
    }


    if(m_area)
    {
        // Make a UpdateArea command.
        ISCORE_TODO;
    }
    else
    {

        m_dispatcher.submitCommand(
                    new AddArea{
                        m_space,
                        m_selectionWidget->comboBox()->currentData().value<AreaFactoryKey>(),
                        QStringList{m_selectionWidget->lineEdit()->text().split(';')}, // TODO multiline
                        dim_map,
                        param_map});
    }
}

void AreaWidget::cleanup()
{
    for(int i = 0; i < m_spaceMappingLayout->rowCount(); i++)
    {
        auto cb = qobject_cast<QComboBox*>(m_spaceMappingLayout->itemAt(i, QFormLayout::ItemRole::FieldRole)->widget());
        cb->clear();
    }

    while (m_paramMappingLayout->count() != 0)
    {
        auto item = m_paramMappingLayout->takeAt(0);
        item->widget()->deleteLater();
        delete item;
    }
}
}
