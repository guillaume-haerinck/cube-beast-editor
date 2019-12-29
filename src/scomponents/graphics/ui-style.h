#pragma once

#include <imgui/imgui.h>

class UIStyle {
public:
    UIStyle() {};

    const ImVec2& largeButtonSize() const { return m_largeButtonSize; }
    ImFont* fontIconLarge() const { return m_fontIconLarge; }
    const float largeButtonRounding() const { return m_largeButtonRounding; }

private:
    ImFont* m_fontIconLarge;
    ImVec2 m_largeButtonSize = ImVec2(50, 50);
    float m_largeButtonRounding = 10.0f;

private:
    friend class App;
};
