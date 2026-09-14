#include <Controller/Controller.h>
#include <Model/EditorState.h>
#include <iostream>
#include <cstdint>
#include <cassert>
#include <chrono>

Controller::Controller() : m_curIndex(-1) {
    setHistoryLength(m_historyLength);
}

namespace {

using CommandUPtr = std::unique_ptr<Command>;

void debugTime(const ::CommandUPtr& command, auto&& func) {
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

void Controller::execute(EditorState& state, ::CommandUPtr command) {
    DEBUG_TIME(command, [&] {
        if (m_curIndex < static_cast<int64_t>(m_history.size()) - 1)
            m_history.erase(m_history.begin() + m_curIndex + 1, m_history.end());

        command->execute(state);
        ++state.version;

        m_history.emplace_back(std::move(command));

        if (m_history.size() > m_historyLength)
            m_history.pop_front();

        m_curIndex = m_history.size() - 1;
    });
}

void Controller::undo(EditorState& state) {
    assert(hasUndo());

    m_history[m_curIndex--]->undo(state);
    ++state.version;
}

void Controller::redo(EditorState& state) {
    assert(hasRedo());

    const int64_t nextIndex = m_curIndex + 1;

    DEBUG_TIME(m_history[nextIndex], [&] {
        m_history[nextIndex]->execute(state);
        m_curIndex = nextIndex;
        ++state.version;
    });
}

void Controller::jumpToHistoryIndex(EditorState& state, const size_t target) {
    assert(target < m_history.size());
    const auto t = static_cast<int64_t>(target);

    if (m_curIndex < t) {
        while (m_curIndex < t)
            redo(state);
    }
    else {
        while (m_curIndex > t)
            undo(state);
    }
}

void Controller::setHistoryLength(const size_t length) {
    m_history.clear();
    m_historyLength = length;
    m_curIndex      = -1;
}

size_t Controller::getHistoryLength() const noexcept {
    return m_historyLength;
}

bool Controller::hasUndo() const noexcept {
    return m_curIndex > 0;
}

bool Controller::hasRedo() const noexcept {
    return m_curIndex < static_cast<int64_t>(m_history.size()) - 1;
}

const std::deque<::CommandUPtr>& Controller::getHistory() const noexcept {
    return m_history;
}

int64_t Controller::getCurrentIndex() const noexcept {
    return m_curIndex;
}

#undef DEBUG_TIME
