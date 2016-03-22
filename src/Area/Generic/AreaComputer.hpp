#pragma once
#include <src/Area/Generic/AreaAlgorithms.hpp>
#include <src/Area/ValMap.hpp>
#include <atomic>

namespace Space
{

template<typename T>
class ThreadedComputation
{
        T& m_obj;
    public:
        ThreadedComputation(T& t):
            m_obj{t}
        {
            m_cp_thread.start();

            QObject::connect(&m_cp_thread, &QThread::finished,
                    &t, &QObject::deleteLater,
                    Qt::QueuedConnection);

            m_obj.moveToThread(&m_cp_thread);
        }

        const QThread& getThread() const
        { return m_cp_thread; }

        template<typename... Args>
        void computeArea(Args&&... args)
        {
            if(computing)
                return;

            computing = true;
            m_obj.startRealCompute(std::forward<Args>(args)...);
        }

    protected:
        std::atomic_bool computing{false};

    private:
        QThread m_cp_thread;
};

class DrawAreaComputer :
        public QObject,
        public ThreadedComputation<DrawAreaComputer>
{
        Q_OBJECT

    public:
        DrawAreaComputer(const QStringList& formulas):
            QObject{},
            ThreadedComputation<DrawAreaComputer>{*this},
            vtk_vec{Computations::make_funs(formulas)}
        {
            connect(this, &DrawAreaComputer::startRealCompute,
                    this, &DrawAreaComputer::computeArea_priv, Qt::QueuedConnection);
        }

    signals:
        void ready(QPainterPath);
        void startRealCompute(Space::Bounds b, SpaceMap sm, ValMap vals);

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
                    auto v1 = val.first.toStdString();
                    f.lhs->SetScalarVariableValue(v1.c_str(), val.second);
                    f.rhs->SetScalarVariableValue(v1.c_str(), val.second);
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
};


class MatrixCollisionComputer :
        public QObject,
        public ThreadedComputation<MatrixCollisionComputer>
{
        Q_OBJECT

    public:
        MatrixCollisionComputer():
            QObject{},
            ThreadedComputation<MatrixCollisionComputer>{*this}
        {
            connect(this, &MatrixCollisionComputer::startRealCompute,
                    this, &MatrixCollisionComputer::computeArea_priv, Qt::QueuedConnection);
        }

    signals:
        void ready(bool);
        void startRealCompute(
                Space::Bounds b,
                Space::SpaceMap sm_1,
                Space::SpaceMap sm_2,
                std::vector<Space::VtkFun> funs1,
                std::vector<Space::VtkFun> funs2,
                QTransform t1,
                QTransform t2
                );

    private slots:
        void computeArea_priv(
                Space::Bounds b,
                Space::SpaceMap sm1,
                Space::SpaceMap sm2,
                std::vector<Space::VtkFun> funs1,
                std::vector<Space::VtkFun> funs2,
                QTransform t1,
                QTransform t2
                )
        {
            if(sm1.size() != 2)
            {
                return;
            }

            emit ready(Computations::check_collision(b, sm1, sm2, funs1, funs2, t1, t2));

            computing = false;
        }

};


class MatrixDistanceComputer :
        public QObject,
        public ThreadedComputation<MatrixDistanceComputer>
{
        Q_OBJECT

    public:
        MatrixDistanceComputer():
            QObject{},
            ThreadedComputation<MatrixDistanceComputer>{*this}
        {
            connect(this, &MatrixDistanceComputer::startRealCompute,
                    this, &MatrixDistanceComputer::computeArea_priv, Qt::QueuedConnection);
        }

    signals:
        void ready(double);
        void startRealCompute(
                Space::Bounds b,
                Space::SpaceMap sm_1,
                Space::SpaceMap sm_2,
                std::vector<Space::VtkFun> funs1,
                std::vector<Space::VtkFun> funs2,
                QTransform t1,
                QTransform t2
                );


    private slots:
        void computeArea_priv(
                Space::Bounds b,
                Space::SpaceMap sm1,
                Space::SpaceMap sm2,
                std::vector<VtkFun> funs1,
                std::vector<VtkFun> funs2,
                QTransform t1,
                QTransform t2
                )
        {
            if(sm1.size() != 2)
            {
                return;
            }

            emit ready(Computations::compute_distance(b, sm1, sm2, funs1, funs2, t1, t2));

            computing = false;
        }
};
}

Q_DECLARE_METATYPE(std::vector<Space::VtkFun>)
Q_DECLARE_METATYPE(QPainterPath)
Q_DECLARE_METATYPE(Space::KeyPair<Id<Space::AreaModel>>)
