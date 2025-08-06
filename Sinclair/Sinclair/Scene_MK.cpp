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
	m_gameObjects.emplace("2Dust", std::move(Dust1));

	auto Dust2 = std::make_unique<Object>();
	auto dustInfo2 = Dust2->GetRenderInfo();
	dustInfo2->SetBitmap(dustBitmap.Get());	// SetBitmap을 해줘야 srcRect 비트맵 크기로 초기화됨

	Dust2->GetTransform().SetPosition({ 1920.f, 0.f });
	auto blur11 = Dust2->AddComponent<GaussianBlur_Effect>(dustInfo2, 3.f, dustBitmap.Get());
	auto updownn = Dust2->AddComponent<UpDown_Effect>(dustInfo2, Dust2->GetTransform(), 5.f, 0.02f, 0.02f);
	auto slidee = Dust2->AddComponent<Slide_Effect>(dustInfo2, Dust2->GetTransform(), 0.05f, dustBitmap.Get()->GetSize().width, 1920.f, true);

	m_gameObjects.emplace("3Dust", std::move(Dust2));

	auto rayBitmap = ResourceManager::Get().GetTexture("treasureRay");
	auto ray = std::make_unique<Object>();
	auto rayInfo = ray->GetRenderInfo();
	rayInfo->SetBitmap(rayBitmap.Get());
	//ray->GetTransform().SetPivotPreset(PivotPreset::Center, rayInfo->GetSize());
	ray->GetTransform().SetPosition({ 0.f, 0.f });
	//auto rotate = ray->AddComponent<Rotate_Effect>(rayInfo, ray->GetTransform(), 0.05f);
	auto blink = ray->AddComponent<Blinking_Effect>(rayInfo, 0.85f, 1.f);
	auto blur_ray = ray->AddComponent<GaussianBlur_Effect>(rayInfo, 5.f, rayBitmap.Get());
	auto blur_ray2 = ray->AddComponent<GaussianBlur_Effect>(rayInfo, 5.f, rayBitmap.Get());
	auto perspect = ray->AddComponent<Rotate3D_Effect>(rayInfo, 0.f, 0.f, 0.f, 0.5f, blur_ray->GetEffect());
	auto perspect2 = ray->AddComponent<Rotate3D_Effect>(rayInfo, 0.f, 0.f, 0.f, -0.1f, blur_ray2->GetEffect());
	auto composite = ray->AddComponent<Composite_Effect>(rayInfo, perspect->GetEffect(), perspect2->GetEffect(), D2D1_COMPOSITE_MODE_SOURCE_COPY);

	m_gameObjects.emplace("4Dust", std::move(ray));
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