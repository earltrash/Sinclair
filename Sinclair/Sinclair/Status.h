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
		Strength   ,
		Magic_Power,
		Health     ,
		Knowledge  ,
		Charm      ,
		/////////
		Null
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
		Ring,
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
		Fam3_a, //2세대 엔딩 후, 명성 수치에 따라서 3세대로 들어가는 애들 
		Fam4_a, //특정 명성 수치 달성 시,

		Fam3_b, //2세대 엔딩 후, 명성 수치에 따라서 3세대로 들어가는 애들 
		Fam4_b, //특정 명성 수치 달성 시,
		UnKnown
};

//----------------------------------- ui ----------------------------------------
enum class UIWindowType
{
		SettingsWindow = 0,         // 설정창
		InventoryWindow,            // 인벤토리
		InventoryTooltip,           // 인벤토리툴팁
		EquipmentWindow,            // 장비창
		StatsWindow,                // 스탯창
		EnhancementWindow,          // 강화창
		SynthesisWindow,            // 합성창
		StatPotionUseWindow         // 스탯 증가 물약 사용창
};

enum class CursorType
{
		Normal,
		Pressed,
		Move,
		Pointer,
		Drag
};

enum class DragSource
{
		Inventory,
		Equipment,
		Enhancement,
		Synthesis,
		None
};

enum class EnchancerType //아이템의 정보를 읽고 와서 enum값 처리 -> enum 읽고 Render처리
{
		Default,
		Fail,
		Sucess,
		Nothing
};


enum class PotionLevel //아이템창 활성화 시키고, 해당 타입으로 이미지를 변화시킴. 
{
		level1, //0
		level2, //1 
		level3, //2 
		nothing

};


struct Recipe {
	std::string mat1;
	std::string mat2;
	std::string result;
};


struct PoolCount {
	Need_Moment pool;
	int count;
};

enum class ES {
	Book,
	Fabric,
	Jewel,
	Mask,
	Metal1,
	Metal2,
	Potion,
	Stone,
	Wood,
	Nothing
};