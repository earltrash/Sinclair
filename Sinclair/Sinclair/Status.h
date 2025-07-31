#pragma once

struct TotalStatus //2차 스테이터스임 앞글자 대문자로 표기 , 플레이어가 가져야 할 구조체임 ->
{
		int Strength;
		int Magic_Power;
		int Health;
		int Knowledge;
		int Charm;
};

struct fundamentalStatus //얘는 앞 글자 소문자로 표기 , 아이템이 가져야 할 구조체임
{
		int power;
		int agile;
		int intelligence;
		int luck;
};



enum class Status_Total
{
		Strength,
		Magic_Power,
		Health,
		Knowledge,
		Charm
};


enum class Status_fundamental //아이템이 갖고 있는 스텟
{
		power,
		agile,
		intelligence,
		luck,

		Unknown
};


enum class Wearable_part //-> Wearable 분류
{
		Weapon,
		Shoes,
		EarRing,
		Neckless,
		Glove,
		Under,
		Upper,
		Helmet,
		Cape,
		UnKnown
};

enum class Need_Moment //-> 언제 필요하냐?
{
		Gen_2, //2세대 
		Gen_3, //3세대
		Gen_4, //4세대 기본
		//자연적으로 생기는 애들 

		//노력해서 생기는 애들 

		Syn, //합성 시,
		Adv, //모험 후,

		//명성 
		Fam3, //2세대 엔딩 후, 명성 수치에 따라서 3세대로 들어가는 애들 
		Fam4, //특정 명성 수치 달성 시,
		UnKnown
};





