#pragma once
#include <src/Bounds.hpp>
#include <src/Area/AreaParser.hpp>
#include <Space/area.hpp>
#include <Space/square_renderer.hpp>
#include <atomic>
#include <QThread>
#include <QMetaType>
#include <QPainterPath>
#include <src/Area/Generic/AreaComputerProxy.hpp>
#include <vtk/vtkFunctionParser.h>
#include <iscore/tools/IdentifiedObject.hpp>
#include <vtk/vtkSmartPointer.h>

#include <QVector2D>
namespace Space
{
class AreaModel;
template<typename K>
struct KeyPair : std::pair<K, K>
{
    public:
        using std::pair<K, K>::pair;

        friend bool operator==(
                const KeyPair& lhs,
                const KeyPair& rhs)
        {
            return (lhs.first == rhs.first && lhs.second == rhs.second)
                    ||
                   (lhs.first == rhs.second && lhs.second == rhs.first);
        }

        friend bool operator!=(
                const KeyPair& lhs,
                const KeyPair& rhs)
        {
            return !(lhs == rhs);
        }
};
template<typename K>
auto make_keys(const K& k1, const K& k2)
{
    return KeyPair<K>{k1, k2};
}

using pair_t = std::pair<
    QStringList,
    GiNaC::relational::operators
>;
struct VtkFun
{
        vtkSmartPointer<vtkFunctionParser> lhs = vtkSmartPointer<vtkFunctionParser>::New();
        vtkSmartPointer<vtkFunctionParser> rhs = vtkSmartPointer<vtkFunctionParser>::New();
        GiNaC::relational::operators op{};
};

struct Computations
{
        static std::vector<VtkFun> make_funs(const QStringList& formulas)
        {
            std::vector<VtkFun> res;
            auto vec = AreaParser::splitRelationships(formulas);

            for(pair_t& form : vec)
            {
                VtkFun f;
                f.lhs->SetFunction(form.first[0].toLatin1().constData());
                f.rhs->SetFunction(form.first[1].toLatin1().constData());

                f.op = form.second;
                res.push_back(f);

                // Evaluate both parts
                // Check if the relation works
            }

            return res;
        }

        static bool compare(double lhs_res, double rhs_res, GiNaC::relational::operators op)
        {
            switch(op)
            {
                case GiNaC::relational::operators::equal:
                {
                    return lhs_res == rhs_res;
                }
                case GiNaC::relational::operators::not_equal:
                {
                    return lhs_res != rhs_res;
                }
                case GiNaC::relational::operators::less:
                {
                    return lhs_res < rhs_res;
                }
                case GiNaC::relational::operators::less_or_equal:
                {
                    return lhs_res <= rhs_res;
                }
                case GiNaC::relational::operators::greater:
                {
                    return lhs_res > rhs_res;
                }
                case GiNaC::relational::operators::greater_or_equal:
                {
                    return lhs_res >= rhs_res;
                }
            }
        }

        template<typename Fun>
        static void do_for(Bounds b, SpaceMap sm, std::vector<VtkFun>& vtk_vec, Fun f)
        {
            auto x_str = sm.begin()->toStdString().c_str();
            auto y_str = (++sm.begin())->toStdString().c_str();

            for(int j = b.min_y; j < b.max_y; j += b.side)
            {
                double y = j;
                for(VtkFun& fun : vtk_vec)
                {
                    fun.lhs->SetScalarVariableValue(y_str, y);
                    fun.rhs->SetScalarVariableValue(y_str, y);
                }

                for(int i = b.min_x; i < b.max_x; i += b.side)
                {
                    double x = i;

                    bool res = true;
                    for(VtkFun& fun : vtk_vec)
                    {
                        fun.lhs->SetScalarVariableValue(x_str, x);
                        fun.rhs->SetScalarVariableValue(x_str, x);

                        res &= compare(
                                       fun.lhs->GetScalarResult(),
                                       fun.rhs->GetScalarResult(),
                                       fun.op);
                        if(!res)
                            break;
                    }

                    if(res)
                    {
                        f(x, y);
                    }
                }
            }
        }

