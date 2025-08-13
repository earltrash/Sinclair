#pragma once
#include "pch.h"
#include "ItemGlobal.h"

bool isDragging = false;
POINT dragStart = {};

Status_fundamental StringToF(std::string& val) //무기 스텟 
{
		if (val == "power") return Status_fundamental::power;
		else if (val == "agile") return Status_fundamental::agile;
		else if (val == "intelligence") return Status_fundamental::intelligence;
		else if (val == "luck") return Status_fundamental::luck;

		return Status_fundamental::Unknown;

}

Wearable_part StringToWP(std::string& val) //무기 ENUM 
{
		if (val == "Weapon") return Wearable_part::Weapon;
		else if (val == "Shoes") return Wearable_part::Shoes;
		else if (val == "Ring") return Wearable_part::Ring;
		else if (val == "Neckless") return Wearable_part::Neckless;
		else if (val == "Glove") return Wearable_part::Glove;
		else if (val == "Under") return Wearable_part::Under;
		else if (val == "Upper") return Wearable_part::Upper;
		else if (val == "Cape") return Wearable_part::Cape;
		else if (val == "Helmet") return Wearable_part::Helmet;
		else if (val == "Upper") return Wearable_part::Upper;
		return Wearable_part::UnKnown;
}

Need_Moment StringToNM(std::string& val)
{
		if (val == "Gen_2") return Need_Moment::Gen_2;
		else if (val == "Gen_3") return Need_Moment::Gen_3;
		else if (val == "Gen_4") return Need_Moment::Gen_4;
		else if (val == "Syn") return Need_Moment::Syn;
		else if (val == "Adv") return Need_Moment::Adv;
		else if (val == "Fam3_a") return Need_Moment::Fam3_a;
		else if (val == "Fam4_a") return Need_Moment::Fam4_a;
		else if (val == "Fam3_b") return Need_Moment::Fam3_b;
		else if (val == "Fam4_b") return Need_Moment::Fam4_b;
		return Need_Moment::UnKnown;

}


ES StringToES(std::string& val)
{
	if (val == "ES_Book") return ES::Book;
	else if (val == "ES_Fabric") return ES::Fabric;
	else if (val == "ES_Jewel") return ES::Jewel;
	else if (val == "ES_Mask") return ES::Mask;
	else if (val == "ES_Metal1") return ES::Metal1;
	else if (val == "ES_Metal2") return ES::Metal2;
	else if (val == "ES_Potion") return ES::Potion;
	else if (val == "ES_Stone") return ES::Stone;
	else if (val == "ES_Wood") return ES::Wood;
	return ES::Nothing;

}


 std::string ESToString(ES& val)
{
	if (val == ES::Book) return "Book";
	else if (val == ES::Fabric) return "Fabric";
	else if (val == ES::Jewel) return "Jewel";
	else if (val == ES::Mask) return "Mask";
	else if (val == ES::Metal1) return "Metal1";
	else if (val == ES::Metal2) return "Metal2";
	else if (val == ES::Potion) return "Potion";
	else if (val == ES::Stone) return "Stone";
	else if (val == ES::Wood) return "Wood";
	return "F";

}



std::wstring UTF8ToWstr(const std::string& str)
{
	if (str.empty()) return std::wstring();

	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
	return wstr;
}



std::wstring StrToWstr(const std::string& source) // 파싱 문자와 아닌 것을 구분하기 위함 
{

	if (source.empty()) return std::wstring();

	// CP_ACP: 시스템 기본 코드 페이지 (한국어 Windows = CP949)
	int size = MultiByteToWideChar(CP_ACP, 0, source.c_str(), -1, nullptr, 0);
	if (size <= 0) return std::wstring();

	std::wstring result(size - 1, 0);
	MultiByteToWideChar(CP_ACP, 0, source.c_str(), -1, &result[0], size);
	return result;

}