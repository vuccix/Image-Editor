#pragma once

#include <Controller/Command.h>
#include <vector>

class Controller {
public:
    Controller();

    void execute(EditorState& state, std::unique_ptr<Command> command);

    void undo(EditorState& state);
    void redo(EditorState& state);

    void setHistoryLength(size_t length);
    size_t getHistoryLength() const noexcept;

    bool hasUndo() const noexcept;
    bool hasRedo() const noexcept;

private:
    std::vector<std::unique_ptr<Command>> m_undoStack;
    std::vector<std::unique_ptr<Command>> m_redoStack;

    size_t m_historyLength = 16;
};
