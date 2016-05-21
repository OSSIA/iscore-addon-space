#include "AreaParser.hpp"
#include <vtkFunctionParser.h>
namespace Space
{

AreaParser::AreaParser(const QStringList& list):
    m_splitted{splitRelationships(list)}
{
}

bool AreaParser::check() const
{
    return !m_splitted.empty() && !m_splitted.front().first.empty();
}

AreaParser::area AreaParser::result()
{
    area a;
    GiNaC::parser parser;
    for(const auto& elt : m_splitted)
    {
        const auto& str = elt.first;
        const auto& op = elt.second;
        GiNaC::ex lhs, rhs;
        if(!str.empty())
        {
            lhs = parser(str[0].toStdString());
            rhs = parser(str[1].toStdString());
        }

        a.rels.emplace_back(lhs, rhs, op);
    }

    // Get all the variables.
    for(const auto& sym : parser.get_syms())
    { a.syms.push_back(GiNaC::ex_to<GiNaC::symbol>(sym.second)); }

    return a;
}


}
