#include "AddArea.hpp"

#include "src/Area/AreaModel.hpp"
#include "src/Area/Generic/GenericAreaModel.hpp"
#include "src/Area/AreaParser.hpp"
#include "src/Space/SpaceModel.hpp"

#include "src/Area/AreaFactory.hpp"
#include "src/Area/SingletonAreaFactoryList.hpp"

#include "src/SpaceProcess.hpp"
#include "src/Area/Generic/AreaComputer.hpp"

#include <iscore/application/ApplicationContext.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <iscore/tools/SettableIdentifierGeneration.hpp>
#include <iscore/tools/ModelPathSerialization.hpp>
#include <iscore/plugins/customfactory/StringFactoryKeySerialization.hpp>

#include <src/Area/Circle/CircleAreaModel.hpp>
#include <src/Area/Pointer/PointerAreaModel.hpp>
#include <iscore/tools/std/StdlibWrapper.hpp>

namespace Space
{

template<int Pres>
QPolygonF circleToPoly(CircleAreaModel::values val)
{
    QPolygonF poly;
    const double f_pres = Pres;
    for(int i = 0; i < Pres; i++)
    {
        poly.append(val.center + val.r * QPointF(
                        std::cos((i / f_pres) * 2. * M_PI),
                        std::sin((i / f_pres) * 2. * M_PI)
                        ));
    }
    return poly;
}

using AreaComputationFun = std::function<bool(const AreaModel& a1, const AreaModel& a2)>;

class CollisionHandler : public QObject
{
        std::map<KeyPair<UuidKey<AreaFactory>>, AreaComputationFun> m_handlers;
        AreaComputationFun m_genericHandler;
        MatrixCollisionComputer* m_cptr;

        std::map<KeyPair<Id<AreaModel>>, bool> m_genericResult;

    public:
        CollisionHandler():
            m_cptr{new MatrixCollisionComputer} // TODO delete
        {
            m_cptr->setParent(this);
            connect(m_cptr, &MatrixCollisionComputer::ready, this,
                    [=] (bool res, KeyPair<Id<AreaModel>> keys) {
                m_genericResult[keys] = res;
            }, Qt::QueuedConnection);

            m_handlers.insert(
                        std::make_pair(
                        make_keys(
                                CircleAreaModel::static_concreteFactoryKey(),
                                CircleAreaModel::static_concreteFactoryKey()),
                              [] (const AreaModel& a1, const AreaModel& a2)
            {
                auto& c1 = static_cast<const CircleAreaModel&>(a1);
                auto& c2 = static_cast<const CircleAreaModel&>(a2);

                auto c1_val = c1.mapToData(c1.currentMapping());
                auto c2_val = c2.mapToData(c2.currentMapping());

                auto c1_poly = a1.transform().map(circleToPoly<12>(c1_val));
                auto c2_poly = a2.transform().map(circleToPoly<12>(c2_val));
                return !c1_poly.intersected(c2_poly).isEmpty();

            }));

            m_handlers.insert(
                        std::make_pair(
                        make_keys(
                                CircleAreaModel::static_concreteFactoryKey(),
                                PointerAreaModel::static_concreteFactoryKey()),
                              [] (const AreaModel& a1, const AreaModel& a2)
            {
                auto& c = static_cast<const CircleAreaModel&>(a1);
                auto& p = static_cast<const PointerAreaModel&>(a2);

                auto c_val = c.mapToData(c.currentMapping());
                auto p_val = p.mapToData(p.currentMapping());

                auto c_poly = a1.transform().map(circleToPoly<12>(c_val));
                return c_poly.contains(a2.transform().map(p_val.center));
            }));
        }

