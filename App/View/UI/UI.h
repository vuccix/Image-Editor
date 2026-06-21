#pragma once

struct UI {
    void init();

    void beginFrame();
    void endFrame();

    void addFont(const auto& font, float sizePixels);

    void dockspace(auto&& callback);

    void menuBar(auto&& body);
    void menu(const char* name, auto&& body);
    void item(const char* label, auto&& callback);
    void item(const char* label, bool isSelected, auto&& callback);
    void item(const char* label, const char* shortcut, auto&& callback);

    void separator();
    void spacing();

private:
    bool m_isInit = false;
};

#include "UI.tpp"

extern UI ui;
