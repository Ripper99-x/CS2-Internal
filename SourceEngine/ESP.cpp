#include "ESP.h"
#include "iEngineClient.h"


Vector4D_t Get2DBox(Vector_t WorldPosition, const float Height)
{
	Vector_t TopPos3D;

	TopPos3D = WorldPosition;
	TopPos3D.z += Height;

	Vector2D_t GroundPos2D, TopPos2D;
	if (!View.WorldToScreen(WorldPosition, GroundPos2D) || !View.WorldToScreen(TopPos3D, TopPos2D))
		return Vector4D_t{ 0.0f, 0.0f, 0.0f, 0.0f };

	if (TopPos2D.y < 0.0f || GroundPos2D.y > ImGui::GetIO().DisplaySize.y)
		return Vector4D_t{ 0.0f, 0.0f, 0.0f, 0.0f };


	Vector2D_t Size, Pos;
	Size.y = std::abs(TopPos2D.y - GroundPos2D.y);
	Size.x = Size.y * 0.60;

	Pos.x = GroundPos2D.x - Size.x / 2;
	Pos.y = TopPos2D.y;

	return Vector4D_t{ Pos.x, Pos.y, Size.x, Size.y };
}


inline void DrawCustomBox(const ImVec2& rect_min, const ImVec2& rect_max, const ImColor& boxColor, float borderThickness, float rounding = 5.0f)
{
	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

	ImVec2 shadowOffset(3.0f, 3.0f);
	ImVec2 shadow_rect_min(rect_min.x + shadowOffset.x, rect_min.y + shadowOffset.y);
	ImVec2 shadow_rect_max(rect_max.x + shadowOffset.x, rect_max.y + shadowOffset.y);
	ImColor shadowColor(0, 0, 0, 100);

	draw_list->AddRect(shadow_rect_min, shadow_rect_max, shadowColor, rounding, 0, borderThickness);

	draw_list->AddRect(rect_min, rect_max, boxColor, rounding, 0, borderThickness);

	float innerOffset = 1.5f;
	ImVec2 inner_rect_min(rect_min.x + innerOffset, rect_min.y + innerOffset);
	ImVec2 inner_rect_max(rect_max.x - innerOffset, rect_max.y - innerOffset);
	ImColor innerBorderColor(150, 150, 150, 200);  
	draw_list->AddRect(inner_rect_min, inner_rect_max, innerBorderColor, rounding, 0, borderThickness * 0.5f);

	ImColor accentColor(255, 255, 255, 150); 
	draw_list->AddLine(ImVec2(rect_min.x, rect_min.y), ImVec2(rect_max.x, rect_min.y), accentColor, borderThickness * 0.5f);

	draw_list->AddLine(ImVec2(rect_min.x, rect_max.y), ImVec2(rect_max.x, rect_max.y), accentColor, borderThickness * 0.5f);
}



inline void DrawHealthBar(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size, bool Horizontal)
{
	ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

	float Proportion = CurrentHealth / MaxHealth;

	ImColor FirstStageColor = ImColor(96, 246, 113, 220);
	ImColor SecondStageColor = ImColor(247, 214, 103, 220);
	ImColor ThirdStageColor = ImColor(255, 95, 95, 220);
	ImColor BackGroundColor = ImColor(90, 90, 90, 220);
	ImColor Color;
	if (Proportion > 0.5)
		Color = FirstStageColor;
	else if (Proportion > 0.25)
		Color = SecondStageColor;
	else
		Color = ThirdStageColor;

	DrawList->AddRectFilled(Pos, { Pos.x + Size.x, Pos.y + Size.y }, BackGroundColor);

	if (Horizontal)
		DrawList->AddRectFilled(Pos, { Pos.x + Size.x * Proportion, Pos.y + Size.y }, Color);

	else
	{
		float healthHeight = Size.y * Proportion;
		DrawList->AddRectFilled({ Pos.x, Pos.y + Size.y - healthHeight }, { Pos.x + Size.x, Pos.y + Size.y }, Color);
	}

	ImColor BorderColor = ImColor(45, 45, 45, 220);
	DrawList->AddRect(Pos, { Pos.x + Size.x, Pos.y + Size.y }, BorderColor);
}


inline void DrawBone(const CBone& BoneData, float Thickness)
{
	if (BoneData.BonePositions.empty())
		return;

	ImDrawList* DrawList = ImGui::GetBackgroundDrawList();

	for (const auto& BoneList : BoneJointList::List)
	{
		const BonePosition* Previous = nullptr;

		for (const auto& i : BoneList)
		{
			if (i >= BoneData.BonePositions.size())
				continue;

			const BonePosition& Current = BoneData.BonePositions[i];

			if (Previous != nullptr && Previous->WorldVisible && Current.WorldVisible && !Current.RayTraceVisible)
				DrawList->AddLine(ImVec2(Previous->ScreenPosition.x, Previous->ScreenPosition.y), ImVec2(Current.ScreenPosition.x, Current.ScreenPosition.y), MenuConfig::BoneColorNotVisible, Thickness);

			else if (Previous != nullptr && Previous->WorldVisible && Current.WorldVisible && Current.RayTraceVisible)
				DrawList->AddLine(ImVec2(Previous->ScreenPosition.x, Previous->ScreenPosition.y), ImVec2(Current.ScreenPosition.x, Current.ScreenPosition.y), MenuConfig::BoneColorVisible, Thickness);

			Previous = &Current;
		}
	}
}


