#include <Controller/Controller.h>
#include <Model/EditorState.h>
#include <iostream>
#include <cstdint>
#include <cassert>
#include <chrono>

Controller::Controller() {
    setHistoryLength(m_historyLength);
}

namespace {

void debugTime(const std::unique_ptr<Command>& command, auto&& func) {
    using clock      = std::chrono::steady_clock;
    const auto start = clock::now();
    const auto name  = command->getName();

    func();

    const auto end = clock::now();
    const auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << name << " took " << dur.count() << " ms\n";
}

#ifdef NDEBUG
    #define DEBUG_TIME(command, func) (func)()
#else
    #define DEBUG_TIME(command, func) debugTime(command, func)
#endif

}

void Controller::execute(EditorState& state, std::unique_ptr<Command> command) {
    DEBUG_TIME(command, [&] {
        if (m_currentIndex < m_history.size())
            m_history.erase(m_history.begin() + static_cast<int64_t>(m_currentIndex), m_history.end());

        command->execute(state);
        ++state.version;

        m_history.emplace_back(std::move(command));
        m_currentIndex = m_history.size();

        if (m_history.size() > m_historyLength) {
            m_history.pop_front();
            m_currentIndex = m_history.size();
        }
    });
}

void Controller::undo(EditorState& state) {
    assert(hasUndo());

    m_history[--m_currentIndex]->undo(state);
    ++state.version;
}

void Controller::redo(EditorState& state) {
    assert(hasRedo());

    DEBUG_TIME(m_history[m_currentIndex], [&] {
        m_history[m_currentIndex++]->execute(state);
        ++state.version;
    });
}

void Controller::jumpToHistoryIndex(EditorState& state, const size_t target) {
    assert(target > m_history.size());

    while (m_currentIndex < target)
        redo(state);

    while (m_currentIndex > target)
        undo(state);
}

void Controller::setHistoryLength(const size_t length) {
    m_history.resize(length);
    m_history.clear();
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
