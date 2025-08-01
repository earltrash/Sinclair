#pragma once
#include "pch.h"
#include "Status.h"
#include "UIWindow.h"


// ���â �ʿ��Ѱ�.
// ���� ���� ���â�� ���� ���� �Ѱ��ֱ�.
// 1�� ���� 4��
		// 2�� ���� 5�� 
		// �̰� ���� �����ΰ� ��� ���ִ°� �ְ� 2�������� �������ִ°� �ְ� ���� window pos, slot pos, slot ���� �ְ� ���� slot�� ������ ���� �־���ϰ�
//		// �ݱ� ��ư ���� �־���ϰ� �巡�� ������ üũ�ϱ� ���� windowbar ���� �־����. 
//enum class Wearable_part //-> Wearable �з� �̰� �� 9���� ���� �� ������ ���Կ� type���� �־����. 
//{
//		Weapon,
//		Shoes,
//		EarRing,
//		Neckless,
//		Glove,
//		Under,
//		Upper,
//		Helmet,
//		Cape,
//		UnKnown
//};
class EquipmentWindow : public UIWindow
{
public:
		void Update() override;;
		void Render() override {};
		bool HandleMouseHover(Vec2 mousePos) override;
		UIWindowType GetType() override { return m_windowType; }
		void SetActivate(bool active) override { m_isActive = active; }

private:
		bool m_isActive = false;
		UIWindowType m_windowType = UIWindowType::EquipmentWindow;
};