VOID __fastcall RenderESP()
{
	ImGui::PushFont(CustomFont);

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame())
		return;

	for (const auto& Entity : *CurrentPlayerList)
	{
		ImVec2 Rect_Min = ImVec2(Entity.Pawn.Rectangle.x, Entity.Pawn.Rectangle.y);
		ImVec2 Rect_Max = ImVec2(Entity.Pawn.Rectangle.x + Entity.Pawn.Rectangle.z, Entity.Pawn.Rectangle.y + Entity.Pawn.Rectangle.w);

		if (MenuConfig::ShowBoxESP)
			DrawCustomBox(Rect_Min, Rect_Max, Entity.Pawn.isVisible ? MenuConfig::BoxColorVisible : MenuConfig::BoxColorNotVisible, MenuConfig::BoxThickness);

		if (MenuConfig::ShowBoneESP)
			DrawBone(Entity.Pawn.BoneData, MenuConfig::BoneThickness);

		if (MenuConfig::ShowHealthBar)
		{
			ImVec2 HealthBarPos = { Entity.Pawn.Rectangle.x - 7.0f, Entity.Pawn.Rectangle.y };
			ImVec2 HealthBarSize = { 4.0f, Entity.Pawn.Rectangle.w };
			DrawHealthBar(100, Entity.Pawn.Health, HealthBarPos, HealthBarSize, false);
		}

		if (MenuConfig::ShowPlayerName)
		{
			ImVec2 TextSize = ImGui::CalcTextSize(Entity.Controller.m_sSanitizedPlayerName.c_str());
			ImVec2 TextPos = { Entity.Pawn.Rectangle.x + Entity.Pawn.Rectangle.z / 2 - TextSize.x / 2, Entity.Pawn.Rectangle.y - 22 };
			ImGui::GetForegroundDrawList()->AddText(TextPos, MenuConfig::NameColor, Entity.Controller.m_sSanitizedPlayerName.c_str());
		}

		if (MenuConfig::ShowDistance)
		{
			float DistanceToLocal = SDK::LocalPawn->GetOrigin().DistTo(Entity.Pawn.m_vOldOrigin) * 0.0254;
			int RoundedDistance = static_cast<int>(std::floor(DistanceToLocal));
			std::string DistanceText = std::to_string(RoundedDistance) + "m";
			ImVec2 DistanceTextSize = ImGui::CalcTextSize(DistanceText.c_str());
			ImVec2 DistanceTextPos = { Entity.Pawn.Rectangle.x + Entity.Pawn.Rectangle.z / 2 - DistanceTextSize.x / 2, Entity.Pawn.Rectangle.y - 35 };
			ImGui::GetForegroundDrawList()->AddText(DistanceTextPos, MenuConfig::DistanceColor, DistanceText.c_str());
		}

		if (MenuConfig::ShowWeaponESP)
		{
			ImGui::PushFont(WeaponFont);

			for (const auto& WeaponEntity : *CurrentWeaponList)
			{
				if (WeaponEntity.CSWeaponBase == Entity.Pawn.CSWeaponBase)
				{
					std::string WeaponIcon = GunIcon(WeaponEntity.Name);
					ImVec2 TextSize = ImGui::CalcTextSize(WeaponIcon.c_str());
					ImVec2 TextPosition = { Entity.Pawn.Rectangle.x + (Entity.Pawn.Rectangle.z - TextSize.x) * 0.5f, Entity.Pawn.Rectangle.y + Entity.Pawn.Rectangle.w };
					ImGui::GetForegroundDrawList()->AddText(TextPosition, MenuConfig::WeaponColor, WeaponIcon.c_str());
					break;
				}
			}

			ImGui::PopFont();
		}
	}

	for (const auto& Entity : *CurrentChickenList)
	{
		ImVec2 Rect_MinChicken = ImVec2(Entity.Chicken.Rectangle.x, Entity.Chicken.Rectangle.y);
		ImVec2 Rect_MaxChicken = ImVec2(Entity.Chicken.Rectangle.x + Entity.Chicken.Rectangle.z, Entity.Chicken.Rectangle.y + Entity.Chicken.Rectangle.w);
		DrawCustomBox(Rect_MinChicken, Rect_MaxChicken, Entity.Chicken.isVisible ? MenuConfig::BoxColorVisible : MenuConfig::BoxColorNotVisible, MenuConfig::BoxThickness);
		ImVec2 TextSize = ImGui::CalcTextSize(Entity.Chicken.Name.c_str());
		ImVec2 TextPos = { Entity.Chicken.Rectangle.x + Entity.Chicken.Rectangle.z / 2 - TextSize.x / 2, Entity.Chicken.Rectangle.y - 16 };
		ImGui::GetForegroundDrawList()->AddText(TextPos, MenuConfig::NameColor, Entity.Chicken.Name.c_str());
	}

	ImGui::PopFont();

	for (const auto& Entity : *CurrentWeaponList)
	{
		Vector2D_t ScreenPosition;

		if (!View.WorldToScreen(Entity.WorldPosition, ScreenPosition))
			continue;

		if (ScreenPosition.x < 0.0f || ScreenPosition.x > ImGui::GetIO().DisplaySize.x ||
			ScreenPosition.y < 0.0f || ScreenPosition.y > ImGui::GetIO().DisplaySize.y)
			continue;

		if (Entity.WorldPosition.x == 0 || Entity.WorldPosition.y == 0 || Entity.WorldPosition.z == 0)
			continue;

		if (Entity.Name == "CBaseAnimGraph" || Entity.Name == "C4")
		{
			ImGui::PushFont(DefuseKitFont);
			std::string WeaponIcon = GunIcon(Entity.Name);
			ImVec2 TextSize = ImGui::CalcTextSize(WeaponIcon.c_str());
			ImGui::GetForegroundDrawList()->AddText(ImVec2(ScreenPosition.x - TextSize.x * 0.5f, ScreenPosition.y - TextSize.y * 0.5f), MenuConfig::ItemColor, WeaponIcon.c_str());
			ImGui::PopFont();
		}
		else
		{
			ImGui::PushFont(WeaponFont);
			std::string WeaponIcon = GunIcon(Entity.Name);
			ImVec2 TextSize = ImGui::CalcTextSize(WeaponIcon.c_str());
			ImGui::GetForegroundDrawList()->AddText(ImVec2(ScreenPosition.x - TextSize.x * 0.5f, ScreenPosition.y - TextSize.y * 0.5f), MenuConfig::ItemColor, WeaponIcon.c_str());
			ImGui::PopFont();
		}
	}

	for (auto it = ActiveInfernoEntities.begin(); it != ActiveInfernoEntities.end();)
	{
		auto& [id, Entity] = *it;

		C_Inferno* Molotov = reinterpret_cast<C_Inferno*>(Entity.Inferno.Base);

		if (!Molotov || !Molotov->IsBurning()) {
			it = ActiveInfernoEntities.erase(it);
			continue;
		}

		auto Now = std::chrono::steady_clock::now();
		auto ElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(Now - Entity.StartTime).count();
		int TotalDuration = 7000;
		int RemainingTime = TotalDuration - ElapsedTime;

		if (RemainingTime <= 0)
		{
			it = ActiveInfernoEntities.erase(it);
			continue;
		}

		std::string WeaponIcon = GunIcon("MolotovGrenade");
		ImVec2 TextSize = ImGui::CalcTextSize(WeaponIcon.c_str());

		Vector2D_t ScreenPosition;

		if (!View.WorldToScreen(Entity.Inferno.WorldPosition, ScreenPosition)) {
			++it;
			continue;
		}

		if (ScreenPosition.x < 0.0f || ScreenPosition.x > ImGui::GetIO().DisplaySize.x ||
			ScreenPosition.y < 0.0f || ScreenPosition.y > ImGui::GetIO().DisplaySize.y) {
			++it;
			continue;
		}

		if (Entity.Inferno.WorldPosition.x == 0 || Entity.Inferno.WorldPosition.y == 0 || Entity.Inferno.WorldPosition.z == 0) {
			++it;
			continue;
		}

		ImGui::PushFont(ProjectileFont);
		ImGui::GetForegroundDrawList()->AddText(ImVec2(ScreenPosition.x - TextSize.x * 0.5f, ScreenPosition.y - TextSize.y * 0.5f), MenuConfig::WeaponColor, WeaponIcon.c_str());
		ImGui::PopFont();

		float BarWidth = TextSize.x * 20.0f;
		float Progress = static_cast<float>(RemainingTime) / static_cast<float>(TotalDuration);
		ImVec2 BarPos = ImVec2(ScreenPosition.x - BarWidth * 0.5f + TextSize.x * 0.2f, ScreenPosition.y - TextSize.y - 5.0f);
		ImVec2 BarEndPos = ImVec2(BarPos.x + BarWidth * Progress, BarPos.y + 5.0f);

		ImU32 BarColor = ImColor::HSV(Progress * 0.33f, 1.0f, 1.0f);

		ImGui::GetForegroundDrawList()->AddRectFilled(BarPos, BarEndPos, BarColor);
		ImGui::GetForegroundDrawList()->AddRect(BarPos, ImVec2(BarPos.x + BarWidth, BarEndPos.y), IM_COL32(255, 255, 255, 255));

		++it;
	}
}