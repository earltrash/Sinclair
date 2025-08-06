#include "Scene_End.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

Scene_End::Scene_End(string name)
{
	m_name = name;
}

Scene_End::~Scene_End()
{

}

void Scene_End::Initalize()
{
	if (dirty) return; // �̹� �ʱ�ȭ�Ǿ����� ��ŵ

	CreateObj(); // ������Ʈ ���� (�� ����)
	dirty = true;
}

void Scene_End::Enter()
{

	Initalize();
}

void Scene_End::Exit()
{

	Clean();
}

void Scene_End::Clean()
{
	m_gameObjects.clear();
}

void Scene_End::Update()
{

}

void Scene_End::LogicUpdate(float delta)
{


}


void Scene_End::Render()
{

}

void Scene_End::CreateObj()
{
	/////////////////////
	/////////////////////
	// ���

	// 1. �̹��� ���� ����
	auto �ΰ���3 = ResourceManager::Get().GetTexture("�ΰ���", "3");
	// 2. ������Ʈ �����
	auto ��� = std::make_unique<Object>();
	���->SetPosition(Vec2(147, 703));
	// 3. ��ư ������Ʈ �����
	auto ����ư = ���->AddComponent<ButtonComponent>();
	����ư->SetWidth(310); ����ư->SetHeight(64);

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	����ư->BitmapPush("normal", �ΰ���3);
	����ư->BitmapPush("hover", �ΰ���3);
	����ư->BitmapPush("pressed", �ΰ���3);

	����ư->SetCurrentBitmap("normal");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto ��񸮽��� = ���->AddComponent<MouseListenerComponent>(
		[����ư](const MSG& msg) {
			����ư->Worked(msg);
		}
	);

	����ư->SetOnClickCallback([this]() {
		std::cout << "����ư������ �����丮��" << std::endl;
		SceneManager::Get().ChangeScene("History");
		});

	/// 9
	m_gameObjects.emplace("�ΰ���3", std::move(���));
}