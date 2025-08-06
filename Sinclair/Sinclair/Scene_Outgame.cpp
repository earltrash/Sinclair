#include "Scene_Outgame.h"
#include "Object.h"
#include "ButtonComponent.h"
#include "BackgroundComponent.h"
#include "MouseListenerComponent.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "Renderer.h"

// ����
#include <random>
#include <iterator>

Scene_Outgame::Scene_Outgame(string name)
{
		m_name = name;

}

Scene_Outgame::~Scene_Outgame()
{

	
}

void Scene_Outgame::Initalize()
{
	if (dirty) return; // �̹� �ʱ�ȭ�Ǿ����� ��ŵ

	CreateObj(); // ������Ʈ ���� (�� ����)
	dirty = true;
}

void Scene_Outgame::Clean()
{
	m_gameObjects.clear();
	//SceneAssets.clear(); // Ŭ�������� ���� ���� ����
	dirty = false;
}

void Scene_Outgame::Update()
{
}

void Scene_Outgame::LogicUpdate(float delta)
{
}

void Scene_Outgame::Enter()
{
	Initalize();

	auto bg = m_gameObjects["Background"s]->GetComponent<BackgroundComponent>();
	bg->SetCurrentBitmap("Background");

	// Ű�� �̹��� ���� �ϴ� ���� "Sinclair + GameManager::Get().curGen" = Sinclair3? 2? 4?
	auto ch = m_gameObjects["Character"s]->GetComponent<BackgroundComponent>();
	std::string result = "Sinclair";
	result += std::to_string(GameManager::Get().curGen);
	ch->SetCurrentBitmap(result);
}

void Scene_Outgame::Exit()
{
	Clean();
}

void Scene_Outgame::Render()
{
	// ��Ƽ���� Ű(Name)�������� ���� �ȴ�. 
	// �� background ����� ���� �׸��� ��
	
	for (const auto& [Name, obj] : m_gameObjects)
	{

		ComPtr<ID2D1Bitmap1> bitmap = nullptr;
		D2D1_RECT_F dest;
		dest.left = 0;
		dest.top = 0;
		dest.right = 0;
		dest.bottom = 0;
		// ���� �����Ѵٸ�...
		float opacity = 1.0f;

		// ButtonComponent �켱 Ȯ��
		auto buttonComp = obj->GetComponent<ButtonComponent>();
		if (buttonComp != nullptr) {
			bitmap = buttonComp->GetBitmap();
			dest.right += buttonComp->GetWidth();
			dest.bottom += buttonComp->GetHeight();
			opacity = buttonComp->m_opacity;
		}
		else {
			// ButtonComponent�� ������ BackgroundComponent Ȯ��
			auto bgComp = obj->GetComponent<BackgroundComponent>();
			if (bgComp != nullptr) {
				bitmap = bgComp->GetCurrentBitmap();
				dest.right += bgComp->GetWidth();
				dest.bottom += bgComp->GetHeight();
			}
		}

		if (!bitmap) {
			continue;
		}

		//dest.left += obj->GetTransform().m_position.x;
		//dest.top += obj->GetTransform().m_position.y;
		dest.left += obj->GetTransform().GetPosition().x;
		dest.top += obj->GetTransform().GetPosition().y;
		dest.right += dest.left;
		dest.bottom += dest.top;

		// ���� ���� x
		//D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest);
		// ���� ���� o
		D2D1_RECT_F srcRect = D2D1::RectF(0, 0, bitmap->GetSize().width, bitmap->GetSize().height); ;

		D2DRenderer::Get().DrawBitmap(bitmap.Get(), dest, srcRect, opacity);
	}

	D2DRenderer::Get().CreateWriteRegularResource();
	D2DRenderer::Get().DrawMessage(L"��Ŭ���� N�� ", 503.f, 805.f, 1300.f, 1000.f, D2D1::ColorF::BlueViolet);
	D2DRenderer::Get().DrawMessage(L"â��� �̵��ұ�?\n(���� �� â��� �̵��մϴ�.)", 503.f, 869.f, 1300.f, 1000.f, D2D1::ColorF::BlueViolet);

	
}

