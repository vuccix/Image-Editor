#pragma once

#include <Controller/Command.h>
#include <functional>

namespace Cmd {

    template <class State>
    class StatefulCommand final : public Command {
    public:
        using Function = std::move_only_function<void(EditorState&, State&)>;

        StatefulCommand(const CommandNames name, State state, Function execute, Function undo)
            : Command(name), m_state(std::move(state)), m_execute(std::move(execute)), m_undo(std::move(undo)) {}

        void execute(EditorState& state) override {
            m_execute(state, m_state);
        }

        void undo(EditorState& state) override {
            m_undo(state, m_state);
        }

    private:
        State    m_state;
        Function m_execute;
        Function m_undo;
    };

    // -----------------------------------------------------------------------------------------------------------------

    std::unique_ptr<Command> deleteLayer(size_t layerID);
    std::unique_ptr<Command> mergeWithLayerBelow(size_t topLayerID);

}
