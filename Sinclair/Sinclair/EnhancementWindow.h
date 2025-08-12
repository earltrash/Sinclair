#pragma once
#include "pch.h"
#include "Status.h"
#include "UIWindow.h"
#include "UIManager.h"
#include "SimpleMathHelper.h"
#include "Item.h"
#include "Inventory.h"
#include "UI_Renderer.h"
#include "ResourceManager.h"
#include "Object.h"
#include "BackgroundComponent.h"

#include <random>


class Object;
// �̰� �ݺ����ٰŴϱ�.
enum class Stat
{
		STR,
		INT,
		DEX,
		LUK
};

#define SlotSize 74.0f

enum class SheetImageType
{
		Normal,
		PartialEnhance,
		FullEnhance
};

class EnhancementWindow : public UIWindow
{
public:


		EnhancementWindow() : UIWindow(UIWindowType::EnhancementWindow, { 0, 0 }, { 524, 766 })
		{
				UI_Renderer* uiRenderer = AddComponent<UI_Renderer>();
				// ������Ʈ ������. �׷��� vector�� 
				if (uiRenderer)
				{
						// ResourceManager�� �ش� Ű�� ��Ʈ���� �ε�
						uiRenderer->SetBitmap("enhancement_window_bg", ResourceManager::Get().GetTexture("enhancementBG"));
						uiRenderer->SetBitmap("close_button", ResourceManager::Get().GetTexture("CloseButton"));
						uiRenderer->SetBitmap("close_button_hover", ResourceManager::Get().GetTexture("CloseButton"));
				}

				// ��� ����.
				m_enhancementSlot = std::make_unique<Object>();
				m_enhancementSlot->SetPosition(Vec2(226, 117));
				auto slot = ResourceManager::Get().GetTexture("enhancementSlot");
				auto slotinfo = m_enhancementSlot->GetRenderInfo();
				slotinfo->SetBitmap(slot.Get());

				// �߾ӹ�ư.
				m_statSelectionButton = std::make_unique<Object>();
				slot = ResourceManager::Get().GetTexture("enhancementMiddle");
				slotinfo = m_statSelectionButton->GetRenderInfo();
				slotinfo->SetBitmap(slot.Get());
				m_statSelectionButton->AddComponent<ButtonComponent>(slotinfo);
				if (auto btn = m_statSelectionButton->GetComponent<ButtonComponent>()) 
				{
						btn->BitmapPush("normal", slot);
						btn->BitmapPush("hover", slot);
						btn->BitmapPush("pressed", slot);
						btn->BitmapPush("disabled", slot);
				}
				m_statSelectionButton->SetPosition(Vec2(83, 305));
				


				// ������ư.
				m_leftArrowButton = std::make_unique<Object>();
				slot = ResourceManager::Get().GetTexture("enhancementLeft");
				slotinfo = m_leftArrowButton->GetRenderInfo();
				slotinfo->SetBitmap(slot.Get());
				m_leftArrowButton->AddComponent<ButtonComponent>(slotinfo);
				if (auto btn = m_leftArrowButton->GetComponent<ButtonComponent>()) 
				{
						btn->BitmapPush("normal", slot);
						btn->BitmapPush("hover", slot);
						btn->BitmapPush("pressed", slot);
						btn->BitmapPush("disabled", slot);
				}
				m_leftArrowButton->SetPosition(Vec2(43, 305));
				

				// ������ư.
				m_rightArrowButton = std::make_unique<Object>();
				slot = ResourceManager::Get().GetTexture("enhancementRight");
				slotinfo = m_rightArrowButton->GetRenderInfo();
				slotinfo->SetBitmap(slot.Get());
				m_rightArrowButton->AddComponent<ButtonComponent>(slotinfo);
				if (auto btn = m_rightArrowButton->GetComponent<ButtonComponent>()) {
						btn->BitmapPush("normal", slot);
						btn->BitmapPush("hover", slot);
						btn->BitmapPush("pressed", slot);
						btn->BitmapPush("disabled", slot);
				}
				m_rightArrowButton->SetPosition(Vec2(454, 305));
				

				// �ֹ�����ư.
				auto btn1 = std::make_unique<Object>();
				slot = ResourceManager::Get().GetTexture("enhancement100");
				slotinfo = btn1->GetRenderInfo();
				slotinfo->SetBitmap(slot.Get());
				btn1->AddComponent<ButtonComponent>(slotinfo);
				//btn1->SetPosition(Vec2(98, 444));
				if (auto bc = btn1->GetComponent<ButtonComponent>()) {
						bc->BitmapPush("normal", slot);
						bc->BitmapPush("hover", slot);
						bc->BitmapPush("pressed", slot);
						bc->BitmapPush("disabled", slot);
				}
				btn1->GetTransform().SetPosition((Vec2(98, 444)));


				auto btn2 = std::make_unique<Object>();
				slot = ResourceManager::Get().GetTexture("enhancement40");
				slotinfo = btn2->GetRenderInfo();
				slotinfo->SetBitmap(slot.Get());
				btn2->AddComponent<ButtonComponent>(slotinfo);
				if (auto bc = btn2->GetComponent<ButtonComponent>()) {
						bc->BitmapPush("normal", slot);
						bc->BitmapPush("hover", slot);
						bc->BitmapPush("pressed", slot);
						bc->BitmapPush("disabled", slot);
				}
				btn2->SetPosition(Vec2(98, 518));
				

				auto btn5 = std::make_unique<Object>();
				slot = ResourceManager::Get().GetTexture("enhancement20");
				slotinfo = btn5->GetRenderInfo();
				slotinfo->SetBitmap(slot.Get());
				btn5->AddComponent<ButtonComponent>(slotinfo);
				if (auto bc = btn5->GetComponent<ButtonComponent>()) {
						bc->BitmapPush("normal", slot);
						bc->BitmapPush("hover", slot);
						bc->BitmapPush("pressed", slot);
						bc->BitmapPush("disabled", slot);
				}
				btn5->SetPosition(Vec2(98, 592));
				

				m_enhancementButtons.push_back(std::move(btn1));
				m_enhancementButtons.push_back(std::move(btn2));
				m_enhancementButtons.push_back(std::move(btn5));

				// �ؽ�ó �̸� �ε�
				auto sheet1 = ResourceManager::Get().GetTexture("enhancementSheetNormal");
				auto sheet2 = ResourceManager::Get().GetTexture("enhancementSheetSuccess");
				auto sheet3 = ResourceManager::Get().GetTexture("enhancementSheetFail");

				// 5�� ������Ʈ ����
				for (int i = 0; i < 5; i++)
				{
						auto ordersheet = std::make_unique<Object>();

						// �⺻ ���� ���� ����
						auto renderInfo = ordersheet->GetRenderInfo();
						renderInfo->SetBitmap(sheet1.Get());

						ordersheet->AddComponent<BackgroundComponent>(renderInfo);

						// X ��ǥ�� 60�� ���� (129, 189, 249, 309, 369)
						ordersheet->SetPosition(Vec2(129 + (i * 60), 236));

						// �ؽ�ó ���� (��� ������Ʈ ����)
						ordersheet->GetComponent<BackgroundComponent>()->BitmapPush("Normal", sheet1);
						ordersheet->GetComponent<BackgroundComponent>()->BitmapPush("Success", sheet2);
						ordersheet->GetComponent<BackgroundComponent>()->BitmapPush("Fail", sheet3);

						// ���Ϳ� �߰�
						m_sheetImages.push_back(std::move(ordersheet));
				}
				// ���� �õ� �ʱ�ȭ
				m_rng.seed(std::random_device{}());

				// ��� �ֹ��� ��ư �ʱ� opacity ����
				for (auto& btn : m_enhancementButtons)
				{
						auto btnComponent = btn->GetComponent<ButtonComponent>();
						if (btnComponent) btnComponent->SetOpacity(0.7f);
				}

				// ��ư �ݹ� ����
				SetupButtonCallbacks();
		}

