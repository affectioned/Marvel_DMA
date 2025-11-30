#include "pch.h"
#include "Color Picker.h"

void ColorPicker::Render()
{
	ImGui::Begin("Color Picker");
	ImGui::ColorEdit4("Enemy Color", &EnemyColor.Value.x);
	ImGui::ColorEdit4("Friendly Color", &FriendlyColor.Value.x);
	ImGui::End();
}