        static bool find_true(Bounds b, SpaceMap sm, std::vector<VtkFun>& vtk_vec)
        {
            auto x_str = sm.begin()->toStdString().c_str();
            auto y_str = (++sm.begin())->toStdString().c_str();

            for(int j = b.min_y; j < b.max_y; j += b.side)
            {
                double y = j;
                for(VtkFun& fun : vtk_vec)
                {
                    fun.lhs->SetScalarVariableValue(y_str, y);
                    fun.rhs->SetScalarVariableValue(y_str, y);
                }

                for(int i = b.min_x; i < b.max_x; i += b.side)
                {
                    double x = i;

                    bool res = true;
                    for(VtkFun& fun : vtk_vec)
                    {
                        fun.lhs->SetScalarVariableValue(x_str, x);
                        fun.rhs->SetScalarVariableValue(x_str, x);

                        res &= compare(
                                       fun.lhs->GetScalarResult(),
                                       fun.rhs->GetScalarResult(),
                                       fun.op);
                        if(!res)
                            break;
                    }
                    if(res)
                    {
                        return true;
                    }
                }
            }
            return false;
        }


        static auto make_area(Bounds b, SpaceMap sm, std::vector<VtkFun>& vtk_vec)
        {
            // TODO have multiple threads that work on A / B / A / B sides of the matrix for faster operation
            std::vector<bool> vec(((b.max_x - b.min_x) / b.side) * ((b.max_y - b.min_y) / b.side), false);
            do_for(b, sm, vtk_vec, [&] (double x, double y) {
                vec[int((x - b.min_x)/b.side) + int((y - b.min_y)/b.side)] = true;
            });
            return vec;
        }


        static bool collides(const Bounds b,
                             const std::vector<bool>& a1,
                             const std::vector<bool>& a2,
                             const QTransform& t1,
                             const QTransform& t2 )
        {
            const int num = a1.size();
            #pragma omp parallel for
            for(int j = b.min_y; j < b.max_y; j += b.side)
            {
                double y = j;
                for(int i = b.min_x; i < b.max_x; i += b.side)
                {
                    double x = i;
                    QPointF p(x, y);
                    QPointF p1 = t1.map(p);
                    QPointF p2 = t2.map(p);

                    int x1 = (p1.x() - b.min_x) / b.side;
                    int x2 = (p2.x() - b.min_x) / b.side;
                    int y1 = (p1.y() - b.min_y) / b.side;
                    int y2 = (p2.y() - b.min_y) / b.side;

                    int s1 = x1 + y1;
                    if(s1 >= num)
                        continue;
                    int s2 = x2 + y2;
                    if(s2 >= num)
                        continue;
                    if(a1[s1] && a2[s2])
                    {
                        return true;
                    }
                }
            }
            return false;
        }


        static double centroid_distance(const Bounds b,
                             const std::vector<bool>& a1,
                             const std::vector<bool>& a2,
                             const QTransform& t1,
                             const QTransform& t2 )
        {
            const int num = a1.size();
            QPointF c1, c2;
            int n1 = 0, n2 = 0;
            #pragma omp parallel reduction(+:c1,c2)
            for(int j = b.min_y; j < b.max_y; j += b.side)
            {
                double y = j;
                for(int i = b.min_x; i < b.max_x; i += b.side)
                {
                    double x = i;
                    QPointF p(x, y);
                    QPointF p1 = t1.map(p);
                    QPointF p2 = t2.map(p);

                    int x1 = (p1.x() - b.min_x) / b.side;
                    int x2 = (p2.x() - b.min_x) / b.side;
                    int y1 = (p1.y() - b.min_y) / b.side;
                    int y2 = (p2.y() - b.min_y) / b.side;

                    int s1 = x1 + y1;
                    if(s1 < num && a1[s1])
                    {
                        c1 += QPointF{x, y};
                        n1++;
                    }

                    int s2 = x2 + y2;
                    if(s2 < num && a2[s2])
                    {
                        c2 += QPointF{x, y};
                        n2++;
                    }
                }
            }

            return QVector2D{c2/n2 - c1/n1}.length();
        }

        static bool check_collision(Bounds b, SpaceMap sm,
                                    std::vector<VtkFun>& vec1,
                                    std::vector<VtkFun>& vec2,
                                    const QTransform& t1,
                                    const QTransform& t2)
        {
            auto a1 = make_area(b, sm, vec1);
            auto a2 = make_area(b, sm, vec2);

            return collides(b, a1, a2, t1, t2);
        }


        static double compute_distance(Bounds b, SpaceMap sm,
                                    std::vector<VtkFun>& vec1,
                                    std::vector<VtkFun>& vec2,
                                    const QTransform& t1,
                                    const QTransform& t2)
        {
            return centroid_distance(
                        b,
                        make_area(b, sm, vec1),
                        make_area(b, sm, vec2),
                        t1, t2);
        }
};
class DrawAreaComputer : public QObject
{
        Q_OBJECT

