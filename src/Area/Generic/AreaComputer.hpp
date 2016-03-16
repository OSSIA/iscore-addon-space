#pragma once
#include <src/Bounds.hpp>
#include <src/Area/AreaParser.hpp>
#include <Space/area.hpp>
#include <Space/square_renderer.hpp>
#include <atomic>
#include <QThread>

#include <vtk/vtkFunctionParser.h>
#include <vtk/vtkSmartPointer.h>
namespace Space
{
class AreaComputer : public QObject
{
        Q_OBJECT

        enum RelOp { inf, inf_eq, sup, sup_eq, eq } ;
        struct LocalSpace
        {
                static constexpr int dimension() { return 2; }
                std::array<GiNaC::symbol, 2> arr;

                auto& variables() const { return arr; }
        };

        class LimitFilter : public QObject
        {
                const AreaComputer& m_area;
            public:
                LimitFilter(const AreaComputer& area, QObject* parent):
                    QObject{parent},
                    m_area{area}
                {
                }

                bool eventFilter(QObject *obj, QEvent *event)
                {
                    if(m_area.computing) {
                        return true;
                    }
                    return QObject::eventFilter(obj, event);
                }
        };

        struct VtkFun
        {
                vtkSmartPointer<vtkFunctionParser> lhs = vtkSmartPointer<vtkFunctionParser>::New();
                vtkSmartPointer<vtkFunctionParser> rhs = vtkSmartPointer<vtkFunctionParser>::New();
                GiNaC::relational::operators op{};
        };

        std::vector<VtkFun> vtk_vec;

        using pair_t = std::pair<
            QStringList,
            GiNaC::relational::operators
        >;
    public:
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

        AreaComputer(const QStringList& formulas):
            vtk_vec{make_funs(formulas)}
        {
            m_cp_thread.start();

            this->installEventFilter(new LimitFilter{*this, this});
            this->moveToThread(&m_cp_thread);
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

    signals:
        void ready(QVector<QRectF>);

    public slots:
        void computeArea(Space::Bounds b, SpaceMap sm, ValMap vals)
        {
            computing = true;
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


            QVector<QRectF> rects;
            rects.reserve(((b.max_x - b.min_x) / b.side) * ((b.max_y - b.min_y) / b.side));

            do_for(b, sm, vtk_vec, [&] (double x, double y) {
                rects.push_back(QRectF{x - b.side/2., y - b.side/2., b.side, b.side});
            });

            emit ready(rects);

            computing = false;
        }


    private:
        QThread m_cp_thread;
        std::atomic_bool computing{false};

};
}
