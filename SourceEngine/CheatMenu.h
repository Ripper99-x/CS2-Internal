#pragma once
#include "Bones.h"
#include "D3D.h"

inline int CurrentHotkey;

namespace MenuConfig
{
	inline bool ShowBoneESP = true;
	inline bool ShowBoxESP = true;
	inline bool ShowHealthBar = true;
	inline int SelectedHotkey;
	inline bool ShowWeaponESP = true;
	inline bool StandloneRCS = false;
	inline int SelectedBoneIndex;
	inline bool BunnyHop = false;
	inline bool AutoStrafe = false;
	inline bool EnableAimbot = false;
	inline bool EnableSilentAim = false;
	inline bool TargetClosestVisibleBoneToFov = false;
	inline bool ShowDistance = true;
	inline bool ItemESP = true;
	inline bool ShowPlayerName = true;
	inline bool DrawFOV = false;
	inline bool TriggerBot = false;
	inline ImColor BoneColorVisible = ImColor(255, 0, 0, 255);     // Red for visible
	inline ImColor BoneColorNotVisible = ImColor(0, 255, 0, 255); // Green for not visible
	inline ImColor BoxColorVisible = ImColor(255, 0, 0, 255);      // Red for visible
	inline ImColor BoxColorNotVisible = ImColor(0, 255, 0, 255);  // Green for not visible
	inline ImColor ItemColor = ImColor(255, 255, 255, 255);      // Cyan
	inline ImColor NameColor = ImColor(255, 255, 255, 255);       // Yellow
	inline ImColor WeaponColor = ImColor(255, 255, 255, 255);	   // White
	inline ImColor DistanceColor = ImColor(255, 255, 255, 255);     // Orange
	inline float BoxThickness = 1.55f;
	inline float BoneThickness = 1.55f;
	inline float AimSmoothness = 1.0f;
	inline float RCS_Y = 2.55f;
	inline float RCS_X = 2.40f;
	inline bool ClosestBone = false;
	inline int CurrentBoneIndex = 6;
	inline float FOV = 100.0f;;
	inline bool ShowMenu = true;
	inline int MenuState = 1;
	inline bool ShowTimer = true;
	inline float TriggerbotDistanceThreshold = 155.0f;
	inline float TriggerbotAngleTolerance = 55.0f;
	inline bool LightChanger = false;
	inline bool EnableSmokeColor = false;
	inline ImVec4 g_SkyboxColor = ImVec4(1.0f, 0.75f, 0.8f, 1.0f);
	inline Vector_t SmokeColorPicker = Vector_t(1.0f, 0.0f, 0.0f); // Default to red
	inline struct WorldModulation
	{
		ImVec4 LightingColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); 
		float LightingIntensity = 1.0f; 
	} wModulation;


}

inline const char* HotKeys[] =
{
		"Shift",   "Alt" ,
		"Left Mouse Button" ,   "Right Mouse Button" ,   "Middle Mouse Button" ,
		"Mouse Button 4" ,   "Mouse Button 5"
};

inline int HotKeyCodes[] =
{
	VK_SHIFT, // "Shift"
	VK_MENU,  // "Alt"
	VK_LBUTTON, // "Left Mouse Button"
	VK_RBUTTON, // "Right Mouse Button"
	VK_MBUTTON, // "Middle Mouse Button"
	VK_XBUTTON1, // "Mouse Button 4"
	VK_XBUTTON2, // "Mouse Button 5"
};


inline const char* Bones[] =
{
	"Head", // head = 6
	"Neck", // neck_0 = 5
	"Spine 1", // spine_1 = 4
	"Spine 2", // spine_2 = 2
	"Pelvis", // pelvis = 0
	"Left Upper Arm", // arm_upper_L = 8
	"Left Lower Arm", // arm_lower_L = 9
	"Left Hand", // hand_L = 10
	"Right Upper Arm", // arm_upper_R = 13
	"Right Lower Arm", // arm_lower_R = 14
	"Right Hand", // hand_R = 15
	"Left Upper Leg", // leg_upper_L = 22
	"Left Lower Leg", // leg_lower_L = 23
	"Left Ankle", // ankle_L = 24
	"Right Upper Leg", // leg_upper_R = 25
	"Right Lower Leg", // leg_lower_R = 26
	"Right Ankle" // ankle_R = 27
};

inline BONEINDEX BoneIndexMap[] =
{
	BONEINDEX::Head,        // 0 "Head" maps to BONEINDEX::Head (6)
	BONEINDEX::Neck_0,      // 1 "Neck" maps to BONEINDEX::Neck_0 (5)
	BONEINDEX::Spine_1,     // 2 "Spine 1" maps to BONEINDEX::Spine_1 (4)
	BONEINDEX::Spine_2,     // 3 "Spine 2" maps to BONEINDEX::Spine_2 (2)
	BONEINDEX::Pelvis,      // 4 "Pelvis" maps to BONEINDEX::Pelvis (0)
	BONEINDEX::Arm_Upper_L, // 5 "Left Upper Arm" maps to BONEINDEX::Arm_Upper_L (8)
	BONEINDEX::Arm_Lower_L, // 6 "Left Lower Arm" maps to BONEINDEX::Arm_Lower_L (9)
	BONEINDEX::Hand_L,      // 7 "Left Hand" maps to BONEINDEX::Hand_L (10)
	BONEINDEX::Arm_Upper_R, // 8 "Right Upper Arm" maps to BONEINDEX::Arm_Upper_R (13)
	BONEINDEX::Arm_Lower_R, // 9 "Right Lower Arm" maps to BONEINDEX::Arm_Lower_R (14)
	BONEINDEX::Hand_R,      // 10 "Right Hand" maps to BONEINDEX::Hand_R (15)
	BONEINDEX::Leg_Upper_L, // 11 "Left Upper Leg" maps to BONEINDEX::Leg_Upper_L (22)
	BONEINDEX::Leg_Lower_L, // 12 "Left Lower Leg" maps to BONEINDEX::Leg_Lower_L (23)
	BONEINDEX::Ankle_L,     // 13 "Left Ankle" maps to BONEINDEX::Ankle_L (24)
	BONEINDEX::Leg_Upper_R, // 14 "Right Upper Leg" maps to BONEINDEX::Leg_Upper_R (25)
	BONEINDEX::Leg_Lower_R, // 15 "Right Lower Leg" maps to BONEINDEX::Leg_Lower_R (26)
	BONEINDEX::Ankle_R      // 16 "Right Ankle" maps to BONEINDEX::Ankle_R (27)
};

VOID MenuESP();
VOID MenuAimBot();
VOID RenderUI();
