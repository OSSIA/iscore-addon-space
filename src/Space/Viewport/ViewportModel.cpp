#include "ViewportModel.hpp"

namespace Space
{
const Id<DimensionModel>& ViewportModel::xDim() const
{
    return m_xDim;
}

void ViewportModel::setXDim(const Id<DimensionModel>& xDim)
{
    m_xDim = xDim;
}

const Id<DimensionModel>& ViewportModel::yDim() const
{
    return m_yDim;
}

void ViewportModel::setYDim(const Id<DimensionModel>& yDim)
{
    m_yDim = yDim;
}
/*
const QMap<Id<DimensionModel>, double>& ViewportModel::defaultValuesMap() const
{
    return m_defaultValuesMap;
}

void ViewportModel::setDefaultValuesMap(const QMap<Id<DimensionModel>, double>& defaultValuesMap)
{
    m_defaultValuesMap = defaultValuesMap;
}
*/

const QString& ViewportModel::name() const
{
    return m_name;
}

void ViewportModel::setName(const QString& name)
{
    m_name = name;
}
}
