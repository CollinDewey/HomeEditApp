#include "widgets.h"

void ImGui::ColorPicker3ButtonSized(const char* label, const ImVec4& col, ImGuiColorEditFlags flags) {
	ImGui::PushID(label);
    
    if (ImGui::Button("Edit")) {
		ImGui::OpenPopup("##ColorPicker3ButtonSizedPopup");
    }

	ImGui::SameLine();
    ImGui::ColorEdit3(label, (float*)&col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker); //ColorEdit instead of ColorButton for Drag/Drop

	if (ImGui::BeginPopup("##ColorPicker3ButtonSizedPopup", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
		ImGui::SetNextItemWidth(212.0f); //Setting height through width
        ImGui::SetWindowPos(ImVec2(94.0f,240.0f)); //Move to the right side of the screen
		ImGui::ColorPicker3("Color", (float*)&col, flags | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoLabel);
        ImGui::EndPopup();
	}

    ImGui::PopID();
}