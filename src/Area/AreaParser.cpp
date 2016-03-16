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

std::unique_ptr<spacelib::area> AreaParser::result()
{
    std::vector<GiNaC::relational> rels;
    std::vector<GiNaC::symbol> syms;
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

        rels.emplace_back(lhs, rhs, op);
    }

    // Get all the variables.
    for(const auto& sym : parser.get_syms())
    { syms.push_back(GiNaC::ex_to<GiNaC::symbol>(sym.second)); }

    return std::make_unique<spacelib::area>(
                std::move(rels),
                syms);
}

QStringList AreaParser::getSymbols()
{
    QSet<QString> variables;
    auto p = vtkFunctionParser::New();
    for(const auto& side : m_splitted)
    {
        {
            p->SetFunction(side.first[0].toLatin1().constData());
            p->IsScalarResult();
            int n = p->GetNumberOfScalarVariables();
            for(int i = 0; i < n; ++i)
                variables.insert(p->GetScalarVariableName(i));
        }
        {
            p->SetFunction(side.first[1].toLatin1().constData());
            p->IsScalarResult();
            int n = p->GetNumberOfScalarVariables();
            for(int i = 0; i < n; ++i)
                variables.insert(p->GetScalarVariableName(i));
        }
    }
    p->Delete();
    return variables.toList();
}
}
