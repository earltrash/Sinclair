#include "Scene_Title.h"
#include "Button.h"

#include "pch.h"

#include "MouseListenerComponent.h"
#include "InputManager.h"
#include "UI_Renderer.h"
#include "Renderer.h"
#include "SpriteRenderer.h"


using path = std::filesystem::path;

Scene_Title::Scene_Title(string name)
{
		m_name = name;
}

Scene_Title::~Scene_Title()
{
}

void Scene_Title::Initalize()
{
		GetAsset(m_name); //에셋만 받게 함. 

		unique_ptr<Button> button =make_unique<Button>("Start" , Rect(100.f, 100.f, 200.f, 200.f)); //rect는 중괄호임.

		ComPtr<ID2D1Bitmap1> temp;
		std::wstring imagePath = L"C:\\Users\\User\\Documents\\2q_mini\\_excutable\\Temp_02\\Resource\\Temp.png";

		D2DRenderer::Get().CreateBitmapFromFile(imagePath.c_str(), &temp);


		button->AddComponent<UI_Renderer>(temp);

		m_gameObjects.emplace(button->name, std::move(button));

		//얘가 갖고 있는 Obj
}

void Scene_Title::Clean()
{
		m_gameObjects.clear();
		SceneAssets.clear();
}

void Scene_Title::Update()
{

}

void Scene_Title::LogicUpdate(float delta)
{

}

void Scene_Title::Enter()
{
		Initalize();
}

void Scene_Title::Exit()
{
}
using Vec2F = MYHelper::Vector2F;

void Scene_Title::Render()
{
		D2DRenderer::Get().RenderBegin();

		for (const auto& [Name, obj] : m_gameObjects)
		{

				if (obj->GetComponent<SpriteRenderer>() != nullptr) //sprite로 랜더를 하는 경우 
				{
						const Frame& frame = obj->GetComponent<SpriteRenderer>()->GetCurrentFrame();

						//const Frame& frame = obj->GetSpriteRenderer().GetCurrentFrame();
						const auto& srcU = frame.srcRect;

						D2D1_RECT_F srcRect = D2D1::RectF(
								static_cast<float>(srcU.left),
								static_cast<float>(srcU.top),
								static_cast<float>(srcU.right),
								static_cast<float>(srcU.bottom)
						);

						const Vec2F& pos = obj->GetTransform().GetPosition(); // 오브젝트의 위치를 가져온다고 가정

						D2D1_RECT_F destRect = {
							pos.x,
							pos.y,
							pos.x + static_cast<float>(srcU.right - srcU.left),
							pos.y + static_cast<float>(srcU.bottom - srcU.top)
						};


						D2DRenderer::Get().DrawBitmap(obj->GetComponent<SpriteRenderer>()->GetCurrentClip().GetBitmap(), destRect, srcRect, 1);
				}


				//만약 Object 내에서 같이 Render 떄리고 싶다면 
				else if (obj->GetComponent<UI_Renderer>() != nullptr)
				{
						auto bmp = obj->GetComponent<UI_Renderer>()->GetBitmap();
						D2D1_SIZE_U bmpSize = bmp->GetPixelSize();

						// Transform 정보 받아옴 (예: 위치 + 크기)
						Vec2 transform = obj->GetTransform().GetPosition();

						D2D1_RECT_F destRect = {
								transform.x,
								transform.y,
								transform.x + static_cast<float>(bmpSize.width) * transform.x,
								transform.y + static_cast<float>(bmpSize.height) * transform.y
						};

						// D2DRenderer 내부에서 deviceContext 사용하고 있다고 가정
						D2DRenderer::Get().DrawBitmap(
								bmp.Get(),
								destRect);

				}


		}

		D2DRenderer::Get().RenderEnd();
}