    public:
        DrawAreaComputer(const QStringList& formulas):
            vtk_vec{Computations::make_funs(formulas)}
        {
            m_cp_thread.start();

            this->moveToThread(&m_cp_thread);

            connect(this, &DrawAreaComputer::startRealCompute,
                    this, &DrawAreaComputer::computeArea_priv, Qt::QueuedConnection);
        }

        ~DrawAreaComputer()
        {
            m_cp_thread.exit();
        }


    signals:
        void ready(QPainterPath);
        void startRealCompute(Space::Bounds b, SpaceMap sm, ValMap vals);

    public slots:
        void computeArea(const Space::Bounds& b, const SpaceMap& sm, const ValMap& vals)
        {
            if(computing)
                return;

            computing = true;
            emit startRealCompute(b, sm, vals);
        }

    private slots:
        void computeArea_priv(Space::Bounds b, SpaceMap sm, ValMap vals)
        {
            if(sm.size() != 2)
            {
                return;
            }

            for(VtkFun& f : vtk_vec)
            {
                for(auto val : vals)
                {
                    f.lhs->SetScalarVariableValue(val.first.c_str(), val.second);
                    f.rhs->SetScalarVariableValue(val.first.c_str(), val.second);
                }
            }


            QPainterPath path;

            Computations::do_for(b, sm, vtk_vec, [&] (double x, double y) {
                path.addRect(x - b.side/2., y - b.side/2., b.side, b.side);
            });

            emit ready(path.simplified());

            computing = false;
        }


    private:
        std::vector<VtkFun> vtk_vec;
        QThread m_cp_thread;
        std::atomic_bool computing{false};
};


class CollisionAreaComputer : public QObject
{
        Q_OBJECT

        using pair_t = std::pair<
            QStringList,
            GiNaC::relational::operators
        >;
    public:
        CollisionAreaComputer()
        {
            m_cp_thread.start();

            this->moveToThread(&m_cp_thread);

            connect(this, &CollisionAreaComputer::startRealCompute,
                    this, &CollisionAreaComputer::computeArea_priv, Qt::QueuedConnection);
        }

        ~CollisionAreaComputer()
        {
            m_cp_thread.exit();
        }


    signals:
        void ready(bool, KeyPair<Id<AreaModel>>);
        void startRealCompute(
                Space::Bounds b,
                SpaceMap sm,
                std::vector<Space::VtkFun> funs,
                KeyPair<Id<AreaModel>>);

    public slots:
        void computeArea(
                const Space::Bounds& b,
                const SpaceMap& sm,
                std::vector<VtkFun> funs,
                KeyPair<Id<AreaModel>> keys)
        {
            if(computing)
                return;

            computing = true;
            emit startRealCompute(b, sm, funs, keys);
        }

    private slots:
        void computeArea_priv(
                Space::Bounds b,
                SpaceMap sm,
                std::vector<VtkFun> funs,
                KeyPair<Id<AreaModel>> keys)
        {
            if(sm.size() != 2)
            {
                return;
            }

            emit ready(Computations::find_true(b, sm, funs), keys);

            computing = false;
        }


    private:
        QThread m_cp_thread;
        std::atomic_bool computing{false};
};


class MatrixCollisionComputer : public QObject
{
        Q_OBJECT

        using pair_t = std::pair<
            QStringList,
            GiNaC::relational::operators
        >;
    public:
        MatrixCollisionComputer()
        {
            m_cp_thread.start();

            this->moveToThread(&m_cp_thread);

            connect(this, &MatrixCollisionComputer::startRealCompute,
                    this, &MatrixCollisionComputer::computeArea_priv, Qt::QueuedConnection);
        }

        ~MatrixCollisionComputer()
        {
            m_cp_thread.exit();
        }


    signals:
        void ready(bool, KeyPair<Id<AreaModel>>);
        void startRealCompute(
                Space::Bounds b,
                SpaceMap sm,
                std::vector<Space::VtkFun> funs1,
                std::vector<Space::VtkFun> funs2,
                QTransform t1,
                QTransform t2,
                KeyPair<Id<AreaModel>> keys
                );

    public slots:
        void computeArea(
                const Space::Bounds& b,
                const SpaceMap& sm,
                const std::vector<VtkFun>& funs1,
                const std::vector<VtkFun>& funs2,
                const QTransform& t1,
                const QTransform& t2,
                const KeyPair<Id<AreaModel>>& k
                )
        {
            if(computing)
                return;

            computing = true;
            emit startRealCompute(b, sm, funs1, funs2, t1, t2, k);
        }

