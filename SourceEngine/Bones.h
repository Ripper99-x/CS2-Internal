#pragma once
#include "Windows.h"
#include "unordered_map"
#include "Utils/cVector.h"


enum BONEINDEX : DWORD
{
    Pelvis = 0,
    Spine_1 = 2,
    Spine_2 = 3,
    Spine_3 = 4,
    Neck_0 = 5,
    Head = 6,
    Arm_Upper_L = 8,
    Arm_Lower_L = 9,
    Hand_L = 10,
    Arm_Upper_R = 13,
    Arm_Lower_R = 14,
    Hand_R = 15,
    Leg_Upper_L = 22,
    Leg_Lower_L = 23,
    Ankle_L = 24,
    Leg_Upper_R = 25,
    Leg_Lower_R = 26,
    Ankle_R = 27,
};


struct BonePosition
{
    Vector_t Location;
    Vector2D_t ScreenPosition;
    bool WorldVisible;
    bool RayTraceVisible;
};

struct CBone
{
    std::vector<BonePosition> BonePositions;
};


struct CBoneData
{
    Vector_t Location;
    float Scale;
    char Padding[0x16];
};

namespace BoneJointList
{
    inline std::list<DWORD> Trunk = { Pelvis, Spine_1, Spine_2, Spine_3, Neck_0, Head };

    inline std::list<DWORD> LeftArm = { Spine_3, Arm_Upper_L, Arm_Lower_L, Hand_L };

    inline std::list<DWORD> RightArm = { Spine_3, Arm_Upper_R, Arm_Lower_R, Hand_R };

    inline std::list<DWORD> LeftLeg = { Pelvis, Leg_Upper_L, Leg_Lower_L, Ankle_L };

    inline std::list<DWORD> RightLeg = { Pelvis, Leg_Upper_R, Leg_Lower_R, Ankle_R };


    inline std::vector<std::list<DWORD>> List = { Trunk, LeftArm, RightArm, LeftLeg, RightLeg};
}
