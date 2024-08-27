#pragma once
#include "CheatMenu.h"
#include "W2S.h"
#include "map"
#include "Utils/Interfaces.h"


Vector4D_t Get2DBox(Vector_t WorldPosition, const float Height);
void DrawCustomBox(const ImVec2& rect_min, const ImVec2& rect_max, const ImColor& boxColor, float borderThickness, bool is3D);
void DrawHealthBar(float MaxHealth, float CurrentHealth, ImVec2 Pos, ImVec2 Size, bool Horizontal);
inline ImFont* CustomFontWeapon;
VOID __fastcall RenderESP();

inline const char* GunIcon(const std::string& weapon)
{
	std::map<std::string, const char*> gunIcons =
	{
		{"P90", "P"},
		{"MP9", "O"},
		{"Mp5sd", "x"},
		{"M4a4", "M"},
		{"Knife", "]"},
		{"DEagle", "A"},
		{"Elite", "B"},
		{"FiveSeven", "C"},
		{"Glock", "D"},
		{"Revolver", "J"},
		{"HKP2000", "E"},
		{"P250", "F"},
		{"Tec9", "H"},
		{"Cz75a", "I"},
		{"MAC10", "K"},
		{"Ump45", "L"},
		{"Bizon", "M"},
		{"MP7", "N"},
		{"GalilAR", "Q"},
		{"Famas", "R"},
		{"M4A1", "S"},
		{"Aug", "U"},
		{"SG556", "V"},
		{"AK47", "W"},
		{"G3SG1", "X"},
		{"SCAR20", "Y"},
		{"AWP", "Z"},
		{"SSG08", "a"},
		{"XM1014", "b"},
		{"Sawedoff", "c"},
		{"Mag7", "d"},
		{"NOVA", "e"},
		{"Negev", "f"},
		{"M249", "g"},
		{"Taser", "h"},
		{"Flashbang", "i"},
		{"HEGrenade", "j"},
		{"SmokeGrenade", "k"},
		{"MolotovGrenade", "l"},
		{"DecoyGrenade", "m"},
		{"C4", "o"},
		{"CBaseAnimGraph", "r"},
		{"IncendiaryGrenade", "n"}
	};

	gunIcons[xorstr_("P90")] = "P";
	gunIcons[xorstr_("MP9")] = "O";
	gunIcons[xorstr_("Mp5sd")] = "x";
	gunIcons[xorstr_("M4a4")] = "M";
	gunIcons[xorstr_("Knife")] = "]";
	gunIcons[xorstr_("DEagle")] = "A";
	gunIcons[xorstr_("Elite")] = "B";
	gunIcons[xorstr_("FiveSeven")] = "C";
	gunIcons[xorstr_("Glock")] = "D";
	gunIcons[xorstr_("Revolver")] = "J";
	gunIcons[xorstr_("HKP2000")] = "E";
	gunIcons[xorstr_("P250")] = "F";
	gunIcons[xorstr_("Tec9")] = "H";
	gunIcons[xorstr_("Cz75a")] = "I";
	gunIcons[xorstr_("MAC10")] = "K";
	gunIcons[xorstr_("Ump45")] = "L";
	gunIcons[xorstr_("Bizon")] = "M";
	gunIcons[xorstr_("MP7")] = "N";
	gunIcons[xorstr_("GalilAR")] = "Q";
	gunIcons[xorstr_("Famas")] = "R";
	gunIcons[xorstr_("M4A1")] = "S";
	gunIcons[xorstr_("Aug")] = "U";
	gunIcons[xorstr_("SG556")] = "V";
	gunIcons[xorstr_("AK47")] = "W";
	gunIcons[xorstr_("G3SG1")] = "X";
	gunIcons[xorstr_("SCAR20")] = "Y";
	gunIcons[xorstr_("AWP")] = "Z";
	gunIcons[xorstr_("SSG08")] = "a";
	gunIcons[xorstr_("XM1014")] = "b";
	gunIcons[xorstr_("Sawedoff")] = "c";
	gunIcons[xorstr_("Mag7")] = "d";
	gunIcons[xorstr_("NOVA")] = "e";
	gunIcons[xorstr_("Negev")] = "f";
	gunIcons[xorstr_("M249")] = "g";
	gunIcons[xorstr_("Taser")] = "h";
	gunIcons[xorstr_("Flashbang")] = "i";
	gunIcons[xorstr_("HEGrenade")] = "j";
	gunIcons[xorstr_("SmokeGrenade")] = "k";
	gunIcons[xorstr_("MolotovGrenade")] = "l";
	gunIcons[xorstr_("DecoyGrenade")] = "m";
	gunIcons[xorstr_("C4")] = "o";
	gunIcons[xorstr_("CBaseAnimGraph")] = "r";
	gunIcons[xorstr_("IncendiaryGrenade")] = "n";

	auto it = gunIcons.find(weapon);

	if (it != gunIcons.end())
		return it->second;

	return "";
}