        bool genericHandler(const AreaModel& a1, const AreaModel& a2)
        {
            auto f1 = Computations::make_funs(a1.formula());
            auto f2 = Computations::make_funs(a2.formula());

            for(auto& f : f1)
            {
                for(auto val : a1.currentMapping())
                {
                    f.lhs->SetScalarVariableValue(val.first.c_str(), val.second);
                    f.rhs->SetScalarVariableValue(val.first.c_str(), val.second);
                }
            }

            for(auto& f : f2)
            {
                for(auto val : a2.currentMapping())
                {
                    f.lhs->SetScalarVariableValue(val.first.c_str(), val.second);
                    f.rhs->SetScalarVariableValue(val.first.c_str(), val.second);
                }
            }

            // Check if a1 == a2 for any value (TODO or more generally if the {f1, f2} system admits a solution)
            auto p = make_keys(a1.id(), a2.id());
            m_cptr->computeArea(
                        a1.space().bounds(),
                        a1.spaceMapping(),
                        f1, f2,
                        a1.transform(), a2.transform(),
                        p);
            auto it = m_genericResult.find(p);
            if(it != m_genericResult.end())
            {
                return it->second;
            }
            else
            {
                return false;
            }
        }

        void inscribe(std::pair<KeyPair<UuidKey<AreaFactory>>, AreaComputationFun> val)
        {
            m_handlers.insert(val);
        }

        bool check(const AreaModel& a1, const AreaModel& a2)
        {
            auto it = m_handlers.find(make_keys(a1.concreteFactoryKey(), a2.concreteFactoryKey()));
            if(it != m_handlers.end())
            {
                return it->second(a1, a2);
            }
            else
            {
                return genericHandler(a1, a2);
            }

            return false;
        }
};


class DistanceHandler : public QObject
{
        std::map<KeyPair<UuidKey<AreaFactory>>, AreaComputationFun> m_handlers;
        AreaComputationFun m_genericHandler;
        MatrixDistanceComputer* m_cptr;

        std::map<KeyPair<Id<AreaModel>>, double> m_genericResult;

    public:
        DistanceHandler():
            m_cptr{new MatrixDistanceComputer} // TODO delete
        {
            m_cptr->setParent(this);
            connect(m_cptr, &MatrixDistanceComputer::ready, this, [=] (double d, KeyPair<Id<AreaModel>> keys) {
                m_genericResult[keys] = d;
            }, Qt::QueuedConnection);

            m_handlers.insert(
                        std::make_pair(
                        make_keys(
                                CircleAreaModel::static_concreteFactoryKey(),
                                CircleAreaModel::static_concreteFactoryKey()),
                              [] (const AreaModel& a1, const AreaModel& a2)
            {
                auto& c1 = static_cast<const CircleAreaModel&>(a1);
                auto& c2 = static_cast<const CircleAreaModel&>(a2);

                auto c1_val = c1.mapToData(c1.currentMapping());
                auto c2_val = c2.mapToData(c2.currentMapping());

                // Distance of centers
                auto dist = [] (auto v1, auto v2) {
                    return std::sqrt(std::pow(v2.center.x() - v1.center.x(), 2) + std::pow(v2.center.y() - v1.center.y(), 2));
                };
                return dist(c1_val, c2_val);
            }));

            m_handlers.insert(
                        std::make_pair(
                        make_keys(
                                CircleAreaModel::static_concreteFactoryKey(),
                                PointerAreaModel::static_concreteFactoryKey()),
                              [] (const AreaModel& a1, const AreaModel& a2)
            {
                auto& c = static_cast<const CircleAreaModel&>(a1);
                auto& p = static_cast<const PointerAreaModel&>(a2);

                auto c_val = c.mapToData(c.currentMapping());
                auto p_val = p.mapToData(p.currentMapping());

                // Check if the distance of both centers is < to the sum of radiuses
                auto dist = [] (auto v1, auto v2) {
                    return std::sqrt(std::pow(v2.center.x() - v1.center.x(), 2) + std::pow(v2.center.y() - v1.center.y(), 2));
                };
                return dist(c_val, p_val);
            }));
        }

