#pragma once
#include <Process/LayerPresenter.hpp>
#include <Process/Focus/FocusDispatcher.hpp>
#include <iscore/model/IdentifiedObjectMap.hpp>
#include <iscore/widgets/GraphicsItem.hpp>

#include "nano_observer.hpp"
#include "Area/AreaModel.hpp"
#include "Area/AreaPresenter.hpp"
class QMainWindow;
class QGraphicsItemGroup;
namespace Process {
class LayerModel;
class LayerView;
}

struct EmptySpaceItem : public QGraphicsItem
{
        QRectF m_rect;
    public:
        EmptySpaceItem(QGraphicsItem* parent = nullptr):
            QGraphicsItem{parent}
        {
            this->setFlag(QGraphicsItem::ItemHasNoContents, true);
        }

        void setRect(const QRectF& rect)
        {

        }

        QRectF boundingRect() const override
        { return m_rect; }

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
        { }
};
namespace Space
{
class LayerView;
class LayerPresenter :
        public Process::LayerPresenter,
        public Nano::Observer
{
    public:
        LayerPresenter(const Process::LayerModel& model,
                       Process::LayerView* view,
                       const Process::ProcessPresenterContext& ctx,
                       QObject* parent);
        ~LayerPresenter();

        void setWidth(qreal width) override;
        void setHeight(qreal height) override;

        void on_focusChanged() override;

        void putToFront() override;
        void putBehind() override;

        void on_zoomRatioChanged(ZoomRatio) override;
        void parentGeometryChanged() override;

        const Process::LayerModel &model() const override;
        const Id<Process::ProcessModel> &modelId() const override;

        void update();

    private:
        void reset();
        void on_areaAdded(const AreaModel&);
        void on_areaRemoved(const AreaModel& a);

        const Process::LayerModel& m_model;
        graphics_item_ptr<LayerView> m_view;

        QMainWindow* m_spaceWindowView{};
        IdContainer<AreaPresenter, AreaModel> m_areas;

        EmptySpaceItem* m_spaceItem{};

    public:
        void fillContextMenu(
                QMenu&,
                QPoint pos,
                QPointF scenepos,
                const Process::LayerContextMenuManager&) const override;
};
}
