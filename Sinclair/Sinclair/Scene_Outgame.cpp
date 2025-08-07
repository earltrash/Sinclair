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
	Clean();
}

void Scene_Outgame::Initalize()
{
	if (dirty) return;  

	CreateObj();  
	dirty = true;
}

void Scene_Outgame::Clean()
{
		m_gameObjects.clear();
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
	SetupCharacterAndBackground();


	 if (GameManager::Get().WasInGame) {
		 m_state = State::CHOICE_MENU;
	 } else {
		 m_state = State::FIRST_ENTER;
		 GameManager::Get().WasInGame = true;
}


	// �켱 �ʱ� ���·� ����
	ChangeState(m_state);
}

void Scene_Outgame::Exit()
{
		Clean();
}

void Scene_Outgame::Render()
{

	for (const auto& [Name, obj] : m_gameObjects)
	{
		D2DRenderer::Get().DrawBitmap(obj->GetRenderInfo()->GetRenderInfo());
	}




	D2DRenderer::Get().CreateWriteResource(L"���� ����� Bold", DWRITE_FONT_WEIGHT_BOLD, 36.0f);
	std::wstring characterName = L"��Ŭ���� " + std::to_wstring(GameManager::Get().curGen) + L"��";
	D2DRenderer::Get().DrawMessage(characterName.c_str(), 481.74f, 828.38f, 1300.f, 1000.f, D2D1::ColorF::White);

	D2DRenderer::Get().CreateWriteResource(L"���� ����� Regular", DWRITE_FONT_WEIGHT_NORMAL, 24.0f);
	// ���¿� ���� ���� �ؽ�Ʈ ������
	if (!curText.empty())
	{
		std::wstring wCurText = StrToWstr(curText);
		D2DRenderer::Get().DrawMessage(wCurText.c_str(), 768.21f, 828.38f, 1300.f, 1000.f, D2D1::ColorF::White);
	}

	D2DRenderer::Get().CreateWriteResource();

}

