#pragma once
#include <QtWidgets>

#include <iscore/widgets/MarginLess.hpp>
#include <iscore/command/Dispatchers/CommandDispatcher.hpp>
namespace iscore
{ struct DocumentContext; }
namespace Space
{
class SpaceModel;
class SpaceTab : public QWidget
{
        Q_OBJECT
    public:
        SpaceTab(
                const iscore::DocumentContext& ctx,
                const SpaceModel& space,
                QWidget *parent);

    private:
        CommandDispatcher<> m_dispatcher;
        const SpaceModel& m_space;
        QVBoxLayout* m_dimensionLayout{};
        QVBoxLayout* m_viewportLayout{};
        QPushButton* m_addDim{}, *m_addViewport{};
};
}
