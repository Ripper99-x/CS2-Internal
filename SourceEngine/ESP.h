#pragma once
#include "CheatMenu.h"
#include "W2S.h"
#include "map"
#include "Utils/Interfaces.h"
#include "chrono"


Vector4D_t Get2DBox(Vector_t WorldPosition, const float Height);
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

	gunIcons[("P90")] = "P";
	gunIcons[("MP9")] = "O";
	gunIcons[("Mp5sd")] = "x";
	gunIcons[("M4a4")] = "M";
	gunIcons[("Knife")] = "]";
	gunIcons[("DEagle")] = "A";
	gunIcons[("Elite")] = "B";
	gunIcons[("FiveSeven")] = "C";
	gunIcons[("Glock")] = "D";
	gunIcons[("Revolver")] = "J";
	gunIcons[("HKP2000")] = "E";
	gunIcons[("P250")] = "F";
	gunIcons[("Tec9")] = "H";
	gunIcons[("Cz75a")] = "I";
	gunIcons[("MAC10")] = "K";
	gunIcons[("Ump45")] = "L";
	gunIcons[("Bizon")] = "M";
	gunIcons[("MP7")] = "N";
	gunIcons[("GalilAR")] = "Q";
	gunIcons[("Famas")] = "R";
	gunIcons[("M4A1")] = "S";
	gunIcons[("Aug")] = "U";
	gunIcons[("SG556")] = "V";
	gunIcons[("AK47")] = "W";
	gunIcons[("G3SG1")] = "X";
	gunIcons[("SCAR20")] = "Y";
	gunIcons[("AWP")] = "Z";
	gunIcons[("SSG08")] = "a";
	gunIcons[("XM1014")] = "b";
	gunIcons[("Sawedoff")] = "c";
	gunIcons[("Mag7")] = "d";
	gunIcons[("NOVA")] = "e";
	gunIcons[("Negev")] = "f";
	gunIcons[("M249")] = "g";
	gunIcons[("Taser")] = "h";
	gunIcons[("Flashbang")] = "i";
	gunIcons[("HEGrenade")] = "j";
	gunIcons[("SmokeGrenade")] = "k";
	gunIcons[("MolotovGrenade")] = "l";
	gunIcons[("DecoyGrenade")] = "m";
	gunIcons[("C4")] = "o";
	gunIcons[("CBaseAnimGraph")] = "r";
	gunIcons[("IncendiaryGrenade")] = "n";

	auto it = gunIcons.find(weapon);

	if (it != gunIcons.end())
		return it->second;

	return "";
}
