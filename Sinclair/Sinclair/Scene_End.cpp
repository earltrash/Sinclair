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
	
	dirty = false;
}

void Scene_End::Update()
{

}

void Scene_End::LogicUpdate(float delta)
{


}


void Scene_End::Render()
{
	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
	}
}

void Scene_End::CreateObj()
{
	//////////////////////
	//////////////////////
	//////////////////////
	// [1] ���� â

	// 1. �̹��� ���� ����
	auto ����1 = ResourceManager::Get().GetTexture("����", "01");
	// 2. ������Ʈ �����
	auto ����â = std::make_unique<Object>();
	//����â->SetPosition(Vec2(-20, -16));
	����â->SetPosition(Vec2(0, 0));

	// 3.0. ���� ���� ������Ʈ
	auto ����âinfo = ����â->GetRenderInfo();
	����âinfo->SetBitmap(����1.Get());
	// 3. ��� ������Ʈ �����
	auto ����âComp = ����â->AddComponent<BackgroundComponent>(����âinfo);
	����âComp->SetWidth(1118);
	����âComp->SetHeight(1113);
	����âComp->BitmapPush("����â", ����1);
	����âComp->SetCurrentBitmap("����â");

	/// 9
	m_gameObjects.emplace("a����â", std::move(����â));

	//////////////////////
	//////////////////////
	//////////////////////
	// [2] �̹���
	// ȿ�� : �̰� ��� �ؾ����� ����ϱ�

	// 1. �̹��� ���� ����
	auto ����2 = ResourceManager::Get().GetTexture("����", "02");
	// 2. ������Ʈ �����
	auto �̹��� = std::make_unique<Object>();
	�̹���->SetPosition(Vec2(17, 22));

	auto �̹���info = �̹���->GetRenderInfo();
	�̹���info->SetBitmap(����2.Get());
	// 3. ��� ������Ʈ �����
	auto �̹���Comp = �̹���->AddComponent<BackgroundComponent>(�̹���info);
	�̹���Comp->SetWidth(����2->GetSize().width);
	�̹���Comp->SetHeight(����2->GetSize().height);
	�̹���Comp->BitmapPush("�̹���", ����2);
	�̹���Comp->SetCurrentBitmap("�̹���");


	/// 9
	m_gameObjects.emplace("�̹���", std::move(�̹���));

	//////////////////////
	//////////////////////
	//////////////////////
	// [3] ���� â

	// 1. �̹��� ���� ����
	auto ����3 = ResourceManager::Get().GetTexture("����", "03");
	// 2. ������Ʈ �����
	auto ����â = std::make_unique<Object>();
	����â->SetPosition(Vec2(1077, -20));

	auto ����âinfo = ����â->GetRenderInfo();
	����âinfo->SetBitmap(����3.Get());
	// 3. ��� ������Ʈ �����
	auto ����âComp = ����â->AddComponent<BackgroundComponent>(����âinfo);
	����âComp->SetWidth(����1->GetSize().width);
	����âComp->SetHeight(����1->GetSize().height);
	����âComp->BitmapPush("����â", ����1);
	����âComp->SetCurrentBitmap("����â");

	/// 9
	m_gameObjects.emplace("a����â", std::move(����â));

	//////////////////////
	//////////////////////
	//////////////////////
	// [4] ��ŵ ��ư

	// 1. �̹��� ���� ����
	auto ����4 = ResourceManager::Get().GetTexture("����", "04");
	// 2. ������Ʈ �����
	auto ��ŵ��ư = std::make_unique<Object>();
	��ŵ��ư->SetPosition(Vec2(1827, 960));

	auto ��ŵ��ưinfo = ��ŵ��ư->GetRenderInfo();
	��ŵ��ưinfo->SetBitmap(����4.Get());

	// 3. ��ư ������Ʈ �����
	auto ��ŵ������Ʈ = ��ŵ��ư->AddComponent<ButtonComponent>(��ŵ��ưinfo);
	��ŵ������Ʈ->SetWidth(����4->GetSize().width);
	��ŵ������Ʈ->SetHeight(����4->GetSize().height);

	// 4. ��ư ��Ʈ�� ����
	��ŵ������Ʈ->BitmapPush("��ŵ", ����4);
	��ŵ������Ʈ->SetCurrentBitmap("��ŵ");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto ��ŵ������ = ��ŵ��ư->AddComponent<MouseListenerComponent>(
		[��ŵ������Ʈ](const MSG& msg) {
			��ŵ������Ʈ->Worked(msg);
		}
	);

	��ŵ������Ʈ->SetOnClickCallback([this]() {
		std::cout << "��ŵ ��ư Ŭ����" << std::endl;
		// ���� ��ŵ ���� �߰�
		});

	/// 9
	m_gameObjects.emplace("��ŵ��ư", std::move(��ŵ��ư));

	////////////////////////
	////////////////////////
	////////////////////////
	//// [5] ���� ��Ī (�߾�����, 90pt)

	//// 1. �ؽ�Ʈ ������Ʈ �����
	//auto ������Ī = std::make_unique<Object>();
	//// �߾������� ���� ��ġ ���� (ȭ�� �ʺ��� �߾�)
	//������Ī->SetPosition(Vec2(960, 400)); // �ӽ� ��ġ, ���� ȭ�� ũ�⿡ �°� ����

	//auto ������Īinfo = ������Ī->GetRenderInfo();
	//// �ؽ�Ʈ ������Ʈ ���� (90pt ��Ʈ)
	//// ���� �ؽ�Ʈ ������Ʈ ������ ���� ���� �ʿ�

	///// 9
	//m_gameObjects.emplace("������Ī", std::move(������Ī));

	//////////////////////
	//////////////////////
	//////////////////////
	// [6] ��

	// 1. �̹��� ���� ����
	auto ����6 = ResourceManager::Get().GetTexture("����", "06");
	// 2. ������Ʈ �����
	auto �� = std::make_unique<Object>();
	��->SetPosition(Vec2(1223, 255));

	auto ��info = ��->GetRenderInfo();
	��info->SetBitmap(����6.Get());
	// 3. ��� ������Ʈ �����
	auto ��Comp = ��->AddComponent<BackgroundComponent>(��info);
	��Comp->SetWidth(����6->GetSize().width);
	��Comp->SetHeight(����6->GetSize().height);
	��Comp->BitmapPush("��", ����6);
	��Comp->SetCurrentBitmap("��");

	/// 9
	m_gameObjects.emplace("��", std::move(��));

	////////////////////////
	////////////////////////
	////////////////////////
	//// [7] ���� ���� (�߾�����, 30pt)

	//// 1. �ؽ�Ʈ ������Ʈ �����
	//auto �������� = std::make_unique<Object>();
	//// �߾������� ���� ��ġ ���� (ȭ�� �ʺ��� �߾�)
	//��������->SetPosition(Vec2(960, 600)); // �ӽ� ��ġ, ���� ȭ�� ũ�⿡ �°� ����

	//auto ��������info = ��������->GetRenderInfo();
	//// �ؽ�Ʈ ������Ʈ ���� (30pt ��Ʈ)
	//// ���� �ؽ�Ʈ ������Ʈ ������ ���� ���� �ʿ�

	///// 9
	//m_gameObjects.emplace("��������", std::move(��������));
}