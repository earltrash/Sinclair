#pragma once

struct TotalStatus //2�� �������ͽ��� �ձ��� �빮�ڷ� ǥ�� , �÷��̾ ������ �� ����ü�� ->
{
		int Strength;
		int Magic_Power;
		int Health;
		int Knowledge;
		int Charm;
};

struct fundamentalStatus //��� �� ���� �ҹ��ڷ� ǥ�� , �������� ������ �� ����ü��
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


enum class Status_fundamental //�������� ���� �ִ� ����
{
		power,
		agile,
		intelligence,
		luck,

		Unknown
};


enum class Wearable_part //-> Wearable �з�
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

enum class Need_Moment //-> ���� �ʿ��ϳ�?
{
		Gen_2, //2���� 
		Gen_3, //3����
		Gen_4, //4���� �⺻
		//�ڿ������� ����� �ֵ� 

		//����ؼ� ����� �ֵ� 

		Syn, //�ռ� ��,
		Adv, //���� ��,

		//�� 
		Fam3_a, //2���� ���� ��, �� ��ġ�� ���� 3����� ���� �ֵ� 
		Fam4_a, //Ư�� �� ��ġ �޼� ��,

		Fam3_b, //2���� ���� ��, �� ��ġ�� ���� 3����� ���� �ֵ� 
		Fam4_b, //Ư�� �� ��ġ �޼� ��,
		UnKnown
};

//----------------------------------- ui ----------------------------------------
enum class UIWindowType
{
		SettingsWindow = 0,         // ����â
		InventoryWindow,            // �κ��丮
		InventoryTooltip,           // �κ��丮����
		EquipmentWindow,            // ���â
		StatsWindow,                // ����â
		EnhancementWindow,          // ��ȭâ
		SynthesisWindow,            // �ռ�â
		StatPotionUseWindow         // ���� ���� ���� ���â
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

enum class EnchancerType //�������� ������ �а� �ͼ� enum�� ó�� -> enum �а� Renderó��
{
		Default,
		Fail,
		Sucess,
		Nothing
};


enum class PotionLevel //������â Ȱ��ȭ ��Ű��, �ش� Ÿ������ �̹����� ��ȭ��Ŵ. 
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