    private slots:
        void computeArea_priv(
                Space::Bounds b,
                SpaceMap sm,
                std::vector<Space::VtkFun> funs1,
                std::vector<Space::VtkFun> funs2,
                QTransform t1,
                QTransform t2,
                KeyPair<Id<AreaModel>> keys
                )
        {
            if(sm.size() != 2)
            {
                return;
            }

            emit ready(Computations::check_collision(b, sm, funs1, funs2, t1, t2), keys);

            computing = false;
        }


    private:
        QThread m_cp_thread;
        std::atomic_bool computing{false};
};


class MatrixDistanceComputer : public QObject
{
        Q_OBJECT

        using pair_t = std::pair<
            QStringList,
            GiNaC::relational::operators
        >;
    public:
        MatrixDistanceComputer()
        {
            m_cp_thread.start();

            this->moveToThread(&m_cp_thread);

            connect(this, &MatrixDistanceComputer::startRealCompute,
                    this, &MatrixDistanceComputer::computeArea_priv, Qt::QueuedConnection);
        }

        ~MatrixDistanceComputer()
        {
            m_cp_thread.exit();
        }


    signals:
        void ready(double, KeyPair<Id<AreaModel>>);
        void startRealCompute(
                Space::Bounds b,
                SpaceMap sm,
                std::vector<Space::VtkFun> funs1,
                std::vector<Space::VtkFun> funs2,
                QTransform t1,
                QTransform t2,
                KeyPair<Id<AreaModel>>
                );

    public slots:
        void computeArea(
                const Space::Bounds& b,
                const SpaceMap& sm,
                const std::vector<Space::VtkFun>& funs1,
                const std::vector<Space::VtkFun>& funs2,
                const QTransform& t1,
                const QTransform& t2,
                const KeyPair<Id<AreaModel>>& k
                )
        {
            if(computing)
                return;

            computing = true;
            emit startRealCompute(b, sm, funs1, funs2, t1, t2, k);
        }

    private slots:
        void computeArea_priv(
                Space::Bounds b,
                SpaceMap sm,
                std::vector<VtkFun> funs1,
                std::vector<VtkFun> funs2,
                QTransform t1,
                QTransform t2,
                KeyPair<Id<AreaModel>> k
                )
        {
            if(sm.size() != 2)
            {
                return;
            }

            emit ready(Computations::compute_distance(b, sm, funs1, funs2, t1, t2), k);

            computing = false;
        }


    private:
        QThread m_cp_thread;
        std::atomic_bool computing{false};
};



class DistanceAreaComputer : public QObject
{
        Q_OBJECT

        using pair_t = std::pair<
            QStringList,
            GiNaC::relational::operators
        >;
    public:
        DistanceAreaComputer()
        {
            m_cp_thread.start();

            this->moveToThread(&m_cp_thread);

            connect(this, &DistanceAreaComputer::startRealCompute,
                    this, &DistanceAreaComputer::computeArea_priv, Qt::QueuedConnection);
        }

        ~DistanceAreaComputer()
        {
            m_cp_thread.exit();
        }


    signals:
        void ready(bool, KeyPair<Id<AreaModel>>);
        void startRealCompute(
                Space::Bounds b,
                SpaceMap sm,
                std::vector<Space::VtkFun> funs,
                KeyPair<Id<AreaModel>>);

    public slots:
        void computeArea(
                const Space::Bounds& b,
                const SpaceMap& sm,
                std::vector<VtkFun> funs,
                KeyPair<Id<AreaModel>> keys)
        {
            if(computing)
                return;

            computing = true;
            emit startRealCompute(b, sm, funs, keys);
        }

    private slots:
        void computeArea_priv(
                Space::Bounds b,
                SpaceMap sm,
                std::vector<VtkFun> funs,
                KeyPair<Id<AreaModel>> keys)
        {
            if(sm.size() != 2)
            {
                return;
            }

            emit ready(Computations::find_true(b, sm, funs), keys);

            computing = false;
        }


    private:
        QThread m_cp_thread;
        std::atomic_bool computing{false};
};
}

Q_DECLARE_METATYPE(std::vector<Space::VtkFun>)
Q_DECLARE_METATYPE(QPainterPath)
Q_DECLARE_METATYPE(Space::KeyPair<Id<Space::AreaModel>>)
