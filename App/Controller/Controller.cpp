#include <Controller/Controller.h>

Controller::Controller() {
    setHistoryLength(m_historyLength);
}

void Controller::execute(EditorState& state, std::unique_ptr<Command> command) {
    command->execute(state);
    m_undoStack.emplace_back(std::move(command));
    m_redoStack.clear(); // clear redo on new action
}

void Controller::undo(EditorState& state) {
    if (m_undoStack.empty()) return;

    auto command = std::move(m_undoStack.back());
    m_undoStack.pop_back();

    command->undo(state);
    m_redoStack.emplace_back(std::move(command));
}

void Controller::redo(EditorState& state) {
    if (m_redoStack.empty()) return;

    auto command = std::move(m_redoStack.back());
    m_redoStack.pop_back();

    command->execute(state);
    m_undoStack.emplace_back(std::move(command));
}

void Controller::setHistoryLength(const size_t length) {
    m_undoStack.reserve(length);
    m_redoStack.reserve(length);
    m_historyLength = length;
}

size_t Controller::getHistoryLength() const noexcept {
    return m_historyLength;
}

bool Controller::hasUndo() const noexcept { return !m_undoStack.empty(); }
bool Controller::hasRedo() const noexcept { return !m_redoStack.empty(); }
