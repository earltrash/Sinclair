#include "Scene_BJ.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "Renderer.h"
#include "UIManager.h"
SceneBj::SceneBj(string name)
{
	m_name = name;
	CreateObj(); //에셋까지 여기로 들어감 .



}

void SceneBj::Initalize()
{
}

void SceneBj::Clean()
{
}

void SceneBj::Update()
{
}

void SceneBj::LogicUpdate(float delta)
{
}

void SceneBj::Enter()
{
}

void SceneBj::Exit()
{
}

void SceneBj::Render()
{
	//D2DRenderer::Get().RenderBegin();

	//for (const auto& [Name, obj] : m_gameObjects)
	//{

	//	ComPtr<ID2D1Bitmap1> bitmap = nullptr;
	//	D2D1_RECT_F dest;
	//	dest.left = 0;
	//	dest.top = 0;
	//	dest.right = 0;
	//	dest.bottom = 0;

	//	// ButtonComponent 우선 확인
	//	auto buttonComp = obj->GetComponent<ButtonComponent>();
	//	if (buttonComp != nullptr) {
	//		bitmap = buttonComp->GetBitmap();
	//		dest.right += buttonComp->GetWidth();
	//		dest.bottom += buttonComp->GetHeight();
	//	}
	//	else {
	//		// ButtonComponent가 없으면 BackgroundComponent 확인
	//		auto bgComp = obj->GetComponent<BackgroundComponent>();
	//		if (bgComp != nullptr) {
	//			bitmap = bgComp->GetBitmap();
	//			dest.right += bgComp->GetWidth();
	//			dest.bottom += bgComp->GetHeight();
	//		}
	//	}

	//	if (!bitmap) {
	//		continue;
	//	}

	//	auto size = bitmap->GetSize();

	//	dest.left += obj->GetTransform().m_position.x;
	//	dest.top += obj->GetTransform().m_position.y;
	//	dest.right += dest.left;
	//	dest.bottom += dest.top;

	//	D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest);
	//}

	// UIManager::Get().Render();


	//D2DRenderer::Get().RenderEnd();
}

void SceneBj::CreateObj()
{




}