        double genericHandler(const AreaModel& a1, const AreaModel& a2)
        {
            auto f1 = Computations::make_funs(a1.formula());
            auto f2 = Computations::make_funs(a2.formula());

            for(auto& f : f1)
            {
                for(auto val : a1.currentMapping())
                {
                    f.lhs->SetScalarVariableValue(val.first.c_str(), val.second);
                    f.rhs->SetScalarVariableValue(val.first.c_str(), val.second);
                }
            }

            for(auto& f : f2)
            {
                for(auto val : a2.currentMapping())
                {
                    f.lhs->SetScalarVariableValue(val.first.c_str(), val.second);
                    f.rhs->SetScalarVariableValue(val.first.c_str(), val.second);
                }
            }

            // Check if a1 == a2 for any value (TODO or more generally if the {f1, f2} system admits a solution)
            auto p = make_keys(a1.id(), a2.id());
            m_cptr->computeArea(
                        a1.space().bounds(),
                        a1.spaceMapping(),
                        f1, f2,
                        a1.transform(), a2.transform(),
                        p);
            auto it = m_genericResult.find(p);
            if(it != m_genericResult.end())
            {
                return it->second;
            }
            else
            {
                return false;
            }
        }

        void inscribe(std::pair<KeyPair<UuidKey<AreaFactory>>, AreaComputationFun> val)
        {
            m_handlers.insert(val);
        }

        double get(const AreaModel& a1, const AreaModel& a2)
        {
            auto it = m_handlers.find(make_keys(a1.concreteFactoryKey(), a2.concreteFactoryKey()));
            if(it != m_handlers.end())
            {
                return it->second(a1, a2);
            }
            else
            {
                return genericHandler(a1, a2);
            }

            return false;
        }
};

AddArea::AddArea(Path<Space::ProcessModel> &&spacProcess,
                 UuidKey<AreaFactory> type,
                 const QStringList &area,
                 const QMap<Id<DimensionModel>, QString> &dimMap,
                 const QMap<QString, Device::FullAddressSettings> &addrMap):
    m_path{std::move(spacProcess)},
    m_areaType{type},
    m_areaFormula{area},
    m_spaceMap{dimMap},
    m_symbolToAddressMap{addrMap}
{
    auto& process = m_path.find();
    m_createdAreaId = getStrongId(process.areas);

    m_createdComputations = getStrongIdRange<ComputationModel>(
                                process.areas.size(),
                                process.computations);
}

void AddArea::undo() const
{
    auto& proc = m_path.find();
    for(auto& id : m_createdComputations)
        proc.computations.remove(id);
    proc.areas.remove(m_createdAreaId);
}

void AddArea::redo() const
{
    auto& proc = m_path.find();

    auto factory = context.components.factory<SingletonAreaFactoryList>().get(m_areaType);
    ISCORE_ASSERT(factory);

    AreaModel* ar = factory->makeModel(m_areaFormula, proc.context(), m_createdAreaId, &proc);

    ar->setSpaceMapping(m_spaceMap);
    ar->setParameterMapping(m_symbolToAddressMap);

    /// temporarily create "collision" computations
    int i = 0;
    for(auto& area : proc.areas)
    {
        { // collisions
            auto ch = new CollisionHandler;
            auto comp = new ComputationModel(ch, [&,ar, ch] () {
                return (double) ch->check(area, *ar);
            },
                proc.space(),
                m_createdComputations[i++],
                &proc);

            comp->metadata.setName(QString("coll_%1_%2")
                                   .arg(area.metadata.name())
                                   .arg(ar->metadata.name()));

            proc.computations.add(comp);
        }
        // TODO distance ?
        // TODO js ?
    }


    proc.areas.add(ar);
}
void AddArea::serializeImpl(DataStreamInput &s) const
{
    s << m_path << m_createdAreaId << m_areaType << m_areaFormula << m_spaceMap << m_symbolToAddressMap;


    auto& vec = m_createdComputations;
    s << (int32_t)vec.size();
    for(const auto& elt : vec)
        s << elt; ;
}

void AddArea::deserializeImpl(DataStreamOutput &s)
{
    s >> m_path >> m_createdAreaId >> m_areaType >> m_areaFormula >> m_spaceMap >> m_symbolToAddressMap;

    int32_t n;
    auto& vec = m_createdComputations;
    s >> n;
    vec.reserve(n);
    for(;n-->0;)
    {
        Id<ComputationModel> val;
        s >> val;
        vec.push_back(val);
    }
}
}
