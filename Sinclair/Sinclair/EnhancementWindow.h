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

#include "PlayComponent.h"
#include "EffectComponent.h"

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
						btn->SetCurrentBitmap("normal");
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
				auto ray = ResourceManager::Get().GetTexture("portalRays");

				explodeTime = 0.3f;
				// 5�� ������Ʈ ����
				for (int i = 0; i < 5; i++)
				{
						m_sheetPosition[i] = { 85.f + (i * 60.f), 185.f };

						auto normalSheet = std::make_unique<Object>();
						auto normalInfo = normalSheet->GetRenderInfo();
						normalInfo->SetBitmap(sheet1.Get());
						normalSheet->GetTransform().SetPosition(Vec2(129 + (i * 60), 236));
						auto ray14normal = normalSheet->AddComponent<Rotate3D_Effect>(normalInfo, 0.f, ray->GetSize().width / 2.f, ray->GetSize().height / 2.f, 0.f, 0.f, 0.1f, ray.Get());
						auto ray24normal = normalSheet->AddComponent<Rotate3D_Effect>(normalInfo, 0.f, ray->GetSize().width / 2.f, ray->GetSize().height / 2.f, 0.f, 0.f, -0.1f, ResourceManager::Get().GetTexture("portalRays").Get());
						auto rayComposite = normalSheet->AddComponent<Composite_Effect>(normalInfo, ray14normal->GetEffect(), ray24normal->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
						auto rayExplode = normalSheet->AddComponent<Explode_Effect>(normalInfo, D2D1_POINT_2F{ ray->GetSize().width / 2.f, ray->GetSize().height / 2.f }, D2D1_VECTOR_2F{ 1.f, 1.f }, D2D1_VECTOR_2F{ 0.2f, 0.2f }, explodeTime, rayComposite->GetEffect());
						auto shadow4normal = normalSheet->AddComponent<Shadow_Effect>(normalInfo, 3.f, 211.f / 255.f, 211.f / 255.f, 211.f / 255.f, 1.f, sheet1.Get());
						auto shadowScale4normal = normalSheet->AddComponent<Scale_Effect>(normalInfo, sheet1->GetSize().width / 2.f, sheet1->GetSize().height / 2.f, 1.1f, 1.1f, shadow4normal->GetEffect());
						auto rayShdwComposite = normalSheet->AddComponent<Composite_Effect>(normalInfo, shadowScale4normal->GetEffect(), rayExplode->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
						auto normal = normalSheet->AddComponent<Composite_Effect>(normalInfo, sheet1.Get(), rayShdwComposite->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);

						//back->EffectPush("Normal", normal->GetEffect());
						m_normalImages.push_back(std::move(normalSheet));

						auto successSheet = std::make_unique<Object>();
						auto successInfo = successSheet->GetRenderInfo();
						successInfo->SetBitmap(sheet2.Get());
						successSheet->GetTransform().SetPosition(Vec2(129 + (i * 60), 236));
						auto shadow4success = successSheet->AddComponent<Shadow_Effect>(successInfo, 3.f, 1.f, 1.f, 1.f, 1.f, sheet2.Get());
						auto shadowScale4success = successSheet->AddComponent<Scale_Effect>(successInfo, sheet2->GetSize().width / 2.f, sheet2->GetSize().height / 2.f, 1.1f, 1.1f, shadow4success->GetEffect());
						auto successShadowComposite = successSheet->AddComponent<Composite_Effect>(successInfo, shadow4success->GetEffect(), shadowScale4success->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
						auto success = successSheet->AddComponent<Composite_Effect>(successInfo, sheet2.Get(), successShadowComposite->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);

						//back->EffectPush("Success", success->GetEffect());
						m_successImages.push_back(std::move(successSheet));

						auto failSheet = std::make_unique<Object>();
						auto failInfo = failSheet->GetRenderInfo();
						failInfo->SetBitmap(sheet3.Get());
						failSheet->GetTransform().SetPosition(Vec2(129 + (i * 60), 236));
						auto shadow4fail = failSheet->AddComponent<Shadow_Effect>(failInfo, 3.f, 1.f, 1.f, 1.f, 1.f, sheet3.Get());
						auto shadowScale4fail = failSheet->AddComponent<Scale_Effect>(failInfo, sheet3->GetSize().width / 2.f, sheet3->GetSize().height / 2.f, 1.1f, 1.1f, shadow4fail->GetEffect());
						auto failShadowComposite = failSheet->AddComponent<Composite_Effect>(failInfo, shadow4fail->GetEffect(), shadowScale4fail->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
						auto fail = failSheet->AddComponent<Composite_Effect>(failInfo, sheet3.Get(), failShadowComposite->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
						
						m_failImages.push_back(std::move(failSheet));
				}
				// ���� �õ� �ʱ�ȭ
				m_rng.seed(std::random_device{}());

				// ��� �ֹ��� ��ư �ʱ� opacity ����
				for (auto& btn : m_enhancementButtons)
				{
					auto btnComponent = btn->GetComponent<ButtonComponent>();
					if (btnComponent) btnComponent->SetOpacity(1.f);
				}

				// ��ư �ݹ� ����
				SetupButtonCallbacks();
		}

		virtual ~EnhancementWindow() = default;

		void Update() override;
		void FixedUpdate(float dt) override;
		float Timer(float dt);
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

		// ��ũ�� ��ư ���� �� �ִ��� üũ�� -> ������Ʈ���� �� ��. ������ ����Ʈ Ȯ�ο�
		void Check4ItemEffect();
		
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
		void SetTargetItem(Item* item);

		Item* GetTargetItem() const { return m_targetItem; }

		// ��Ʈ �̹��� ������ ���� ������Ʈ
		void UpdateSheetVisibility();
		// ���� ������ ���� �ʱ�ȭ
		void InitializeItemObject();

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

		std::unique_ptr<Object> m_enhancementSlot;									 // ����.
		std::unique_ptr<Object> m_statSelectionButton;						    	 // �߾� ��ȭ��ư.
		std::unique_ptr<Object> m_leftArrowButton;								     // ���� ȭ��ǥ
		std::unique_ptr<Object> m_rightArrowButton;								     // ���� ȭ��ǥ
		std::vector<std::unique_ptr<Object>> m_enhancementButtons;					 // �ֹ��� ��ư 3��.
		//std::vector<std::unique_ptr<Object>> m_sheetImages;						 // ��ȭ ���� 5��. 
		std::vector<std::unique_ptr<Object>> m_normalImages;					     // ��ȭ ���� 5��. 
		std::vector<std::unique_ptr<Object>> m_successImages;					     // ��ȭ ���� 5��. 
		std::vector<std::unique_ptr<Object>> m_failImages;							 // ��ȭ ���� 5��. 
		std::unordered_map<int, D2D_VECTOR_2F> m_sheetPosition;

		std::unique_ptr<UI_Renderer> uirender = nullptr;
		Stat m_selectedStat = Stat::STR;

		// �������� ��Ʈ ����
		int m_renderSheetCount = 0;
		Item* m_targetItem = nullptr;
		float explodeTime = 0.f;	// explode effect play time

		// fixedUpdate���� ����Ʈ time check
		float time = 0.f;
		float x = 0.f;
		// ����
		std::mt19937 m_rng;

		// ���õ� �ֹ��� �ε���.
		size_t m_selectedScrollIndex = 0;
};

