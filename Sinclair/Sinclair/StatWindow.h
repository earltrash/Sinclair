#pragma once
#include "pch.h"
#include "Status.h"
#include "UIWindow.h"
#include "UIManager.h"
#include "SimpleMathHelper.h"
#include "Item.h"
#include "EquipmentWindow.h"
#include "UI_Renderer.h"
#include "ResourceManager.h"

class StatWindow : public UIWindow
{
public:
		StatWindow(EquipmentWindow* equipmentWindow)
				: UIWindow(UIWindowType::StatsWindow, { 100, 100 }, { 554, 766 })
		{
				AddComponent<UI_Renderer>();

				// 스탯 초기화.
				m_fundamentalStats.power = 0;
				m_fundamentalStats.agile = 0;
				m_fundamentalStats.intelligence = 0;
				m_fundamentalStats.luck = 0;

				m_totalStats.Strength = 0;
				m_totalStats.Knowledge = 0;
				m_totalStats.Health = 0;
				m_totalStats.Magic_Power = 0;
				m_totalStats.Charm = 0;
		}
		virtual ~StatWindow() = default;

		void Update() override;
		void Render() override;

		// 마우스 인풋 관련 함수들.
		bool HandleMouseDown(Vec2 mousePos) override;

		bool HandleMouseUp(Vec2 mousePos) override;

		// 호버도 뭐 없음.
		bool HandleMouseHover(Vec2 mousePos) override { return true; }
		// 더블클릭 안받을거임.
		bool HandleDoubleClick(Vec2 mousePos) override { return true; }	

		// 타입 체크용
		UIWindowType GetType() override { return m_windowType; }

		void CalculateStats();
		void RenderBackground();
		void RenderTitleBar();
		void RenderCloseButton();
		void RenderStatsText();
		void RenderRadarChart();

		float NormalizeStat(float statValue, float min, float max);

private:
		fundamentalStatus m_fundamentalStats; // 1차 스탯을 담을 구조체
		TotalStatus m_totalStats;						  // 2차 스탯을 담을 구조체
		Vec2 m_statPoints[5];								  // 오각형 꼭짓점 위치 저장
};


