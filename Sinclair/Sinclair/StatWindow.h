#pragma once
#include "pch.h"
#include "Status.h"
#include "UIWindow.h"
#include "UIManager.h"
#include "SimpleMathHelper.h"
#include "Item.h"
#include "Wearable.h"
#include "EquipmentWindow.h"
#include "UI_Renderer.h"
#include "ResourceManager.h"

class StatWindow : public UIWindow
{
public:
		StatWindow() : UIWindow(UIWindowType::StatsWindow, { 50, 50 }, { 554, 766 })
		{
				AddComponent<UI_Renderer>();

				// 스탯 초기화.
				m_equipmentFundamentalStats.power = 0;
				m_equipmentFundamentalStats.agile = 0;
				m_equipmentFundamentalStats.intelligence = 0;
				m_equipmentFundamentalStats.luck = 0;

				m_totalStats.Strength = 0;
				m_totalStats.Knowledge = 0;
				m_totalStats.Health = 0;
				m_totalStats.Magic_Power = 0;
				m_totalStats.Charm = 0;

				m_statBase = ResourceManager::Get().Get_UIBank().Get_Image("StatBase").Get();
				m_backgroundBitmap = ResourceManager::Get().Get_UIBank().Get_Image("StatBG").Get();
				m_closeButton = ResourceManager::Get().Get_UIBank().Get_Image("CloseButton").Get();
		}
		virtual ~StatWindow() = default;

		void Update() override;
		void Render() override;

		// 마우스 인풋 관련 함수들. 근데 메세지 입력 처리만 하고 따른거 할거 없음. UIWindow에서 처리함.
		bool HandleMouseDown(Vec2 mousePos) override;

		bool HandleMouseUp(Vec2 mousePos) override;

		// 호버도 뭐 없음.
		bool HandleMouseHover(Vec2 mousePos) override { return true; }
		// 더블클릭 안받을거임.
		bool HandleDoubleClick(Vec2 mousePos) override { return true; }	
		bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;
		bool HandleMouseRight(Vec2 mousePos) override;

		// 타입 체크용
		UIWindowType GetType() override { return m_windowType; }

		//void CalculateStats();
		void RenderBackground();
		void RenderTitleBar();
		void RenderCloseButton();
		void RenderStatsText();
		void RenderRadarChart();
		// 함수 만들어주기.
		const fundamentalStatus& GetfundamentalStats() const { return m_equipmentFundamentalStats; }
		float NormalizeStat(float statValue, float min, float max);

		// 장비창 가져와서 스탯 전체 업데이트
		void UpdateTotalStats();

		// 특수무기 스탯 추가.
		void CalculateBonusStats(Item* item);
private:
		fundamentalStatus m_equipmentFundamentalStats;		// 장비 1차 스탯을 담을 구조체
		TotalStatus m_totalStats;						    // 최종 2차 스탯을 담을 구조체
		Vec2 m_statPoints[5];								// 오각형 꼭짓점 위치 저장


		ComPtr<ID2D1Bitmap1> m_closeButton;
		ComPtr<ID2D1Bitmap1> m_backgroundBitmap;
		ComPtr<ID2D1Bitmap1> m_statBase;
};



