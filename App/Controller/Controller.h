#pragma once

#include <Controller/Command.h>
#include <deque>

class Controller {
public:
    Controller();

    void execute(EditorState& state, std::unique_ptr<Command> command);

    void undo(EditorState& state);
    void redo(EditorState& state);

    void jumpToHistoryIndex(EditorState& state, size_t target);

    void setHistoryLength(size_t length);
    size_t getHistoryLength() const noexcept;

    bool hasUndo() const noexcept;
    bool hasRedo() const noexcept;

    using CommandUPtr = std::unique_ptr<Command>;

    const std::deque<CommandUPtr>& getHistory() const noexcept;
    size_t getCurrentIndex() const noexcept;

private:
    std::deque<CommandUPtr> m_history;
    size_t                  m_currentIndex  = 0;
    size_t                  m_historyLength = 16;
};
