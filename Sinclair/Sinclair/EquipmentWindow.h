#pragma once
#include "pch.h"
#include "Status.h"
#include "UIWindow.h"


// 장비창 필요한거.
// 지금 현재 장비창의 총합 스탯 넘겨주기.
// 1차 스탯 4개
		// 2차 스탯 5개 
		// 이걸 전부 만들어두고 계산 해주는게 있고 2차스탯을 리턴해주는게 있고 현재 window pos, slot pos, slot 갯수 있고 각각 slot의 아이템 정보 있어야하고
//		// 닫기 버튼 영역 있어야하고 드래그 영역을 체크하기 위한 windowbar 영역 있어야함. 
//enum class Wearable_part //-> Wearable 분류 이거 즉 9개가 전부 다 각각의 슬롯에 type으로 있어야함. 
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

