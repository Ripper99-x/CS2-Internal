#include "CheatMenu.h"


VOID MenuESP()
{
    ImGui::PushItemWidth(220.0f);

    // Enable Box ESP
    ImGui::Text(("Box ESP"));
    ImGui::Separator();
    ImGui::Checkbox(("Enable"), &MenuConfig::ShowBoxESP);
    ImGui::SameLine(200);
    ImGui::Text(("Visible:"));
    ImGui::SameLine(270); 
    ImGui::ColorEdit4(("##BoxColorVisible"), reinterpret_cast<float*>(&MenuConfig::BoxColorVisible), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::SameLine(360); 
    ImGui::Text(("Not Visible:"));
    ImGui::SameLine(460); 
    ImGui::ColorEdit4(("##BoxColorNotVisible"), reinterpret_cast<float*>(&MenuConfig::BoxColorNotVisible), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Text(("Thickness:"));
    ImGui::SameLine(270); 
    ImGui::SliderFloat(("##BoxThickness"), &MenuConfig::BoxThickness, 0.5f, 2.0f, "%.2f");

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text(("Bone ESP"));
    ImGui::Separator();
    ImGui::Checkbox(("Enable"), &MenuConfig::ShowBoneESP);
    ImGui::SameLine(200);
    ImGui::Text(("Visible:"));
    ImGui::SameLine(270); 
    ImGui::ColorEdit4(("##BoneColorVisible"), reinterpret_cast<float*>(&MenuConfig::BoneColorVisible), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::SameLine(360); 
    ImGui::Text(("Not Visible:"));
    ImGui::SameLine(460); 
    ImGui::ColorEdit4(("##BoneColorNotVisible"), reinterpret_cast<float*>(&MenuConfig::BoneColorNotVisible), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Text(("Thickness:"));
    ImGui::SameLine(270); 
    ImGui::SliderFloat(("##BoneThickness"), &MenuConfig::BoneThickness, 0.5f, 2.0f, "%.2f");

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text(("Other ESP"));
    ImGui::Separator();
    ImGui::Checkbox(("Health ESP"), &MenuConfig::ShowHealthBar);
    ImGui::Checkbox(("Distance ESP"), &MenuConfig::ShowDistance);
    ImGui::SameLine(200);
    ImGui::ColorEdit4(("##DistanceColor"), reinterpret_cast<float*>(&MenuConfig::DistanceColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Checkbox(("Weapon ESP"), &MenuConfig::ShowWeaponESP);
    ImGui::SameLine(200);
    ImGui::ColorEdit4(("##WeaponColor"), reinterpret_cast<float*>(&MenuConfig::WeaponColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Checkbox(("Name ESP"), &MenuConfig::ShowPlayerName);
    ImGui::SameLine(200);
    ImGui::ColorEdit4(("##NameColor"), reinterpret_cast<float*>(&MenuConfig::NameColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Checkbox(("Item ESP"), &MenuConfig::ItemESP);
    ImGui::SameLine(200);
    ImGui::ColorEdit4(("##ItemColor"), reinterpret_cast<float*>(&MenuConfig::ItemColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

    ImGui::PopItemWidth();
}


VOID MenuAimBot()
{
    ImGui::PushItemWidth(220.0f);

    // Aimbot settings
    ImGui::Text(("Aimbot Settings"));
    ImGui::Separator();
    ImGui::Checkbox(("Enable Aimbot"), &MenuConfig::EnableAimbot);
    if (MenuConfig::EnableAimbot)
        MenuConfig::StandloneRCS = false;

    ImGui::Checkbox(("Standlone RCS"), &MenuConfig::StandloneRCS);
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SliderFloat("Vertical Recoil Control (RCS_Y)", &MenuConfig::RCS_Y, 0.0f, 5.0f, "%.2f");
    ImGui::SliderFloat("Horizontal Recoil Control (RCS_X)", &MenuConfig::RCS_X, 0.0f, 5.0f, "%.2f");

    // Bone selection
    ImGui::Text(("Bone Selection"));
    ImGui::Separator();
    if (ImGui::Combo(("Visible Bone"), &MenuConfig::CurrentBoneIndex, Bones, IM_ARRAYSIZE(Bones)))
    {
        BONEINDEX SelectedBoneIndex = BoneIndexMap[MenuConfig::CurrentBoneIndex];
        MenuConfig::SelectedBoneIndex = SelectedBoneIndex;
    }
    ImGui::Checkbox(("Aim Closest Visible Bone to FOV"), &MenuConfig::TargetClosestVisibleBoneToFov);
    ImGui::Spacing();
    ImGui::Spacing();

    // Aimbot parameters
    ImGui::Text(("Aimbot Parameters"));
    ImGui::Separator();
    ImGui::SliderFloat(("Smoothness"), &MenuConfig::AimSmoothness, 1.0f, 10.0f);
    ImGui::SliderFloat(("FOV"), &MenuConfig::FOV, 1.0f, 1000.0f);
    ImGui::Checkbox(("Draw FOV"), &MenuConfig::DrawFOV);
    ImGui::Spacing();
    ImGui::Spacing();

    // Hotkey selection
    ImGui::Text(("Hotkey"));
    ImGui::Separator();
    if (ImGui::Combo(("AimKey"), &CurrentHotkey, HotKeys, IM_ARRAYSIZE(HotKeys)))
        MenuConfig::SelectedHotkey = CurrentHotkey;


    ImGui::Spacing();
    ImGui::Spacing();

    // Silent Aim
    ImGui::Text(("Additional Settings"));
    ImGui::Separator();
    ImGui::Checkbox(("Silent Aim"), &MenuConfig::EnableSilentAim);

    ImGui::PopItemWidth();
}





VOID Miscellaneous()
{
    ImGui::Checkbox("Light Changer", &MenuConfig::LightChanger);

    if (MenuConfig::LightChanger)
    {
        ImGui::Begin("Miscellaneous Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::PushItemWidth(220.0f);

        // Section: Lighting Settings
        ImGui::Text("Lighting Settings:");
        ImGui::Separator();

        ImGui::Text("Color:");
        ImGui::SameLine();
        ImGui::ColorEdit4("##LightingColor", (float*)&MenuConfig::wModulation.LightingColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);

        ImGui::Text("Intensity:");
        ImGui::SameLine();
        ImGui::SliderFloat("##LightingIntensity", &MenuConfig::wModulation.LightingIntensity, -10.0f, 10.0f);

        ImGui::Spacing();
        ImGui::Separator();

        ImGui::Text("Skybox Color Settings:");
        ImGui::Separator();

        static float Hue = 0.0f;

        ImGui::SliderFloat("Hue", &Hue, 0.0f, 1.0f, "%.2f");

        ImVec4 RGBColor = ImColor::HSV(Hue, 1.0f, 1.0f); 

        MenuConfig::g_SkyboxColor.x = RGBColor.x;
        MenuConfig::g_SkyboxColor.y = RGBColor.y;
        MenuConfig::g_SkyboxColor.z = RGBColor.z;

        ImGui::PopItemWidth();
        ImGui::End();
    }

    ImGui::Text("Smoke Color Settings:");
    ImGui::Separator();

    ImGui::Checkbox("Enable Smoke Color", &MenuConfig::EnableSmokeColor);

    if (MenuConfig::EnableSmokeColor)
        ImGui::ColorEdit4("##SmokeColorPicker", (float*)&MenuConfig::SmokeColorPicker);


    ImGui::Text("TriggerBot Settings:");

    ImGui::Checkbox("Enable TriggerBot", &MenuConfig::TriggerBot);

    ImGui::SliderFloat("Distance Threshold", &MenuConfig::TriggerbotDistanceThreshold, 0.0f, 250.0f, "%.1f");
    ImGui::SliderFloat("Angle Tolerance", &MenuConfig::TriggerbotAngleTolerance, 0.0f, 150.0f, "%.1f");
    ImGui::Text("Leave them on default if you don't understand what they do:");


}

VOID __fastcall RenderUI()
{
    ImVec4 backgroundColor = ImVec4(0.05f, 0.1f, 0.15f, 1.0f);
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.3f, 0.2f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.3f, 0.2f, 1.0f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, backgroundColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, backgroundColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, backgroundColor);

    ImGui::SetNextWindowSize(ImVec2(650, 490));
    ImGui::SetNextWindowBgAlpha(0.90f);

    ImGuiWindowFlags WindowFlags =
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoResize;

    ImGui::PushFont(CustomFont);


    if (MenuConfig::ShowMenu)
    {

        ImVec2 MenuSize = ImVec2(1000, 1000);

        ImGui::Begin(("CS2 INTERNAL"), nullptr, WindowFlags);

        if (ImGui::Button(("ESP"), ImVec2(185, 0))) MenuConfig::MenuState = 1;
        ImGui::SameLine();
        if (ImGui::Button(("Aimbot"), ImVec2(185, 0))) MenuConfig::MenuState = 2;
        ImGui::SameLine();
        if (ImGui::Button(("Miscellaneous"), ImVec2(215, 0))) MenuConfig::MenuState = 3;

        switch (MenuConfig::MenuState)
        {
        case 1:
            ImGui::SetWindowSize(MenuSize);
            MenuESP();
            break;
        case 2:
            ImGui::SetWindowSize(MenuSize);
            MenuAimBot();
            break;
        case 3:
            ImGui::SetWindowSize(MenuSize);
            Miscellaneous();
            break;
        default:
            break;
        }

        ImGui::End();
    }

    ImGui::PopFont();
    ImGui::PopStyleColor(3);
}