#include "pch.h"
#include "Scene.h"
//#include "SceneFactory.h"
#include "Renderer.h"
#include "SpriteRenderer.h"
#include "UI_Renderer.h"


void SceneStandard::Update()
{
		for (auto& e : m_gameObjects)
		{
				e.second->Update(); //일단 뭐 없긴 함. 
		}
}


void SceneStandard::LogicUpdate(float delta)
{

		for (auto& [name, obj] : m_gameObjects) //Object 위치에 접근을 해야 하니깐, Object 개수만큼 반복, 
		{
				//자신 있음 주석에 있는 거로 떄리셈 

				//obj->GetSpriteRenderer().Update(delta); //sprite update 

				if (obj->GetComponent<SpriteRenderer>() != nullptr)
						obj->GetComponent< SpriteRenderer>()->Update(delta);
		}

}

using Vec2F = MYHelper::Vector2F;

void SceneStandard::Render() //UI 렌더
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

void SceneStandard::AssetMapping() 
{
		for (auto& [name, obj] : m_gameObjects) { //map의 auto 반복문은 이런 구조...
				for (const auto& asset : SceneAssets) {
						if (asset.Name == name) {
								{
										if (obj->GetComponent<SpriteRenderer>() != nullptr)
										{
												obj->GetComponent<SpriteRenderer>()->AddClip(asset.Ani_Name, asset.clip);
												if (asset.Ani_Name == "Idle")
														obj->GetComponent<SpriteRenderer>()->SetCurrentClip(&asset.clip);
										}
										else if (obj->GetComponent<UI_Renderer>() != nullptr)
										{
												obj->GetComponent<UI_Renderer>()->SetBitmap(asset.clip.GetBitmap()); //아 근데 이건 고민해 봐야 할 듯, 버튼도 여러개일 거니깐 
										}

								}
						}
				}
				return;
		}
}





