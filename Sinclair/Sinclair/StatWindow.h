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
		StatWindow() : UIWindow(UIWindowType::StatsWindow, { 50, 50 }, { 554, 766 })
		{
				AddComponent<UI_Renderer>();

				// ���� �ʱ�ȭ.
				m_fundamentalStats.power = 0;
				m_fundamentalStats.agile = 0;
				m_fundamentalStats.intelligence = 0;
				m_fundamentalStats.luck = 0;

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

		// ���콺 ��ǲ ���� �Լ���. �ٵ� �޼��� �Է� ó���� �ϰ� ������ �Ұ� ����. UIWindow���� ó����.
		bool HandleMouseDown(Vec2 mousePos) override;

		bool HandleMouseUp(Vec2 mousePos) override;

		// ȣ���� �� ����.
		bool HandleMouseHover(Vec2 mousePos) override { return true; }
		// ����Ŭ�� �ȹ�������.
		bool HandleDoubleClick(Vec2 mousePos) override { return true; }	
		bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;
		bool HandleMouseRight(Vec2 mousePos) override;

		// Ÿ�� üũ��
		UIWindowType GetType() override { return m_windowType; }

		void CalculateStats();
		void RenderBackground();
		void RenderTitleBar();
		void RenderCloseButton();
		void RenderStatsText();
		void RenderRadarChart();

		float NormalizeStat(float statValue, float min, float max);

private:
		fundamentalStatus m_fundamentalStats; // 1�� ������ ���� ����ü
		TotalStatus m_totalStats;						  // 2�� ������ ���� ����ü
		Vec2 m_statPoints[5];								  // ������ ������ ��ġ ����


		ComPtr<ID2D1Bitmap1> m_closeButton;
		ComPtr<ID2D1Bitmap1> m_backgroundBitmap;
		ComPtr<ID2D1Bitmap1> m_statBase;
};



