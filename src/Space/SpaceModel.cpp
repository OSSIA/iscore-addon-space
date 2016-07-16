#include "SpaceModel.hpp"
#include <algorithm>

namespace Space
{
SpaceModel::SpaceModel(
        const Id<SpaceModel> &id,
        QObject *parent):
    IdentifiedObject{id, staticMetaObject.className(), parent}
{
    emit spaceChanged();
}

SpaceModel::SpaceModel(const SpaceModel& other, QObject* parent):
    IdentifiedObject{other.id(), staticMetaObject.className(), parent}
{
    for(auto& dim : other.dimensions())
    {
        this->addDimension(new DimensionModel{dim, this});
    }

    for(auto& vp : other.viewports())
    {
        this->addViewport(new ViewportModel{vp, this});
    }

    m_precision = other.m_precision;
    m_defaultViewport = other.m_defaultViewport;
    emit spaceChanged();
}

SpaceModel::~SpaceModel()
{

}

Bounds SpaceModel::bounds() const
{
    const auto& x = dimension("x");
    const auto& y = dimension("y");
    return {
        x.min(),
        x.max(),
        y.min(),
        y.max(),
        double(m_precision)
    };
}

void SpaceModel::addDimension(DimensionModel *dim)
{
    m_dimensions.insert(dim);
    connect(dim, &DimensionModel::minChanged, this,
            &SpaceModel::spaceChanged);
    connect(dim, &DimensionModel::maxChanged, this,
            &SpaceModel::spaceChanged);
    emit spaceChanged();
}

const DimensionModel& SpaceModel::dimension(const QString& name) const
{
    auto it = std::find_if(m_dimensions.begin(),
                           m_dimensions.end(),
                           [&] (const DimensionModel& dim) { return dim.name() == name; });

    ISCORE_ASSERT(it != m_dimensions.end());

    return *it;
}

const DimensionModel& SpaceModel::dimension(const Id<DimensionModel> &id) const
{
    return m_dimensions.at(id);
}

void SpaceModel::removeDimension(const Id<DimensionModel> &id)
{
    auto it = m_dimensions.get().find(id);
    auto& elt = **it ;
    m_dimensions.remove(it);
    delete &elt;
    emit spaceChanged();
}


void SpaceModel::addViewport(ViewportModel* v)
{
    m_viewports.insert(v);
    if(!m_defaultViewport)
        m_defaultViewport = v->id();
    emit viewportAdded(*v);
}

void SpaceModel::removeViewport(const Id<ViewportModel>& vm)
{
    auto it = m_viewports.get().find(vm);
    auto& elt = **it ;
    m_viewports.remove(it);
    delete &elt;

    if(m_defaultViewport == vm)
    {
        if(!m_viewports.empty())
        {
            m_defaultViewport = (*m_viewports.begin()).id();
        }
        else
        {
            m_defaultViewport = Id<ViewportModel>{};
        }
    }
}


void clear(Space::SpaceModel& sp)
{
    while(!sp.dimensions().empty())
    {
        sp.removeDimension(sp.dimensions().begin()->id());
    }

    while(!sp.viewports().empty())
    {
        sp.removeViewport(sp.viewports().begin()->id());
    }
}

}
