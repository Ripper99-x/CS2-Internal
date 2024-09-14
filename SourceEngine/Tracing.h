#pragma once
#include <array>
#include "cstdint"
#include "Utils/ReturnAddressr.h"
#include "Utils/cVector.h"
#include "Entity.h"
#include "cGameEntitySystem.h"
#include "Offsets.h"

#define CLIP_TRACE_TO_PLAYERS "48 8B C4 55 56 48 8D A8 58 FF FF FF 48 81 EC 98 01 00 00 48"
#define TRACE_SHAPE "48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 55 41 56 48 8D AC 24"
#define COMBINE(x, y) x##y
#define COMBINE2(x, y) COMBINE(x, y)


#define PAD_CLASS_DEBUG(sz) int COMBINE2(pad_, __COUNTER__)[sz];
#define CS2_PAD( number, size )                          \
private:                                                 \
    [[maybe_unused]] std::array< std::byte, size > m_unknown_##number{ }; \
public:
enum Contents_t
{
	CONTENTS_EMPTY = 0,
	CONTENTS_SOLID = 0x1,
	CONTENTS_WINDOW = 0x2,
	CONTENTS_AUX = 0x4,
	CONTENTS_GRATE = 0x8,
	CONTENTS_SLIME = 0x10,
	CONTENTS_WATER = 0x20,
	CONTENTS_BLOCKLOS = 0x40,
	CONTENTS_OPAQUE = 0x80,
	CONTENTS_TESTFOGVOLUME = 0x100,
	CONTENTS_UNUSED = 0x200,
	CONTENTS_BLOCKLIGHT = 0x400,
	CONTENTS_TEAM1 = 0x800,
	CONTENTS_TEAM2 = 0x1000,
	CONTENTS_IGNORE_NODRAW_OPAQUE = 0x2000,
	CONTENTS_MOVEABLE = 0x4000,
	CONTENTS_AREAPORTAL = 0x8000,
	CONTENTS_PLAYERCLIP = 0x10000,
	CONTENTS_MONSTERCLIP = 0x20000,
	CONTENTS_CURRENT_0 = 0x40000,
	CONTENTS_CURRENT_90 = 0x80000,
	CONTENTS_CURRENT_180 = 0x100000,
	CONTENTS_CURRENT_270 = 0x200000,
	CONTENTS_CURRENT_UP = 0x400000,
	CONTENTS_CURRENT_DOWN = 0x800000,
	CONTENTS_ORIGIN = 0x1000000,
	CONTENTS_MONSTER = 0x2000000,
	CONTENTS_DEBRIS = 0x4000000,
	CONTENTS_DETAIL = 0x8000000,
	CONTENTS_TRANSLUCENT = 0x10000000,
	CONTENTS_LADDER = 0x20000000,
	CONTENTS_HITBOX = 0x40000000,
};

enum Masks_t
{
	MASK_ALL = 0xFFFFFFFF,
	MASK_SOLID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
	MASK_PLAYERSOLID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
	MASK_NPCSOLID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
	MASK_NPCFLUID = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE,
	MASK_WATER = CONTENTS_WATER | CONTENTS_MOVEABLE | CONTENTS_SLIME,
	MASK_OPAQUE = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_OPAQUE,
	MASK_OPAQUE_AND_NPCS = MASK_OPAQUE | CONTENTS_MONSTER,
	MASK_BLOCKLOS = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_BLOCKLOS,
	MASK_BLOCKLOS_AND_NPCS = MASK_BLOCKLOS | CONTENTS_MONSTER,
	MASK_VISIBLE = MASK_OPAQUE | CONTENTS_IGNORE_NODRAW_OPAQUE,
	MASK_VISIBLE_AND_NPCS = MASK_OPAQUE_AND_NPCS | CONTENTS_IGNORE_NODRAW_OPAQUE,
	MASK_SHOT = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE | CONTENTS_HITBOX,
	MASK_SHOT_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_DEBRIS,
	MASK_SHOT_HULL = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE,
	MASK_SHOT_PORTAL = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER,
	MASK_SOLID_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_GRATE,
	MASK_PLAYERSOLID_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_PLAYERCLIP | CONTENTS_GRATE,
	MASK_NPCSOLID_BRUSHONLY = CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE,
	MASK_NPCWORLDSTATIC = CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE,
	MASK_NPCWORLDSTATIC_FLUID = CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP,
	MASK_SPLITAREPORTAL = CONTENTS_WATER | CONTENTS_SLIME,
	MASK_CURRENT = CONTENTS_CURRENT_0 | CONTENTS_CURRENT_90 | CONTENTS_CURRENT_180 | CONTENTS_CURRENT_270 | CONTENTS_CURRENT_UP | CONTENTS_CURRENT_DOWN,
	MASK_DEADSOLID = CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_GRATE,
};

