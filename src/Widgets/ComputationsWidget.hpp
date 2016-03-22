#pragma once
#include <QtWidgets>
#include <iscore/command/Dispatchers/CommandDispatcher.hpp>
#include <nano_observer.hpp>
namespace iscore
{ struct DocumentContext; }
namespace Explorer
{ class AddressEditWidget; }
namespace Space {
class AreaModel;
class ProcessModel;
class ComputationModel;
class ComputationsWidget : public QWidget, public Nano::Observer
{
    public:
        ComputationsWidget(
                const iscore::DocumentContext& ctx,
                const Space::ProcessModel &space,
                QWidget* parent);


        // If null, will add a new area instead.
        void setActiveComputation(const ComputationModel *);

    private:
        void validate();
        void cleanup();
        void loadComputation(const ComputationModel& comp);
        void newComputation();

        void rebuildAreas();
        void areaAdded(const AreaModel&);
        void areaRemoved(const AreaModel&);

    private:
        CommandDispatcher<> m_dispatcher;
        const Space::ProcessModel& m_space;
        const ComputationModel* m_computation{};

        QComboBox* m_a1{}, *m_a2{}, *m_type{};

        Explorer::AddressEditWidget* m_address{};
};
}
