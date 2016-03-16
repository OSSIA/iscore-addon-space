#include "EditionSettings.hpp"

namespace Space
{

Tool EditionSettings::tool() const
{
    return m_tool;
}

void EditionSettings::setTool(Tool tool)
{
    m_tool = tool;
    emit toolChanged(tool);
}

void EditionSettings::setDefault()
{
    setTool(Tool::Move);
}

void EditionSettings::restoreTool()
{
    setTool(m_previousTool);
}

}