enum
{
	SURF_LIGHT = 0x0001,
	SURF_SKY2D = 0x0002,
	SURF_SKY = 0x0004,
	SURF_WARP = 0x0008,
	SURF_TRANS = 0x0010,
	SURF_NOPORTAL = 0x0020,
	SURF_TRIGGER = 0x0040,
	SURF_NODRAW = 0x0080,
	SURF_HINT = 0x0100,
	SURF_SKIP = 0x0200,
	SURF_NOLIGHT = 0x0400,
	SURF_BUMPLIGHT = 0x0800,
	SURF_NOSHADOWS = 0x1000,
	SURF_NODECALS = 0x2000,
	SURF_NOPAINT = SURF_NODECALS,
	SURF_NOCHOP = 0x4000,
	SURF_HITBOX = 0x8000
};
struct Ray_t
{
	Vector_t Start = Vector_t(0, 0, 0);
	Vector_t End = Vector_t(0, 0, 0);
	Vector_t Mins = Vector_t(0, 0, 0);
	Vector_t Maxs = Vector_t(0, 0, 0);
	char __pad0000[0x4];
	std::uint8_t UnkownType = 0x0;
};
static_assert(sizeof(Ray_t) == 0x38);

class Trace_Filter_t
{
public:
	char __pad0000[0x8];
	std::uint64_t trace_mask;
	std::uint64_t null_it[2];
	std::uint32_t SkipHandles[4];
	std::uint16_t Collisions[2];
	std::uint16_t N0000011C;
	std::uint8_t layer;
	std::uint8_t N00000104;
	std::uint8_t null_it3;

};

struct C_TraceHitboxData
{
	CS2_PAD(0, 0x38);
	int m_hitgroup{ };
};

class Game_Trace_t
{
public:
	void* Surface;
	C_BaseEntity* HitEntity;
	C_TraceHitboxData* HitboxData;
	CS2_PAD(0, 0x38);
	std::uint32_t Contents;
	CS2_PAD(1, 0x24);
	Vector_t m_start_pos, m_end_pos, m_normal, m_pos;
	MEM_PAD(0x4);
	float Fraction;
	MEM_PAD(0x6);
	bool m_all_solid;
	CS2_PAD(4, 0x4D); 
};


struct UpdateValueT
{
	float previousLenght{ };
	float currentLenght{ };
	CS2_PAD(0, 0x8);
	std::int16_t handleIdx{ };
	CS2_PAD(1, 0x6);
};
struct trace_arr_element_t {
	CS2_PAD(0, 0x30);
};

struct Trace_Data_t
{
	std::int32_t m_uk1{ };
	float m_uk2{ 52.0f };
	void* m_arr_pointer{ };
	std::int32_t m_uk3{ 128 };
	std::int32_t m_uk4{ static_cast<std::int32_t>(0x80000000) };
	std::array< trace_arr_element_t, 0x80 > m_arr = { };
	CS2_PAD(0, 0x8);
	std::int64_t m_num_update{ };
	void* m_pointer_update_value{ };
	CS2_PAD(1, 0xC8);
	Vector_t m_start{ }, m_end{ };
	CS2_PAD(2, 0x50);
};

class Tracing
{
public:
	void InitializeTraceInfo(Game_Trace_t* const Hit)
	{
		using InitializeTraceInfoFunction = void(__fastcall*)(Game_Trace_t*);
		static InitializeTraceInfoFunction InitializeTraceInfoFn = nullptr;
		if (!InitializeTraceInfoFn)
			InitializeTraceInfoFn = reinterpret_cast<InitializeTraceInfoFunction>(M::FindPattern(CLIENT_DLL, "48 89 5C 24 08 57 48 83 EC 20 48 8B D9 33 FF 48 8B 0D"));

		FakeReturnAddress(Offsets->GameData.ReturnAddress, InitializeTraceInfoFn, Hit);
	}

	void InitializeTrace(Game_Trace_t& Trace)
	{
		using InitializeTraceFunction = void(__fastcall*)(Game_Trace_t&);
		static InitializeTraceFunction InitializeTraceFn = nullptr;

		if (!InitializeTraceFn)
			InitializeTraceFn = reinterpret_cast<InitializeTraceFunction>(M::FindPattern(CLIENT_DLL, "48 89 5C 24 08 57 48 83 EC 20 48 8B D9 33 FF 48 8B 0D"));

		FakeReturnAddress<void, Game_Trace_t&>(Offsets->GameData.ReturnAddress, InitializeTraceFn, Trace);
	}

	void Init(Trace_Filter_t& Filter, C_CSPlayerPawn* Skip, uint64_t Mask, uint8_t Layer, uint16_t Idk)
	{
		using InitTraceFilterFunction = Trace_Filter_t * (__fastcall*)(Trace_Filter_t&, void*, uint64_t, uint8_t, uint16_t);
		static InitTraceFilterFunction InitTraceFilterFn = nullptr;

		if (InitTraceFilterFn == nullptr)
			InitTraceFilterFn = reinterpret_cast<InitTraceFilterFunction>(M::FindPattern(CLIENT_DLL, ("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 0F B6 41 37 33")));

		FakeReturnAddress<Trace_Filter_t*, Trace_Filter_t&, void*, uint64_t, uint8_t, uint16_t>(Offsets->GameData.ReturnAddress, InitTraceFilterFn, Filter, Skip, Mask, Layer, Idk);
	}

