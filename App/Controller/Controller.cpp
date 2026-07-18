#include <Controller/Controller.h>
#include <cstdint>

Controller::Controller() {
    setHistoryLength(m_historyLength);
}

void Controller::execute(EditorState& state, std::unique_ptr<Command> command) {
    if (m_currentIndex < m_history.size())
        m_history.erase(m_history.begin() + static_cast<int64_t>(m_currentIndex), m_history.end());

    command->execute(state);
    m_history.emplace_back(std::move(command));
    m_currentIndex = m_history.size();

    if (m_history.size() > m_historyLength) {
        m_history.pop_front();
        m_currentIndex = m_history.size();
    }
}

void Controller::undo(EditorState& state) {
    if (!hasUndo()) return;

    m_history[--m_currentIndex]->undo(state);
}

void Controller::redo(EditorState& state) {
    if (!hasRedo()) return;

    m_history[m_currentIndex++]->execute(state);
}

void Controller::jumpToHistoryIndex(EditorState& state, const size_t target) {
    if (target > m_history.size()) return;

    while (m_currentIndex < target)
        redo(state);

    while (m_currentIndex > target)
        undo(state);
}

void Controller::setHistoryLength(const size_t length) {
    m_history.clear();
    m_history.resize(length);
    m_historyLength = length;
}

size_t Controller::getHistoryLength() const noexcept {
    return m_historyLength;
}

bool Controller::hasUndo() const noexcept { return m_currentIndex > 0;                }
bool Controller::hasRedo() const noexcept { return m_currentIndex < m_history.size(); }

using CommandUPtr = std::unique_ptr<Command>;

const std::deque<CommandUPtr>& Controller::getHistory()      const noexcept { return m_history;      }
size_t                         Controller::getCurrentIndex() const noexcept { return m_currentIndex; }
