#pragma once

#include <src/Bounds.hpp>
#include <src/Area/AreaParser.hpp>
#include <src/Area/ValMap.hpp>
#include <Space/area.hpp>
#include <Space/square_renderer.hpp>
#include <QThread>
#include <QMetaType>
#include <QPainterPath>
#include <vtkFunctionParser.h>
#include <iscore/tools/IdentifiedObject.hpp>
#include <vtkSmartPointer.h>
#include <QVector2D>
#include <boost/dynamic_bitset.hpp>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QFile>
namespace Space
{
// http://stackoverflow.com/a/16446040/1495627
class BitMatrix
{
    public:
        BitMatrix (int width, int height) :
            width{width},
            height{height},
            m_bits(width * height)
        {

        }

        const int width;
        const int height;

        int size() const
        { return width * height;}

        void set(int x, int y) {
            m_bits.set(y * width + x);
        }

        bool valid(int x, int y) const {
            return  x >= 0 && y >= 0 && x < width && y < height;
        }

        bool test(int x, int y) const {
            return m_bits.test(y * height + x);
        }
    private:
        boost::dynamic_bitset<> m_bits;
};

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
                auto s0 = form.first[0].toStdString();
                auto s1 = form.first[1].toStdString();
                f.lhs->SetFunction(s0.c_str());
                f.rhs->SetFunction(s1.c_str());

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
            ISCORE_ABORT;
        }

        template<typename Fun>
        static void do_for(Bounds b, SpaceMap sm, const std::vector<VtkFun>& vtk_vec, Fun f)
        {
            auto x_str = sm.begin()->toStdString().c_str();
            auto y_str = (++sm.begin())->toStdString().c_str();

            for(int j = b.min_y; j < b.max_y; j += b.side)
            {
                double y = j;
                for(const VtkFun& fun : vtk_vec)
                {
                    // TODO use indices instead
                    fun.lhs->SetScalarVariableValue(y_str, y);
                    fun.rhs->SetScalarVariableValue(y_str, y);
                }

                for(int i = b.min_x; i < b.max_x; i += b.side)
                {
                    double x = i;

                    bool res = true;
                    for(const VtkFun& fun : vtk_vec)
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

        static auto make_area(Bounds b, SpaceMap sm, const std::vector<VtkFun>& vtk_vec)
        {
            // TODO have multiple threads that work on A / B / A / B sides of the matrix for faster operation
            BitMatrix vec((b.max_x - b.min_x) / b.side, (b.max_y - b.min_y) / b.side);
            do_for(b, sm, vtk_vec, [&] (double x, double y) {
                vec.set((x - b.min_x)/b.side, (y - b.min_y)/b.side);
            });
            return vec;
        }

        static QImage debug_area(Bounds b, const BitMatrix& a1, const QTransform& t1)
        {
            QImage img(a1.width, a1.height, QImage::Format::Format_ARGB32);
            img.fill(Qt::transparent);
            const QColor col = qRgb(qrand() / 255, qrand() / 255, qrand() / 255);

            for(int j = b.min_y; j < b.max_y; j += b.side)
            {
                double y = j;
                for(int i = b.min_x; i < b.max_x; i += b.side)
                {
                    double x = i;
                    QPointF p(x, y);
                    QPointF p1 = t1.map(p);

                    int x1 = (p1.x() - b.min_x) / b.side;
                    int y1 = (p1.y() - b.min_y) / b.side;

                    if(a1.valid(x1, y1) && a1.test(x1, y1))
                    {
                        img.setPixel(x1, y1, col.rgba());
                    }
                }
            }

            return img;
        }

        static void debug_areas(const Bounds b,
                                const BitMatrix& a1,
                                const BitMatrix& a2,
                                const QTransform& t1,
                                const QTransform& t2)
        {
            QImage img1 = debug_area(b, a1, t1);
            QImage img2 = debug_area(b, a2, t2);
            QPixmap res(img1.size());

            {QPainter p(&res);
                p.drawImage(QPoint{}, img1);
                p.drawImage(QPoint{}, img2);
            }

            QImage final = res.toImage();
            static int img_num = 0;
            final.save("/tmp/isc/" + QString::number(img_num++) + ".png");
            qDebug("test");

        }

        static bool collides(const Bounds b,
                             const BitMatrix& a1,
                             const BitMatrix& a2,
                             const QTransform& t1,
                             const QTransform& t2 )
        {
            auto it1 = t1.inverted();
            auto it2 = t2.inverted();
            // #pragma omp parallel for
            for(int j = b.min_y; j < b.max_y; j += b.side)
            {
                double y = j;
                for(int i = b.min_x; i < b.max_x; i += b.side)
                {
                    double x = i;
                    // If any point in our space
                    // matches any point of the transformed spaces...
                    QPointF p(x, y);
                    QPointF p1 = it1.map(p);
                    QPointF p2 = it2.map(p);

                    int x1 = (p1.x() - b.min_x) / b.side;
                    int y1 = (p1.y() - b.min_y) / b.side;
                    int x2 = (p2.x() - b.min_x) / b.side;
                    int y2 = (p2.y() - b.min_y) / b.side;

                    if(a1.valid(x1, y1) &&
                       a2.valid(x2, y2) &&
                       a1.test(x1, y1) &&
                       a2.test(x2, y2))
                    {
                        return true;
                    }
                }
            }
            return false;
        }


        static double centroid_distance(const Bounds b,
                                        const BitMatrix& a1,
                                        const BitMatrix& a2,
                                        const QTransform& t1,
                                        const QTransform& t2 )
        {
            QPointF c1, c2;
            int n1 = 0, n2 = 0;
            // #pragma omp parallel reduction(+:c1,c2)
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

                    if(a1.valid(x1, y1) && a1.test(x1, y1))
                    {
                        c1 += QPointF{x, y};
                        n1++;
                    }

                    if(a2.valid(x2, y2) && a2.test(x2, y2))
                    {
                        c2 += QPointF{x, y};
                        n2++;
                    }
                }
            }

            return QVector2D{c2/double(n2) - c1/double(n1)}.length();
        }

        static bool check_collision(Bounds b,
                                    Space::SpaceMap sm_1,
                                    Space::SpaceMap sm_2,
                                    const std::vector<VtkFun>& vec1,
                                    const std::vector<VtkFun>& vec2,
                                    const QTransform& t1,
                                    const QTransform& t2)
        {
            auto a1 = make_area(b, sm_1, vec1);
            auto a2 = make_area(b, sm_2, vec2);

            return collides(b, a1, a2, t1, t2);
        }


        static double compute_distance(Bounds b,
                                       Space::SpaceMap sm_1,
                                       Space::SpaceMap sm_2,
                                       const std::vector<VtkFun>& vec1,
                                       const std::vector<VtkFun>& vec2,
                                       const QTransform& t1,
                                       const QTransform& t2)
        {
            return centroid_distance(
                        b,
                        make_area(b, sm_1, vec1),
                        make_area(b, sm_2, vec2),
                        t1, t2);
        }
};
}
