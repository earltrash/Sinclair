#include "Scene_MK.h"
#include "Button.h"

#include "pch.h"
#include "BackgroundComponent.h"
#include "ButtonComponent.h"
#include "MouseListenerComponent.h"
#include "InputManager.h"
#include "UI_Renderer.h"
#include "Renderer.h"
#include "SpriteRenderer.h"
#include "RenderInfo.h"
#include "EffectComponent.h"
#include "PlayComponent.h"

using path = std::filesystem::path;

Scene_MK::Scene_MK(string name)
{
	m_name = name;
}

Scene_MK::~Scene_MK()
{
}

void Scene_MK::Initalize()
{
	//////////////////////
	// Background 

	// 1. 이미지 갖고 오기
	auto gameStartBackground = ResourceManager::Get().GetTexture("시작화면");
	// 2. 오브젝트 만들기
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));
	// 3. 컴포넌트 넣기
	auto renderinfo = Background->GetRenderInfo();
	renderinfo->SetBitmap(gameStartBackground.Get());
	auto bgComp = Background->AddComponent<BackgroundComponent>(renderinfo);
	// 3.1.1 사이즈 다르면 
	bgComp->SetWidth(1920); bgComp->SetHeight(1080);
	bgComp->BitmapPush("Background", gameStartBackground);
	
	// 9. 게임 오브젝트들에 집어넣기
	m_gameObjects.emplace("1Background", std::move(Background));


	auto dustBitmap = ResourceManager::Get().GetTexture("airParticle");

	auto Dust1 = std::make_unique<Object>();
	auto dustInfo1 = Dust1->GetRenderInfo();
	dustInfo1->SetBitmap(dustBitmap.Get());
	auto blur1 = Dust1->AddComponent<GaussianBlur_Effect>(dustInfo1, 3.f, dustBitmap.Get());
	auto updown = Dust1->AddComponent<UpDown_Effect>(dustInfo1, Dust1->GetTransform(), 5.f, 0.02f, 0.02f);
	auto slide = Dust1->AddComponent<Slide_Effect>(dustInfo1, Dust1->GetTransform(), 0.05f, dustBitmap.Get()->GetSize().width, 1920.f, true);
	m_gameObjects.emplace("2", std::move(Dust1));

	auto Dust2 = std::make_unique<Object>();
	auto dustInfo2 = Dust2->GetRenderInfo();
	dustInfo2->SetBitmap(dustBitmap.Get());	// SetBitmap을 해줘야 srcRect 비트맵 크기로 초기화됨

	Dust2->GetTransform().SetPosition({ 1920.f, 0.f });
	auto blur11 = Dust2->AddComponent<GaussianBlur_Effect>(dustInfo2, 3.f, dustBitmap.Get());
	auto updownn = Dust2->AddComponent<UpDown_Effect>(dustInfo2, Dust2->GetTransform(), 5.f, 0.02f, 0.02f);
	auto slidee = Dust2->AddComponent<Slide_Effect>(dustInfo2, Dust2->GetTransform(), 0.05f, dustBitmap.Get()->GetSize().width, 1920.f, true);

	m_gameObjects.emplace("3", std::move(Dust2));

	auto rayBitmap = ResourceManager::Get().GetTexture("portalRays");
	auto sheetBitmap = ResourceManager::Get().GetTexture("강화", "기본");
	auto ray = std::make_unique<Object>();
	auto rayInfo = ray->GetRenderInfo();
	rayInfo->SetBitmap(rayBitmap.Get());
	ray->GetTransform().SetPosition({ 200.f, 200.f });
	ray->GetTransform().SetPivotPreset(PivotPreset::Center, { rayBitmap.Get()->GetSize().width, rayBitmap.Get()->GetSize().height });
	ray->GetTransform().SetScale({ 1.f, 1.f });
	//auto blink = ray->AddComponent<Blinking_Effect>(rayInfo, 0.8f, 2.f);
	auto scaleRay = ray->AddComponent<Scale_Effect>(rayInfo, 100.f, 100.f, 1.5f, 1.5f, rayBitmap.Get());
	auto perspect = ray->AddComponent<Rotate3D_Effect>(rayInfo, 0.f, rayBitmap.Get()->GetSize().width / 2.f, rayBitmap.Get()->GetSize().height / 2.f, 0.f, 0.f, 0.2f, scaleRay->GetEffect());
	auto perspect2 = ray->AddComponent<Rotate3D_Effect>(rayInfo, 0.f, rayBitmap.Get()->GetSize().width / 2.f, rayBitmap.Get()->GetSize().height / 2.f, 0.f, 0.f, -0.2f, scaleRay->GetEffect());
	auto composite = ray->AddComponent<Composite_Effect>(rayInfo, perspect->GetEffect(), perspect2->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
	//auto composite4 = ray->AddComponent<Composite_Effect>(rayInfo, sheetBitmap.Get(), composite->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
	auto colorRay = ray->AddComponent<Color_Effect>(rayInfo, 255.f / 256.f, 192.f / 256.f, 203.f / 256.f, 1.f, composite->GetEffect());
	auto explodeRay = ray->AddComponent<Explode_Effect>(rayInfo, 1.5f, 1.5f, 1.5f, 1.5f, 1.f, colorRay->GetEffect());
	ray->GetRenderInfo()->SetisActive(false);
	m_gameObjects.emplace("4", std::move(ray));

	
	auto sheet = std::make_unique<Object>();
	auto sheetInfo = sheet->GetRenderInfo();
	sheetInfo->SetBitmap(sheetBitmap.Get());
	sheet->GetTransform().SetPosition({ 100.f, 100.f });
	auto shdwSheet = sheet->AddComponent<Shadow_Effect>(sheetInfo, 5.f, 0.5f, 0.5f, 0.5f, 0.8f, sheetBitmap.Get());
	auto scaleSheet = sheet->AddComponent<Scale_Effect>(sheetInfo, 1.5f, 1.5f, shdwSheet->GetEffect());
	auto cmpshadowSheet = sheet->AddComponent<Composite_Effect>(sheetInfo, sheetBitmap.Get(), scaleSheet->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
	auto compoSheet = sheet->AddComponent<Composite_Effect>(sheetInfo, cmpshadowSheet->GetEffect(), explodeRay->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
	m_gameObjects.emplace("5", std::move(sheet));

	auto YrayBitmap = ResourceManager::Get().GetTexture("treasureRay");
	auto Yray = std::make_unique<Object>();
	auto YrayInfo = Yray->GetRenderInfo();
	YrayInfo->SetBitmap(YrayBitmap.Get());
	Yray->GetTransform().SetPosition({ 500.f, 200.f });
	Yray->GetTransform().SetPivotPreset(PivotPreset::Center, { YrayBitmap.Get()->GetSize().width, YrayBitmap.Get()->GetSize().height });
	//auto Yblink = Yray->AddComponent<Blinking_Effect>(YrayInfo, 0.9f, 1.5f);
	auto scaleYray = Yray->AddComponent<Scale_Effect>(YrayInfo, 3.f, 3.f, YrayBitmap.Get());
	auto Yperspect = Yray->AddComponent<Rotate3D_Effect>(YrayInfo, 0.f, YrayBitmap.Get()->GetSize().width / 2.f, YrayBitmap.Get()->GetSize().height / 2.f, 0.f, 0.f, 0.15f, scaleYray->GetEffect());
	auto Yperspect2 = Yray->AddComponent<Rotate3D_Effect>(YrayInfo, 0.f, YrayBitmap.Get()->GetSize().width / 2.f, YrayBitmap.Get()->GetSize().height / 2.f, 0.f, 0.f, -0.15f, scaleYray->GetEffect());
	auto Ycomposite = Yray->AddComponent<Composite_Effect>(YrayInfo, Yperspect->GetEffect(), Yperspect2->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
	m_gameObjects.emplace("6", std::move(Yray));

	auto runeBitmap = ResourceManager::Get().GetTexture("runeword05");
	auto glowBitmap = ResourceManager::Get().GetTexture("basicglow");
	auto glow = std::make_unique<Object>();
	auto glowInfo = glow->GetRenderInfo();
	glowInfo->SetBitmap(glowBitmap.Get());
	glow->GetTransform().SetPosition({ 500.f, 700.f });
	auto blinkGlow = glow->AddComponent<Blink_Effect>(glowInfo, 0.7f, 2.f, glowBitmap.Get());
	auto colorGlow = glow->AddComponent<Color_Effect>(glowInfo, 51.f / 256.f, 51.f / 256.f, 255.f / 256.f, 1.f, blinkGlow->GetEffect());
	auto compoGlow = glow->AddComponent<Composite_Effect>(glowInfo, runeBitmap.Get(), colorGlow->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
	m_gameObjects.emplace("7", std::move(glow));
	
	auto ringBitmap = ResourceManager::Get().GetTexture("E081");
	auto ring = std::make_unique<Object>();
	auto ringInfo = ring->GetRenderInfo();
	ringInfo->SetBitmap(ringBitmap.Get());
	ringInfo->SetDestRect({ 0.f, 0.f, ringBitmap.Get()->GetSize().width, ringBitmap.Get()->GetSize().height });
	ring->SetPosition({ 500.f, 600.f });
	auto shdwRing = ring->AddComponent<Shadow_Effect>(ringInfo, 3.f, 0.f, 0.f, 0.f, 1.f, ringBitmap.Get());
	auto scaleRing = ring->AddComponent<Scale_Effect>(ringInfo, 1.2f, 1.2f, shdwRing->GetEffect());
	auto cmpshadowRing = ring->AddComponent<Composite_Effect>(ringInfo, ringBitmap.Get(), scaleRing->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
	auto cmpsRing = ring->AddComponent<Composite_Effect>(ringInfo, cmpshadowRing->GetEffect(), Ycomposite->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
	m_gameObjects.emplace("8", std::move(ring));
	
	//auto rune = std::make_unique<Object>();
	//auto runeInfo = rune->GetRenderInfo();
	//runeInfo->SetBitmap(runeBitmap.Get());
	//rune->GetTransform().SetPosition({ 500.f, 600.f });
	//rune->AddComponent<Composite_Effect>(runeInfo, runeBitmap.Get(), glowBitmap.Get(), D2D1_COMPOSITE_MODE_SOURCE_OVER);
	
}

void Scene_MK::Clean()
{
	m_gameObjects.clear();
	SceneAssets.clear();
}

void Scene_MK::Update()
{
	for (auto& obj : m_gameObjects)
	{
		obj.second->Update();
	}
}

void Scene_MK::LogicUpdate(float delta)
{
	for (auto& obj : m_gameObjects)
	{
		obj.second->FixedUpdate(delta);
	}
}

void Scene_MK::Enter()
{
	Initalize();
}

void Scene_MK::Exit()
{
}
using Vec2F = MYHelper::Vector2F;

void Scene_MK::Render()
{
	

	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
	}

	
}