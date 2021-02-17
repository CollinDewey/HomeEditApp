namespace ImGuiMod {
    IMGUI_API bool          ColorEditCustom3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
    IMGUI_API bool          ColorEditCustom4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
    IMGUI_API void          ColorTooltip(const char* text, const float* col, ImGuiColorEditFlags flags = 0);
    IMGUI_API bool          ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, ImVec2 size = ImVec2(0, 0)); // display a color square/button, hover for details, return true when pressed.
 }