void Scene_Outgame::CreateObj()
{
	//////////////////////
	// Background 

	// 1. �̹��� ���� ����
	auto outGameBackground = ResourceManager::Get().GetTexture("�ƿ�����");
	// 2. ������Ʈ �����
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));

	// 3.0. ���� ���� ������Ʈ
	auto bgInfo = Background->AddComponent<RenderInfo>(outGameBackground.Get());

	// 3. ��� ������Ʈ �����
	auto bgComp = Background->AddComponent<BackgroundComponent>(bgInfo);
	// 3.1.1 ������ �ٸ��� 
	bgComp->SetWidth(1920); bgComp->SetHeight(1080);
	bgComp->BitmapPush("Background", outGameBackground);
	// 9. ���� ������Ʈ�鿡 ����ֱ�
	m_gameObjects.emplace("Background", std::move(Background));



	//////////////////////
	//////////////////////
	// ĳ���� �̹��� : ��� ������Ʈ��� ��� ������ -> �̹��� ������Ʈ��� �ٲ�� ��. 
	// ���� ����ٰ� ���̴�? �־�� �ȴٰ� ������. 
	// ĳ���Ϳ� �÷� ������ �ٲ�����. -> �׷� ��ư ������Ʈ�� �ٲ�� �ϴ� �� �ƴѰ�?

	// 1. �̹��� ���� ����
	auto Sinclair2 = ResourceManager::Get().GetTexture("��Ŭ����", "2��");
	auto Sinclair3 = ResourceManager::Get().GetTexture("��Ŭ����", "3��");
	auto Sinclair4 = ResourceManager::Get().GetTexture("��Ŭ����", "4��");
	// 2. ������Ʈ �����
	auto Character = std::make_unique<Object>();
	Character->SetPosition(Vec2(0, 0));

	// 3.0. ���� ���� ������Ʈ
	auto chInfo = Character->AddComponent<RenderInfo>(Sinclair2.Get());
	// 3. ��� ������Ʈ �����
	auto chComp = Character->AddComponent<BackgroundComponent>(chInfo);

	// 3.1.1 ������ �ٸ��� 
	chComp->SetWidth(1920); chComp->SetHeight(1080);
	chComp->BitmapPush("Sinclair2", Sinclair2);
	chComp->BitmapPush("Sinclair3", Sinclair3);
	chComp->BitmapPush("Sinclair4", Sinclair4);
	// 9. ���� ������Ʈ�鿡 ����ֱ� 
	m_gameObjects.emplace("Character", std::move(Character));
	

	//////////////////////
	//////////////////////
	// �ؽ�Ʈ �ڽ� 
	
	// 1. �̹��� ���� ����
	auto �ƿ�����1 = ResourceManager::Get().GetTexture("�ƿ�����", "1");
	// 2. ������Ʈ �����
	auto �ؽ�Ʈ�ڽ� = std::make_unique<Object>();
	     �ؽ�Ʈ�ڽ�->SetPosition(Vec2(455, 772));

	// 3.0. ���� ���� ������Ʈ
	auto outgame1Info = �ؽ�Ʈ�ڽ�->AddComponent<RenderInfo>(�ƿ�����1.Get());
	// 3. ��� ������Ʈ �����
	auto �ؽ�Ʈ�ڽ�Comp = �ؽ�Ʈ�ڽ�->AddComponent<BackgroundComponent>(outgame1Info);
	// 3.1.1 ������ �ٸ��� 
	�ؽ�Ʈ�ڽ�Comp->SetWidth(1010); �ؽ�Ʈ�ڽ�Comp->SetHeight(207);
	�ؽ�Ʈ�ڽ�Comp->BitmapPush("�ƿ�����1", �ƿ�����1);
	�ؽ�Ʈ�ڽ�Comp->SetCurrentBitmap("�ƿ�����1");
	// 9. ���� ������Ʈ�鿡 ����ֱ� 
	m_gameObjects.emplace("�ƿ�����1", std::move(�ؽ�Ʈ�ڽ�));


	/////////////////////
	/////////////////////
	// �� (���� ��ư)

	// 1. �̹��� ���� ����
	auto �ƿ�����4 = ResourceManager::Get().GetTexture("�ƿ�����", "4");
	// 2. ������Ʈ �����
	auto �� = std::make_unique<Object>();
	��->SetPosition(Vec2(1087, 921));
	// 3. ��ư ������Ʈ �����
	auto YesComp = ��->AddComponent<ButtonComponent>();
	YesComp->SetWidth(140); YesComp->SetHeight(40);

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	YesComp->BitmapPush("normal",  �ƿ�����4);
	YesComp->BitmapPush("hover",   �ƿ�����4);
	YesComp->BitmapPush("pressed", �ƿ�����4);

	YesComp->SetCurrentBitmap("normal");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto Yes_mouseListener = ��->AddComponent<MouseListenerComponent>(
		[YesComp](const MSG& msg) {
			YesComp->Worked(msg);
		}
	);

	YesComp->SetOnClickCallback([this]() {
		SceneManager::Get().ChangeScene("InGame");
		std::cout << "SceneManager::Get().ChangeScene(\"InGame\");" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("�ƿ�����4", std::move(��));


	/////////////////////
	// �ƴϿ� (���� ��ư)

	// 1. �̹��� ���� ����
	auto �ƿ�����5 = ResourceManager::Get().GetTexture("�ƿ�����", "5");
	// 2. ������Ʈ �����
	auto �ƴϿ� = std::make_unique<Object>();
	�ƴϿ�->SetPosition(Vec2(1280, 921));
	// 3. ��ư ������Ʈ �����
	auto NoComp = �ƴϿ�->AddComponent<ButtonComponent>();
	NoComp->SetWidth(140); NoComp->SetHeight(40);

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	NoComp->BitmapPush("normal",  �ƿ�����5);
	NoComp->BitmapPush("hover",   �ƿ�����5);
	NoComp->BitmapPush("pressed", �ƿ�����5);

	NoComp->SetCurrentBitmap("normal");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto No_mouseListener = �ƴϿ�->AddComponent<MouseListenerComponent>(
		[NoComp](const MSG& msg) {
			NoComp->Worked(msg);
		}
	);

	NoComp->SetOnClickCallback([this]() {
		//SceneManager::Get().ChangeScene("InGame");
		std::cout << "�ƴϿ� (���� ��ư)" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("�ƿ�����5", std::move(�ƴϿ�));


	/////////////////////
	// �ڷΰ��� (����ȭ������)

	// 1. �̹��� ���� ����
	auto �ڷΰ��� = ResourceManager::Get().GetTexture("�ڷΰ���");
	// 2. ������Ʈ �����
	auto �ڷ� = std::make_unique<Object>();
	�ڷ�->SetPosition(Vec2(64 , 57));
	// 3. ��ư ������Ʈ �����
	auto backComp = �ڷ�->AddComponent<ButtonComponent>();
	backComp->SetWidth(37); backComp->SetHeight(37);
	
	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	backComp->BitmapPush("normal", �ڷΰ���);
	backComp->BitmapPush("hover", �ڷΰ���);
	backComp->BitmapPush("pressed", �ڷΰ���);

	backComp->SetCurrentBitmap("normal");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto Back_mouseListener = �ڷ�->AddComponent<MouseListenerComponent>(
		[backComp](const MSG& msg) {
			backComp->Worked(msg);
		}
	);

	backComp->SetOnClickCallback([this]() {
		std::cout << "��ư Ŭ���� - ���� ��: " << typeid(*this).name() << std::endl;
		SceneManager::Get().ChangeScene("Title");
		//std::cout << "�ڷΰ��� (����ȭ������)" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("�ڷΰ���", std::move(�ڷ�));

}

std::string Scene_Outgame::getRandomText()
{
	auto range = outGameTextTable.equal_range(m_id);

	if (range.first == range.second) {
		return "�ؽ�Ʈ ����"; // ID�� ���� ���
	}

	// �ش� ID�� ��� �ؽ�Ʈ�� ���ͷ� ����
	std::vector<std::string> texts;
	for (auto it = range.first; it != range.second; ++it) {
		texts.push_back(it->second);
	}

	// ���� ����
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, texts.size() - 1);

	return texts[dist(gen)];
}
