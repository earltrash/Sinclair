#include "pch.h"
#include "M_Core.h"
#include "InputManager.h"
#include "Dispatcher.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "IAssetProvider.h"
#include "Renderer.h"
#include "UIManager.h"

using namespace std;


void M_Core::Init()
{
    const wchar_t* className = L"MK_00";
    const wchar_t* windowName = L"Do_Flight";

    HRESULT hr = __super::Create(className, windowName, 1920, 1080);
    assert(SUCCEEDED(hr));

    hr = ComInit(); // Com ��ü ���� 
    assert(SUCCEEDED(hr));

    InputManager::Get().SetWindowHandle(m_hWnd);

    hr = ModuleInit();     // �Ŵ����� �ʱ�ȭ�� �� �ܰ迡�� ���� : ���ҽ� load & ��� ��� �ʱ�ȭ �� ����
    assert(SUCCEEDED(hr));

    //etc
    CursorManager::Get().LoadCursorBitmaps();
    std::cout << "Init ������" << std::endl;

    // ���� Ŀ�� ������.
    ShowCursor(FALSE);
}


void M_Core::Run() //���� ���, ���ν����� InputManager�� ���� �ʿ�� ����. + ���ν����� ���� ����� ������ �־ MSG ����ü ��ü�� ������ �� ���⿡ ���⼭ Input ó��
{
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) //Ư�� �޽����� ó���ؾ� ��. 
        {
            if (!InputManager::Get().MsgCheck(msg)) //Inputmanager���� ó���ϴ� �ָ� �갡 ��, ��ǲó���� ���⼭ ��. 
            {
                TranslateMessage(&msg); //�������� ���ν�����...
                DispatchMessage(&msg);
            }
        }
        else
        {
            m_timer->Tick();
            GameLoop();
        }
    }
}




void M_Core::GameLoop() //event state function render 
{
    FixedUpdate(); //���� �� �Ǵ�ó�� 
    Update();  // event state
    Render(); //����
}


void M_Core::FixedUpdate() //�ð� ó�� 
{
    m_Scene_map->at(SceneManager::Get().GetCurrentIndex())->LogicUpdate(m_timer->DeltaTime());
}

void M_Core::Update()
{
    InputManager::Get().Update();
    m_Scene_map->at(SceneManager::Get().GetCurrentIndex())->Update();
    UIManager::Get().Update();

}

void M_Core::Render()
{
    D2DRenderer::Get().RenderBegin();
    m_Scene_map->at(SceneManager::Get().GetCurrentIndex())->Render();


    UIManager::Get().Render();
    CursorManager::Get().RenderCursor();
    D2DRenderer::Get().RenderEnd();
}

void M_Core::End()
{
    D2DRenderer::Get().Uninitialize();
    ModuleClean();

}

void M_Core::ModuleClean()
{
    SceneManager::Get().Clean();
    InputManager::Get().Clean();
    ResourceManager::Get().Clean();
}

bool M_Core::ComInit()  //DX11 ���� 
{
    D2DRenderer::Get().Initialize(m_hWnd);
    ID3D11Device* pd3dDevice = D2DRenderer::Get().GetD3DDevice();
    return true;


} //�ϴ� ���� 

bool M_Core::ModuleInit()
{

    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    std::wcout << L"[exe ��ġ] " << exePath << std::endl;


    InputManager::Get().m_broadcaster = make_unique<EventDispatcher>(); //�̰� ����� �ϳ� ������ 
    if (!InputManager::Get().m_broadcaster)
    {
        std::cout << "broadcaster �Ҵ� �ȵ�" << std::endl;
        return false;
    }

   
    ResourceManager::Get().GameAssetLoad();
    m_Scene_map = make_shared<unordered_map<string, shared_ptr<SceneStandard>>>();  //Core�� UPdate�� ������ �ϴ�
    SceneManager::Get().Initalize(m_Scene_map); //���� map ����� ���� 

    UIManager::Get().Initialize(); //UI �� �����ϱ�. 


    m_timer = make_unique<GameTimer>();
    m_timer->Start();
    return true;
}







