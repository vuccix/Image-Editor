#pragma once

#include <Model/EditorState.h>
#include <memory>
#include <string>

class Command {
public:
    virtual ~Command() = default;

    virtual void execute(EditorState& state) = 0;
    virtual void undo(EditorState& state)    = 0;

    virtual std::string getName() const      = 0;
};
