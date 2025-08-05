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

    hr = ComInit(); // Com 객체 생성 
    assert(SUCCEEDED(hr));

    InputManager::Get().SetWindowHandle(m_hWnd);

    hr = ModuleInit();     // 매니저의 초기화를 이 단계에서 실행 : 리소스 load & 모듈 멤버 초기화 및 생성
    assert(SUCCEEDED(hr));

    //etc
    CursorManager::Get().LoadCursorBitmaps();
    std::cout << "Init 성공적" << std::endl;

    // 원래 커서 숨기자.
    ShowCursor(FALSE);
}


void M_Core::Run() //연구 결과, 프로시저에 InputManager를 넣을 필요는 없음. + 프로시저의 생성 양식은 정해져 있어서 MSG 구조체 전체를 보내줄 수 없기에 여기서 Input 처리
{
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) //특정 메시지만 처리해야 함. 
        {
            if (!InputManager::Get().MsgCheck(msg)) //Inputmanager에서 처리하는 애면 얘가 함, 인풋처리는 여기서 함. 
            {
                TranslateMessage(&msg); //나머지는 프로시저가...
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
    FixedUpdate(); //물리 및 판단처리 
    Update();  // event state
    Render(); //렌더
}


void M_Core::FixedUpdate() //시간 처리 
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

bool M_Core::ComInit()  //DX11 기준 
{
    D2DRenderer::Get().Initialize(m_hWnd);
    ID3D11Device* pd3dDevice = D2DRenderer::Get().GetD3DDevice();
    return true;


} //일단 보류 

bool M_Core::ModuleInit()
{

    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    std::wcout << L"[exe 위치] " << exePath << std::endl;


    InputManager::Get().m_broadcaster = make_unique<EventDispatcher>(); //이거 디버깅 하나 만들자 
    if (!InputManager::Get().m_broadcaster)
    {
        std::cout << "broadcaster 할당 안됨" << std::endl;
        return false;
    }

   
    ResourceManager::Get().GameAssetLoad();
    m_Scene_map = make_shared<unordered_map<string, shared_ptr<SceneStandard>>>();  //Core가 UPdate로 돌려야 하니
    SceneManager::Get().Initalize(m_Scene_map); //받은 map 멤버로 시작 

    UIManager::Get().Initialize(); //UI 들 생성하기. 


    m_timer = make_unique<GameTimer>();
    m_timer->Start();
    return true;
}







