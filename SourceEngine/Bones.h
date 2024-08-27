#pragma once
#include "Windows.h"
#include "unordered_map"
#include "Utils/cVector.h"


enum BONEINDEX : DWORD
{
    Head = 6,
    Neck_0 = 5,
    Spine_1 = 4,
    Spine_2 = 2,
    Pelvis = 0,
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


struct BoneJointData
{
    Vector_t Pos;
    char pad[0x14];
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
    char Padding[0x16];  // 0x10 (16 bytes) for a quaternion
};

namespace BoneJointList
{
    inline std::list<DWORD> Trunk = { Head, Neck_0, Spine_2, Pelvis };
    inline std::list<DWORD> LeftArm = { Neck_0, Arm_Upper_L, Arm_Lower_L, Hand_L };
    inline std::list<DWORD> RightArm = { Neck_0, Arm_Upper_R, Arm_Lower_R, Hand_R };
    inline std::list<DWORD> LeftLeg = { Pelvis, Leg_Upper_L, Leg_Lower_L, Ankle_L };
    inline std::list<DWORD> RightLeg = { Pelvis, Leg_Upper_R, Leg_Lower_R, Ankle_R };
    inline std::vector<std::list<DWORD>> List = { Trunk, LeftArm, RightArm, LeftLeg, RightLeg };

};
