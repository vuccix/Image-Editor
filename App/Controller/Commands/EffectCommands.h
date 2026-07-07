#pragma once

#include <Controller/Command.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FlipHorizontalCommand : public Command {
public:
    void execute(EditorState& state) override;
    void undo(EditorState& state) override;

    std::string getName() const override {
        return "Flip Horizontally";
    }

private:
    std::vector<Pixel> m_backup;
    size_t             m_layerID = SIZE_MAX;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FlipVerticalCommand : public Command {
public:
    void execute(EditorState& state) override;
    void undo(EditorState& state) override;

    std::string getName() const override {
        return "Flip Vertically";
    }

private:
    std::vector<Pixel> m_backup;
    size_t             m_layerID = SIZE_MAX;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class InvertCommand : public Command {
public:
    void execute(EditorState& state) override;
    void undo(EditorState& state) override;

    std::string getName() const override {
        return "Invert Colors";
    }

private:
    std::vector<Pixel> m_backup;
    size_t             m_layerID = SIZE_MAX;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class InvertAlphaCommand : public Command {
public:
    void execute(EditorState& state) override;
    void undo(EditorState& state) override;

    std::string getName() const override {
        return "Invert Alpha";
    }

private:
    std::vector<Pixel> m_backup;
    size_t             m_layerID = SIZE_MAX;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
