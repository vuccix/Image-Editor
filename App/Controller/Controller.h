#pragma once

#include <Controller/Command.h>
#include <cstdint>
#include <deque>

class Controller {
public:
    using CommandUPtr = std::unique_ptr<Command>;

    Controller();

    void execute(EditorState& state, CommandUPtr command);

    void undo(EditorState& state);
    void redo(EditorState& state);

    void jumpToHistoryIndex(EditorState& state, size_t target);

    void setHistoryLength(size_t length);
    size_t getHistoryLength() const noexcept;

    bool hasUndo() const noexcept;
    bool hasRedo() const noexcept;

    const std::deque<CommandUPtr>& getHistory() const noexcept;
    int64_t getCurrentIndex() const noexcept;

private:
    std::deque<CommandUPtr> m_history;
    int64_t                 m_curIndex;
    size_t                  m_historyLength = 16;
};
