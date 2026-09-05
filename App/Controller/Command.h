#pragma once

#include "Commands/Names.h"
#include <string_view>
#include <memory>

struct EditorState;

class Command {
public:
    explicit Command(CommandNames name);
    virtual ~Command() = default;

    virtual void execute(EditorState& state) = 0;
    virtual void undo(EditorState& state)    = 0;

    std::string_view getName() const;

protected:
    CommandNames m_name;
};
