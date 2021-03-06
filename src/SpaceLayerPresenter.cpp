#include "SpaceLayerPresenter.hpp"
#include "SpaceLayerModel.hpp"
#include "SpaceLayerView.hpp"
#include "SpaceProcess.hpp"
#include <src/Space/SpaceModel.hpp>
#include <src/Space/Viewport/ViewportModel.hpp>
#include <src/Space/Dimension/DimensionModel.hpp>
#include <iscore/widgets/GraphicsItem.hpp>

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include "Widgets/SpaceGuiWindow.hpp"
#include <iscore/document/DocumentInterface.hpp>
#include <core/document/Document.hpp>
#include <QMainWindow>

#include <src/SpaceProcess.hpp>
#include "src/Area/AreaFactory.hpp"
#include "src/Area/SingletonAreaFactoryList.hpp"


namespace Space
{
LayerPresenter::LayerPresenter(
        const Process::LayerModel& model,
        Process::LayerView* view,
        const Process::ProcessPresenterContext& ctx,
        QObject* parent):
    Process::LayerPresenter{ctx, parent},
    m_model{static_cast<const Space::Layer&>(model)},
    m_view{static_cast<LayerView*>(view)},
    m_spaceItem{new EmptySpaceItem{m_view}}
{
    const auto& procmodel = static_cast<Space::ProcessModel&>(m_model.processModel());
    m_spaceWindowView = new QMainWindow;
    m_spaceWindowView->setCentralWidget(
                new SpaceGuiWindow{
                    m_context.context,
                    procmodel,
                    m_spaceWindowView});

    connect(m_view, &LayerView::guiRequested,
            m_spaceWindowView, &QWidget::show);

    con(procmodel.space(), &SpaceModel::spaceChanged,
        this, &LayerPresenter::update);
    connect(m_view, &LayerView::contextMenuRequested,
            this, &LayerPresenter::contextMenuRequested);
    for(const auto& area : procmodel.areas)
    {
        on_areaAdded(area);
    }

    procmodel.areas.added.connect<LayerPresenter, &LayerPresenter::on_areaAdded>(this);
    procmodel.areas.removing.connect<LayerPresenter, &LayerPresenter::on_areaRemoved>(this);
    m_view->setEnabled(true);

    parentGeometryChanged();
}

LayerPresenter::~LayerPresenter()
{
}

void LayerPresenter::setWidth(qreal width)
{
    m_view->setWidth(width);
    reset();
    update();
}

void LayerPresenter::setHeight(qreal height)
{
    m_view->setHeight(height);
    reset();
    update();
}
void LayerPresenter::on_focusChanged()
{

}

void LayerPresenter::putToFront()
{
    m_view->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
    update();
}

void LayerPresenter::putBehind()
{
    m_view->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    update();
}

void LayerPresenter::on_zoomRatioChanged(ZoomRatio)
{
    reset();
    update();
}

void LayerPresenter::parentGeometryChanged()
{
    reset();
    update();
}

const Process::LayerModel &LayerPresenter::model() const
{
    return m_model;
}

const Id<Process::ProcessModel> &LayerPresenter::modelId() const
{
    return m_model.processModel().id();
}

void LayerPresenter::update()
{
    m_view->update();
    for(auto& pres : m_areas)
    {
        pres.update();
    }
}

void LayerPresenter::reset()
{
    const auto& procmodel = static_cast<Space::ProcessModel&>(m_model.processModel());

    // x0 is vp.XDim.min, etc...
    auto& space = procmodel.space();

    const DimensionModel& xDim = space.dimension("x");
    const DimensionModel& yDim = space.dimension("y");

    QTransform t;
    t.scale(m_view->width() / (xDim.max() - xDim.min()),
            m_view->height() / (yDim.max() - yDim.min()));
    t.translate(-xDim.min(), -yDim.min());

    m_spaceItem->setTransform(t);
}

void LayerPresenter::on_areaAdded(const AreaModel & a)
{
    AreaFactory* fact = m_context.context.app.interfaces<AreaFactoryList>().get(a.concreteKey());

    auto v = fact->makeView(m_spaceItem);

    auto pres = fact->makePresenter(v, a, this);

    con(a, &AreaModel::areaChanged,
        pres, &AreaPresenter::on_areaChanged,
        Qt::QueuedConnection);
    m_areas.insert(pres);
    pres->on_areaChanged(a.currentMapping());
    update();
}

void LayerPresenter::on_areaRemoved(
        const AreaModel & a)
{
    auto& map = m_areas.get();
    auto it = map.find(a.id());
    if(it != map.end())
    {
        delete *it;
        map.erase(it);
    }

    update();
}


void LayerPresenter::fillContextMenu(
        QMenu& menu,
        QPoint pos,
        QPointF scenepos,
        const Process::LayerContextMenuManager&) const
{
    auto act = menu.addAction(tr("Show"));
    connect(act, &QAction::triggered, this, [&] () {
       m_spaceWindowView->show();
    });
}
}
