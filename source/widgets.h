#pragma once

#include "imgui/imgui.h"

namespace ImGui {
    void ColorPicker3ButtonSized(const char* label, const ImVec4& col, ImGuiColorEditFlags flags = ImGuiColorEditFlags_None);
}