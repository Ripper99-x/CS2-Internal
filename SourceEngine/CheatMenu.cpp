#include "CheatMenu.h"


VOID MenuESP()
{
    ImGui::PushItemWidth(220.0f);

    // Enable Box ESP
    ImGui::Text(xorstr_("Box ESP"));
    ImGui::Separator();
    ImGui::Checkbox(xorstr_("Enable"), &MenuConfig::ShowBoxESP);
    ImGui::SameLine(200);
    ImGui::Text(xorstr_("Visible:"));
    ImGui::SameLine(270); 
    ImGui::ColorEdit4(xorstr_("##BoxColorVisible"), reinterpret_cast<float*>(&MenuConfig::BoxColorVisible), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::SameLine(360); 
    ImGui::Text(xorstr_("Not Visible:"));
    ImGui::SameLine(460); 
    ImGui::ColorEdit4(xorstr_("##BoxColorNotVisible"), reinterpret_cast<float*>(&MenuConfig::BoxColorNotVisible), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Text(xorstr_("Thickness:"));
    ImGui::SameLine(270); 
    ImGui::SliderFloat(xorstr_("##BoxThickness"), &MenuConfig::BoxThickness, 0.5f, 2.0f, "%.2f");

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text(xorstr_("Bone ESP"));
    ImGui::Separator();
    ImGui::Checkbox(xorstr_("Enable"), &MenuConfig::ShowBoneESP);
    ImGui::SameLine(200);
    ImGui::Text(xorstr_("Visible:"));
    ImGui::SameLine(270); 
    ImGui::ColorEdit4(xorstr_("##BoneColorVisible"), reinterpret_cast<float*>(&MenuConfig::BoneColorVisible), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::SameLine(360); 
    ImGui::Text(xorstr_("Not Visible:"));
    ImGui::SameLine(460); 
    ImGui::ColorEdit4(xorstr_("##BoneColorNotVisible"), reinterpret_cast<float*>(&MenuConfig::BoneColorNotVisible), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Text(xorstr_("Thickness:"));
    ImGui::SameLine(270); 
    ImGui::SliderFloat(xorstr_("##BoneThickness"), &MenuConfig::BoneThickness, 0.5f, 2.0f, "%.2f");

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text(xorstr_("Other ESP"));
    ImGui::Separator();
    ImGui::Checkbox(xorstr_("Health ESP"), &MenuConfig::ShowHealthBar);
    ImGui::Checkbox(xorstr_("Distance ESP"), &MenuConfig::ShowDistance);
    ImGui::SameLine(200);
    ImGui::ColorEdit4(xorstr_("##DistanceColor"), reinterpret_cast<float*>(&MenuConfig::DistanceColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Checkbox(xorstr_("Weapon ESP"), &MenuConfig::ShowWeaponESP);
    ImGui::SameLine(200);
    ImGui::ColorEdit4(xorstr_("##WeaponColor"), reinterpret_cast<float*>(&MenuConfig::WeaponColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Checkbox(xorstr_("Name ESP"), &MenuConfig::ShowPlayerName);
    ImGui::SameLine(200);
    ImGui::ColorEdit4(xorstr_("##NameColor"), reinterpret_cast<float*>(&MenuConfig::NameColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::Checkbox(xorstr_("Item ESP"), &MenuConfig::ItemESP);
    ImGui::SameLine(200);
    ImGui::ColorEdit4(xorstr_("##ItemColor"), reinterpret_cast<float*>(&MenuConfig::ItemColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

    ImGui::PopItemWidth();
}


VOID MenuAimBot()
{
    ImGui::PushItemWidth(220.0f);

    // Aimbot settings
    ImGui::Text(xorstr_("Aimbot Settings"));
    ImGui::Separator();
    ImGui::Checkbox(xorstr_("Enable Aimbot"), &MenuConfig::EnableAimbot);
    if (MenuConfig::EnableAimbot)
        MenuConfig::StandloneRCS = false;

    ImGui::Checkbox(xorstr_("Standlone RCS"), &MenuConfig::StandloneRCS);
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SliderFloat("Vertical Recoil Control (RCS_Y)", &MenuConfig::RCS_Y, 0.0f, 5.0f, "%.2f");
    ImGui::SliderFloat("Horizontal Recoil Control (RCS_X)", &MenuConfig::RCS_X, 0.0f, 5.0f, "%.2f");

    // Bone selection
    ImGui::Text(xorstr_("Bone Selection"));
    ImGui::Separator();
    if (ImGui::Combo(xorstr_("Visible Bone"), &MenuConfig::CurrentBoneIndex, Bones, IM_ARRAYSIZE(Bones)))
    {
        BONEINDEX SelectedBoneIndex = BoneIndexMap[MenuConfig::CurrentBoneIndex];
        MenuConfig::SelectedBoneIndex = SelectedBoneIndex;
    }
    ImGui::Checkbox(xorstr_("Aim Closest Visible Bone to FOV"), &MenuConfig::TargetClosestVisibleBoneToFov);
    ImGui::Spacing();
    ImGui::Spacing();

    // Aimbot parameters
    ImGui::Text(xorstr_("Aimbot Parameters"));
    ImGui::Separator();
    ImGui::SliderFloat(xorstr_("Smoothness"), &MenuConfig::AimSmoothness, 1.0f, 10.0f);
    ImGui::SliderFloat(xorstr_("FOV"), &MenuConfig::FOV, 1.0f, 1000.0f);
    ImGui::Checkbox(xorstr_("Draw FOV"), &MenuConfig::DrawFOV);
    ImGui::Spacing();
    ImGui::Spacing();

    // Hotkey selection
    ImGui::Text(xorstr_("Hotkey"));
    ImGui::Separator();
    if (ImGui::Combo(xorstr_("AimKey"), &CurrentHotkey, HotKeys, IM_ARRAYSIZE(HotKeys)))
        MenuConfig::SelectedHotkey = CurrentHotkey;


    ImGui::Spacing();
    ImGui::Spacing();

    // Silent Aim
    ImGui::Text(xorstr_("Additional Settings"));
    ImGui::Separator();
    ImGui::Checkbox(xorstr_("Silent Aim"), &MenuConfig::EnableSilentAim);

    ImGui::PopItemWidth();
}



VOID Movement()
{
    ImGui::Checkbox(xorstr_("Bunnyhop"), &MenuConfig::BunnyHop);
    ImGui::Checkbox(xorstr_("Auto Strafe"), &MenuConfig::AutoStrafe);
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
        ImGui::Begin(xorstr_("CS2 INTERNAL"), nullptr, WindowFlags);

        if (ImGui::Button(xorstr_("ESP"), ImVec2(185, 0))) MenuConfig::MenuState = 1;
        ImGui::SameLine();
        if (ImGui::Button(xorstr_("Aimbot"), ImVec2(185, 0))) MenuConfig::MenuState = 2;
        ImGui::SameLine();
        if (ImGui::Button(xorstr_("Movement"), ImVec2(215, 0))) MenuConfig::MenuState = 3;

        ImVec2 menuSize = ImVec2(650, 5570);
        switch (MenuConfig::MenuState)
        {
        case 1:
            ImGui::SetWindowSize(menuSize);
            MenuESP();
            break;
        case 2:
            ImGui::SetWindowSize(menuSize);
            MenuAimBot();
            break;
        case 3:
            ImGui::SetWindowSize(menuSize);
            Movement();
            break;
        default:
            break;
        }

        ImGui::End();
    }

    ImGui::PopFont();
    ImGui::PopStyleColor(3);
}