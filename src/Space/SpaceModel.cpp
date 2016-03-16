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

void SpaceModel::removeDimension(const QString &name)
{
    ISCORE_TODO;
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
    ISCORE_TODO;

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
}
