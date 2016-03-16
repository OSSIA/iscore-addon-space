#pragma once
#include <QObject>

namespace Space
{

enum class Tool
{
    Move, Rotate, Scale
};

class EditionSettings : public QObject
{
        Q_OBJECT
        Q_PROPERTY(Space::Tool tool READ tool WRITE setTool NOTIFY toolChanged)

    public:
        Tool tool() const;

        void setTool(Tool tool);

        void setDefault();
        void restoreTool();

    signals:
        void toolChanged(Tool tool);

    private:
        Tool m_tool{Tool::Move};
        Tool m_previousTool{Tool::Move};
};
}