		virtual ~EnhancementWindow() = default;

		void Update() override;
		void Render() override;

		// ���콺 ��ǲ ���� �Լ���.
		bool HandleMouseDown(Vec2 mousePos) override;

		bool HandleMouseUp(Vec2 mousePos) override;

		// ȣ���� ������ ���� �����ҵ�. ��Ȯ�� �𸣰���.
		bool HandleMouseHover(Vec2 mousePos) override;

		// ����Ŭ�� �ȹ�������.
		bool HandleDoubleClick(Vec2 mousePos) override { return true; }

		bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;

		bool HandleMouseRight(Vec2 mousePos) override;

		// Ÿ�� üũ��
		UIWindowType GetType() override { return m_windowType; }
		
		// ������
		void RenderBackground();
		void RenderCloseButton();

		// ��ȭ ����
		void RenderEnhancementSlot();

		// �¿��� ���� �־ str int luk dex �̰� text �ٲ������.
		void RenderStatSelectionButtons();

		// ���ȭ��� stat �� +n ������ �ؽ�Ʈ ���.
		void RenderStatText();

		// ��ȭ ��ư 3�� �׸���.
		void RenderScrollButtons();

		// ��ȭ ���� 5��. 
		void RenderSheetImages();

		// ��ȭ
		void TryEnhance(int successRate);
		void SetTargetItem(Item* item) { m_targetItem = item; }
		Item* GetTargetItem() const { return m_targetItem; }

		// ��Ʈ �̹��� ������ ���� ������Ʈ
		void UpdateSheetVisibility();

		// ���콺�� ȣ���������� üũ.
		bool IsMouseOverObject(const Vec2& mousePos, Object* obj) const;
		Vec2 GetObjectScreenPos(Object* obj) const;
		bool IsMouseOverButton(const Vec2& mousePos, Object* obj) const;

		// ��ư �ݹ� ����
		void SetupButtonCallbacks();
		void OnStatSelectionButtonClick();
		void OnLeftArrowClick();
		void OnRightArrowClick();
		void OnEnhancementButtonClick(size_t buttonIndex);

		// ���õ� �ֹ����� ���� ���� �� ��ȯ
		int GetSelectedStatValue() const;     

		// �ݱ� ��ư �� 
		void ReturnItemToInventory();

		// ȣ�� ��������.
		SynSlot SlotInit(Vec2 pos);
private:

		std::unique_ptr<Object> m_enhancementSlot;									     // ����.
		std::unique_ptr<Object> m_statSelectionButton;						    	 // �߾� ��ȭ��ư.
		std::unique_ptr<Object> m_leftArrowButton;								    	 // ���� ȭ��ǥ
		std::unique_ptr<Object> m_rightArrowButton;								    	 // ���� ȭ��ǥ
		std::vector<std::unique_ptr<Object>> m_enhancementButtons;       // �ֹ��� ��ư 3��.
		std::vector<std::unique_ptr<Object>> m_sheetImages;					     // ��ȭ ���� 5��. 

		std::unique_ptr<UI_Renderer> uirender = nullptr;
		Stat m_selectedStat = Stat::STR;

		// �������� ��Ʈ ����
		int m_renderSheetCount = 0;
		Item* m_targetItem = nullptr;

		// ����
		std::mt19937 m_rng;

		// ���õ� �ֹ��� �ε���.
		size_t m_selectedScrollIndex = 0;
};

