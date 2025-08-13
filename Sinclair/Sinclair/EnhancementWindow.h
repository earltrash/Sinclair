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
// 이거 반복해줄거니까.
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
				// 오브젝트 생성함. 그러고 vector에 
				if (uiRenderer)
				{
						// ResourceManager가 해당 키로 비트맵을 로드
						uiRenderer->SetBitmap("enhancement_window_bg", ResourceManager::Get().GetTexture("enhancementBG"));
						uiRenderer->SetBitmap("close_button", ResourceManager::Get().GetTexture("CloseButton"));
						uiRenderer->SetBitmap("close_button_hover", ResourceManager::Get().GetTexture("CloseButton"));
				}

				// 장비 슬롯.
				m_enhancementSlot = std::make_unique<Object>();
				m_enhancementSlot->SetPosition(Vec2(226, 117));
				auto slot = ResourceManager::Get().GetTexture("enhancementSlot");
				auto slotinfo = m_enhancementSlot->GetRenderInfo();
				slotinfo->SetBitmap(slot.Get());

				// 중앙버튼.
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
				


				// 좌측버튼.
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
				

				// 우측버튼.
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
				

				// 주문서버튼.
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

				// 텍스처 미리 로드
				auto sheet1 = ResourceManager::Get().GetTexture("enhancementSheetNormal");
				auto sheet2 = ResourceManager::Get().GetTexture("enhancementSheetSuccess");
				auto sheet3 = ResourceManager::Get().GetTexture("enhancementSheetFail");
				auto ray = ResourceManager::Get().GetTexture("portalRays");

				explodeTime = 0.3f;
				// 5개 오브젝트 생성
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
				// 랜덤 시드 초기화
				m_rng.seed(std::random_device{}());

				// 모든 주문서 버튼 초기 opacity 설정
				for (auto& btn : m_enhancementButtons)
				{
					auto btnComponent = btn->GetComponent<ButtonComponent>();
					if (btnComponent) btnComponent->SetOpacity(1.f);
				}

				// 버튼 콜백 설정
				SetupButtonCallbacks();
		}

		virtual ~EnhancementWindow() = default;

		void Update() override;
		void FixedUpdate(float dt) override;
		float Timer(float dt);
		void Render() override;

		// 마우스 인풋 관련 함수들.
		bool HandleMouseDown(Vec2 mousePos) override;

		bool HandleMouseUp(Vec2 mousePos) override;

		// 호버는 아이템 툴팁 떠야할듯. 정확히 모르겠음.
		bool HandleMouseHover(Vec2 mousePos) override;

		// 더블클릭 안받을거임.
		bool HandleDoubleClick(Vec2 mousePos) override { return true; }

		bool HandleDropFailure(Vec2 mousePos, Item* draggedItem, DragSource source) override;

		bool HandleMouseRight(Vec2 mousePos) override;

		// 타입 체크용
		UIWindowType GetType() override { return m_windowType; }

		// 스크롤 버튼 눌린 거 있는지 체크용 -> 업데이트에서 쓸 거. 아이템 이펙트 확인용
		void Check4ItemEffect();
		
		// 렌더용
		void RenderBackground();
		void RenderCloseButton();

		// 강화 슬롯
		void RenderEnhancementSlot();

		// 좌우측 끝에 있어서 str int luk dex 이걸 text 바꿔줘야함.
		void RenderStatSelectionButtons();

		// 배경화면과 stat 및 +n 나오는 텍스트 출력.
		void RenderStatText();

		// 강화 버튼 3개 그리기.
		void RenderScrollButtons();

		// 강화 유무 5개. 
		void RenderSheetImages();

		// 강화
		void TryEnhance(int successRate);
		void SetTargetItem(Item* item);

		Item* GetTargetItem() const { return m_targetItem; }

		// 시트 이미지 렌더링 개수 업데이트
		void UpdateSheetVisibility();
		// 들어온 아이템 설정 초기화
		void InitializeItemObject();

		// 마우스가 호버상태인지 체크.
		bool IsMouseOverObject(const Vec2& mousePos, Object* obj) const;
		Vec2 GetObjectScreenPos(Object* obj) const;
		bool IsMouseOverButton(const Vec2& mousePos, Object* obj) const;

		// 버튼 콜백 설정
		void SetupButtonCallbacks();
		void OnStatSelectionButtonClick();
		void OnLeftArrowClick();
		void OnRightArrowClick();
		void OnEnhancementButtonClick(size_t buttonIndex);

		// 선택된 주문서에 따른 스탯 값 반환
		int GetSelectedStatValue() const;     

		// 닫기 버튼 시 
		void ReturnItemToInventory();

		// 호버 띄울려고함.
		SynSlot SlotInit(Vec2 pos);
private:

		std::unique_ptr<Object> m_enhancementSlot;									 // 슬롯.
		std::unique_ptr<Object> m_statSelectionButton;						    	 // 중앙 강화버튼.
		std::unique_ptr<Object> m_leftArrowButton;								     // 좌측 화살표
		std::unique_ptr<Object> m_rightArrowButton;								     // 우측 화살표
		std::vector<std::unique_ptr<Object>> m_enhancementButtons;					 // 주문서 버튼 3개.
		//std::vector<std::unique_ptr<Object>> m_sheetImages;						 // 강화 유무 5개. 
		std::vector<std::unique_ptr<Object>> m_normalImages;					     // 강화 유무 5개. 
		std::vector<std::unique_ptr<Object>> m_successImages;					     // 강화 유무 5개. 
		std::vector<std::unique_ptr<Object>> m_failImages;							 // 강화 유무 5개. 
		std::unordered_map<int, D2D_VECTOR_2F> m_sheetPosition;

		std::unique_ptr<UI_Renderer> uirender = nullptr;
		Stat m_selectedStat = Stat::STR;

		// 렌더링할 시트 개수
		int m_renderSheetCount = 0;
		Item* m_targetItem = nullptr;
		float explodeTime = 0.f;	// explode effect play time

		// fixedUpdate에서 이펙트 time check
		float time = 0.f;
		float x = 0.f;
		// 랜덤
		std::mt19937 m_rng;

		// 선택된 주문서 인덱스.
		size_t m_selectedScrollIndex = 0;
};

