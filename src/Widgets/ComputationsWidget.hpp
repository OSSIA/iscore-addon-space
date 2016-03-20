#pragma once
#include <QtWidgets>
#include <iscore/command/Dispatchers/CommandDispatcher.hpp>
namespace iscore
{ struct DocumentContext; }

namespace Space {
class ProcessModel;
class ComputationModel;
class ComputationsWidget : public QWidget
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

    private:
        CommandDispatcher<> m_dispatcher;
        const Space::ProcessModel& m_space;
        const ComputationModel* m_computation{};
};
}
