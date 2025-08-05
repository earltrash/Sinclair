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
		else if (val == "EarRing") return Wearable_part::EarRing;
		else if (val == "Neckless") return Wearable_part::Neckless;
		else if (val == "Glove") return Wearable_part::Glove;
		else if (val == "Under") return Wearable_part::Under;
		else if (val == "Upper") return Wearable_part::Upper;
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