	void ClipTraceToPlayers(Vector_t& Start, Vector_t& End, Trace_Filter_t* Filter, Game_Trace_t* Trace, float Min, int Length, float Max)
	{
		using ClipTraceToPlayersFunction = void(__fastcall*)(Vector_t&, Vector_t&, Trace_Filter_t*, Game_Trace_t*, float, int, float);
		static ClipTraceToPlayersFunction ClipTraceToPlayersFn = nullptr;

		if (ClipTraceToPlayersFn == nullptr)
		{
			ClipTraceToPlayersFn = reinterpret_cast<ClipTraceToPlayersFunction>(M::FindPattern(CLIENT_DLL, CLIP_TRACE_TO_PLAYERS));
			if (ClipTraceToPlayersFn == nullptr)
				ClipTraceToPlayersFn = reinterpret_cast<ClipTraceToPlayersFunction>(M::FindPattern(CLIENT_DLL, "48 8B C4 55 56 48 8D A8 58"));
		}

		FakeReturnAddress<void, Vector_t&, Vector_t&, Trace_Filter_t*, Game_Trace_t*, float, int, float>(Offsets->GameData.ReturnAddress, ClipTraceToPlayersFn, Start, End, Filter, Trace, Min, Length, Max);
	}

	static void GetTraceInfo(Trace_Data_t* Trace, Game_Trace_t* Hit, const float UnknownFloat, void* Unknown)
	{
		using GetTraceInfoFunction = void(__fastcall*)(Trace_Data_t*, Game_Trace_t*, float, void*);
		static GetTraceInfoFunction GetTraceInfoFn = nullptr;

		if (GetTraceInfoFn == nullptr)
			GetTraceInfoFn = reinterpret_cast<GetTraceInfoFunction>(M::FindPattern(CLIENT_DLL, ("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 60 48 8B E9 0F")));

		FakeReturnAddress<void, Trace_Data_t*, Game_Trace_t*, float, void*>(Offsets->GameData.ReturnAddress, GetTraceInfoFn, Trace, Hit, UnknownFloat, Unknown);
	}

	static bool HandleBulletPenetration(Trace_Data_t* const Trace, void* Stats, UpdateValueT* const ModValue, const bool draw_showimpacts = false)
	{
		using HandleBulletPenetrationFunction = bool(__fastcall*)(Trace_Data_t*, void*, UpdateValueT*, void*, void*, void*, void*, void*, bool);
		static HandleBulletPenetrationFunction HandleBulletPenetrationFn = nullptr;

		if (HandleBulletPenetrationFn == nullptr)
			HandleBulletPenetrationFn = reinterpret_cast<HandleBulletPenetrationFunction>(M::FindPattern(CLIENT_DLL, ("48 8B C4 44 89 48 20 55 57 41 55")));

		return FakeReturnAddress<bool, Trace_Data_t*, void*, UpdateValueT*, void*, void*, void*, void*, void*, bool>(Offsets->GameData.ReturnAddress, HandleBulletPenetrationFn, Trace, Stats, ModValue, nullptr, nullptr, nullptr, nullptr, nullptr, draw_showimpacts);
	}

	static void CreateTrace(Trace_Data_t* const Trace, const Vector_t Start, const Vector_t End, const Trace_Filter_t& Filter, const int PenetrationCount)
	{
		using CreateTraceFunction = void(__fastcall*)(Trace_Data_t*, Vector_t, Vector_t, Trace_Filter_t, int);
		static CreateTraceFunction CreateTraceFn = nullptr;

		if (CreateTraceFn == nullptr)
			CreateTraceFn = reinterpret_cast<CreateTraceFunction>(M::FindPattern(CLIENT_DLL, ("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 56 41 57 48 83 EC 40 F2")));

		FakeReturnAddress<void, Trace_Data_t*, Vector_t, Vector_t, Trace_Filter_t, int>(Offsets->GameData.ReturnAddress, CreateTraceFn, Trace, Start, End, Filter, PenetrationCount);
	}

	void TraceShape(Ray_t& Ray, Vector_t* Start, Vector_t* End, Trace_Filter_t Filter, Game_Trace_t& Trace)
	{
		using TraceShapeFunction = bool(__fastcall*)(void*, Ray_t&, Vector_t*, Vector_t*, Trace_Filter_t, Game_Trace_t&);
		static TraceShapeFunction TraceShapeFn = nullptr;

		if (TraceShapeFn == nullptr)
			TraceShapeFn = reinterpret_cast<TraceShapeFunction>(M::FindPattern(CLIENT_DLL, "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 48 89 4C 24 08 55 41 54 41 55 41 56 41 57 48 8D"));

		FakeReturnAddress<bool, void*, Ray_t&, Vector_t*, Vector_t*, Trace_Filter_t, Game_Trace_t&>(Offsets->GameData.ReturnAddress, TraceShapeFn, this, Ray, Start, End, Filter, Trace);
	}
};

