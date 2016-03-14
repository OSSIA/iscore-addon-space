#pragma once
#include <Space/area.hpp>
#include <QStringList>

namespace Space
{
class AreaParser
{
    public:
        AreaParser(const QStringList& str);

        bool check() const;
        std::unique_ptr<spacelib::area> result();


        // Maps to GiNaC::relational::operators

        static auto toOp(const QString& str)
        {
            thread_local const QStringList operator_map_rels{"==", "!=", "<", "<=", ">", ">="}; // In the order of GiNaC::relational::operators
            return static_cast<GiNaC::relational::operators>(operator_map_rels.indexOf(str));
        }

        static std::pair<QStringList, GiNaC::relational::operators> splitRelationship(const QString& eq)
        {
            thread_local const QStringList ordered_rels{"==", "!=", "<=", ">=", "<", ">"}; // Else parsing fails due to < matching before <=

            QString found_rel;
            QStringList res;

            for(const QString& rel : ordered_rels)
            {
                if(eq.contains(rel))
                {
                    res = eq.split(rel);
                    found_rel = rel;
                    break;
                }
            }

            if(res.size() == 2)
                return std::make_pair(res, toOp(found_rel));
            return {};
        }

        static auto splitRelationships(const QStringList& list)
        {
            std::vector<
                std::pair<
                    QStringList,
                    GiNaC::relational::operators
                >
            > splitted;

            splitted.reserve(list.size());
            for(auto str : list)
                splitted.push_back(splitRelationship(str));

            return splitted;
        }

        std::vector<
            std::pair<
                QStringList,
                GiNaC::relational::operators
            >
        > m_splitted;

};

// TODO AreaValidator for QLineEdit..
}