void Scene_Outgame::CreateObj()
{
	//////////////////////
	//////////////////////
	//////////////////////
	// [1] Background 

	// 1. �̹��� ���� ����
	auto outGameBackground = ResourceManager::Get().GetTexture("�ƿ�����");
	// 2. ������Ʈ �����
	auto Background = std::make_unique<Object>();
	Background->SetPosition(Vec2(0, 0));

	// 3.0. ���� ���� ������Ʈ
	auto bgInfo = Background->GetRenderInfo();
	bgInfo->SetBitmap(outGameBackground.Get());

	// 3. ��� ������Ʈ �����
	auto bgComp = Background->AddComponent<BackgroundComponent>(bgInfo);
	// 3.1.1 ������ �ٸ��� 
	bgComp->SetWidth(1920); bgComp->SetHeight(1080);
	bgComp->BitmapPush("Background", outGameBackground);
	// 9. ���� ������Ʈ�鿡 ����ֱ�
	m_gameObjects.emplace("Background", std::move(Background));



	//////////////////////
	//////////////////////
	//////////////////////
	// [2] ĳ���� �̹��� 
	// ��� ������Ʈ��� ��� ������ -> �̹��� ������Ʈ��� �ٲ�� ��. 
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
	auto chInfo = Character->GetRenderInfo();
	chInfo->SetBitmap(Sinclair2.Get());
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
	//////////////////////
	// [3] �ؽ�Ʈ â
	
	// 1. �̹��� ���� ����
	auto �ƿ�����1 = ResourceManager::Get().GetTexture("�ƿ�����", "01");
	// 2. ������Ʈ �����
	auto �ؽ�Ʈ�ڽ� = std::make_unique<Object>();
	     �ؽ�Ʈ�ڽ�->SetPosition(Vec2(421, 766));

	// 3.0. ���� ���� ������Ʈ
		 auto outgame1Info = �ؽ�Ʈ�ڽ�->GetRenderInfo();
		 outgame1Info->SetBitmap(�ƿ�����1.Get());
	// 3. ��� ������Ʈ �����
	auto �ؽ�Ʈ�ڽ�Comp = �ؽ�Ʈ�ڽ�->AddComponent<BackgroundComponent>(outgame1Info);
	
	// 3.1.1 ������ 
	�ؽ�Ʈ�ڽ�Comp->SetWidth(�ƿ�����1->GetSize().width);
	�ؽ�Ʈ�ڽ�Comp->SetHeight(�ƿ�����1->GetSize().height);


	�ؽ�Ʈ�ڽ�Comp->BitmapPush("�ƿ�����1", �ƿ�����1);
	�ؽ�Ʈ�ڽ�Comp->SetCurrentBitmap("�ƿ�����1");
	// 9. ���� ������Ʈ�鿡 ����ֱ� 
	m_gameObjects.emplace("�ƿ�����1", std::move(�ؽ�Ʈ�ڽ�));


	/////////////////////
	/////////////////////
	/////////////////////
	// [4] �� ��ư

	// 1. �̹��� ���� ����
	auto �ƿ�����4 = ResourceManager::Get().GetTexture("�ƿ�����", "04");
	// 2. ������Ʈ �����
	auto �� = std::make_unique<Object>();
	��->SetPosition(Vec2(1134, 905));

	auto ��info = ��->GetRenderInfo();
	��info->SetBitmap(�ƿ�����4.Get());
	// 3. ��ư ������Ʈ �����
	auto YesComp = ��->AddComponent<ButtonComponent>(��info);
	YesComp->SetWidth( �ƿ�����4->GetSize().width);
	YesComp->SetHeight(�ƿ�����4->GetSize().height);

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	YesComp->BitmapPush("yes",  �ƿ�����4);

	YesComp->SetCurrentBitmap("yes");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto Yes_mouseListener = ��->AddComponent<MouseListenerComponent>(
		[YesComp](const MSG& msg) {
			YesComp->Worked(msg);
		}
	);

	YesComp->SetOnClickCallback([this]() {
		// �׷��� �ϸ� �Ǵ� ����
		//SceneManager::Get().ChangeScene("InGame");
		//std::cout << "SceneManager::Get().ChangeScene(\"InGame\");" << std::endl;
		});

	/// 9
	m_gameObjects.emplace("�ƿ�����4", std::move(��));


	/////////////////////
	/////////////////////
	/////////////////////
	// [5] �ƴϿ� ��ư/���� ��ư

	// 1. �̹��� ���� ����
	auto �ƿ�����5���� = ResourceManager::Get().GetTexture("�ƿ�����", "05����");
	auto �ƿ�����5�ƴϿ� = ResourceManager::Get().GetTexture("�ƿ�����", "05�ƴϿ�");

	// 2. ������Ʈ �����
	auto �ƴϿ� = std::make_unique<Object>();
	�ƴϿ�->SetPosition(Vec2(1296, 906));

	auto �ƴϿ�info = �ƴϿ�->GetRenderInfo();
	�ƴϿ�info->SetBitmap(�ƿ�����5����.Get());

	// 3. ��ư ������Ʈ �����
	auto NoComp = �ƴϿ�->AddComponent<ButtonComponent>(�ƴϿ�info);
	NoComp->SetWidth(�ƿ�����5�ƴϿ�->GetSize().width);
	NoComp->SetHeight(�ƿ�����5�ƴϿ�->GetSize().height);

	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	NoComp->BitmapPush("next", �ƿ�����5����);
	NoComp->BitmapPush("no", �ƿ�����5�ƴϿ�);

	//changeState()
	//NoComp->SetCurrentBitmap("no");

	// 5. ���콺 ������ ������Ʈ (��ư ������Ʈ�� ĸó�� ����)
	auto No_mouseListener = �ƴϿ�->AddComponent<MouseListenerComponent>(
		[NoComp](const MSG& msg) {
			NoComp->Worked(msg);
		}
	);

	NoComp->SetOnClickCallback([this]() {
			//changeState()
		});

	/// 9 
	m_gameObjects.emplace("�ƿ�����5", std::move(�ƴϿ�));

	

	/////////////////////
	/////////////////////
	/////////////////////
	// [6] �ڷΰ��� ��ư

	// 1. �̹��� ���� ����
	auto �ڷΰ��� = ResourceManager::Get().GetTexture("�ڷΰ���");
	// 2. ������Ʈ �����
	auto �ڷ� = std::make_unique<Object>();
	�ڷ�->SetPosition(Vec2(41, 31));

	auto �ڷ�info = �ڷ�->GetRenderInfo();
	�ڷ�info->SetBitmap(�ڷΰ���.Get());
	// 3. ��ư ������Ʈ �����
	auto backComp = �ڷ�->AddComponent<ButtonComponent>(�ڷ�info);
	backComp->SetWidth(�ڷΰ���->GetSize().width); 
	backComp->SetHeight(�ڷΰ���->GetSize().height);
	
	//  4. ��ư ��Ʈ�� ����
	// ���� �����̸� ���� �̷��� �� �ʿ� ���� ��. 
	backComp->BitmapPush("back", �ڷΰ���);

	backComp->SetCurrentBitmap("back");

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
	auto range = outGameTextTable.equal_range(m_state);

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

void Scene_Outgame::SetupCharacterAndBackground()
{	
	// ����� �ȹٲ�
	auto bg = m_gameObjects["Background"s]->GetComponent<BackgroundComponent>();
	bg->SetCurrentBitmap("Background");

	// Ű�� �̹��� ���� �ϴ� ���� "Sinclair + GameManager::Get().curGen" = Sinclair3? 2? 4?
	auto ch = m_gameObjects["Character"s]->GetComponent<BackgroundComponent>();
	std::string result = "Sinclair";
	result += std::to_string(GameManager::Get().curGen/*�갡 ���븦 ��Ÿ���� ������*/);
	ch->SetCurrentBitmap(result);
}

void Scene_Outgame::ChangeState(State newState)
{
	m_state = newState;


	// ��ư �ݹ��� �������� ���� �غ�
	auto yesButton = m_gameObjects["�ƿ�����4"]->GetComponent<ButtonComponent>();
	auto noButton = m_gameObjects["�ƿ�����5"]->GetComponent<ButtonComponent>();
	// '����' �Ǵ� ������ ��ư�鵵 ���������� �����ɴϴ�.

	switch (m_state)
	{
	case FIRST_ENTER:
	{
		curText = getRandomText();

		// '����' ��ư�� �����ְ� '��' ��ư�� ����ϴ�.
		yesButton->m_isInvisible = true;
		noButton->SetCurrentBitmap("next");

		// '����' ��ư�� �ݹ��� �����Ͽ�, ���� �ؽ�Ʈ�� �����ְų� ���¸� 80002�� �����ϵ��� �մϴ�.
		noButton->SetOnClickCallback([this]() {
			ChangeState(CHOICE_MENU);
		});
		break;
}

	case CHOICE_MENU: // �и��� ���� ����
	{
		curText = outGameTextTable.find(m_state)->second; 
		// "> â��� �̵��Ѵ�.\n> ������ ������."
		// 'â��', '����' ��ư�� �����ְ� �ݹ��� �����ؾ� �մϴ�.
		// ����� '��/�ƴϿ�' ��ư�� �����Ƿ�, �̵��� ��Ȱ���ϰų� ���� ������ �մϴ�.
		// ����: '��' ��ư�� 'â��' ��ư���� ���
		// ���� ��ư �����!
		// ��Ȱ��ȭ�� �����
		yesButton->m_isInvisible = false;
		noButton->SetCurrentBitmap("no");

		yesButton->SetOnClickCallback([this]() {
			// 3, 4����� ������ ���� �� ���ٴ� ���� ������ ���⼭ GameManager::Get().curGen���� Ȯ�� ����
			ChangeState(ENTER_OUTGAME);
			});

		// ����: '�ƴϿ�' ��ư�� '����' ��ư���� ���
		noButton->SetOnClickCallback([this]() {
			ChangeState(ENTER_END);
			});
		break;
	}

	case ENTER_OUTGAME:
	{
		curText = outGameTextTable.find(m_state)->second; // "â�� ���ðڽ��ϱ�?..."



		yesButton->SetOnClickCallback([this]() {
			cout << "â�� ���ðڽ��ϱ�?..." << endl;
			SceneManager::Get().ChangeScene("InGame");
			});

		noButton->SetOnClickCallback([this]() {
			ChangeState(CHOICE_MENU); // �������� ���ư���
			});
		break;
	}

	case ENTER_END:
	{
		curText = outGameTextTable.find(m_state)->second; // "������ �����ðڽ��ϱ�?..."


		yesButton->SetOnClickCallback([this]() {
			cout << "������ �����ðڽ��ϱ�?..." << endl;
			SceneManager::Get().ChangeScene("End");
			});

		noButton->SetOnClickCallback([this]() {
			ChangeState(CHOICE_MENU); // �������� ���ư���
			});
		break;
	}
	}
}

