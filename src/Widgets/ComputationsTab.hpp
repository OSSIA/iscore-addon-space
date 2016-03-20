#pragma once
#include <QtWidgets>
#include <nano_observer.hpp>

namespace iscore {
struct DocumentContext;
}
namespace Space
{
class ComputationModel;
class ComputationsWidget;
class ProcessModel;
class ComputationsTab : public QWidget,
        public Nano::Observer
{
        Q_OBJECT
    public:
        ComputationsTab(
                const iscore::DocumentContext& ctx,
                const Space::ProcessModel &space,
                QWidget *parent);

    private:
        void rebuildList();
        void on_compAdded(const ComputationModel&);
        void on_compRemoved(const ComputationModel&);

        const Space::ProcessModel &m_space;

        QListWidget* m_compList{};
        ComputationsWidget* m_compWidget{};
        QPushButton* m_newComp{};
};
}

