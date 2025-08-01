#include "UI_Bank.h"
#include "Renderer.h"
#include "UI_Renderer.h"
#include "ButtonComponent.h"

void UI_Bank::GetUI_Image(const string& path) // 자동 매핑이긴 함. 
{
    for (const auto& entry : fs::directory_iterator(path))
    {
        if (!entry.is_regular_file())
            continue;

        std::string filename = entry.path().filename().string();

        // 확장자 체크 (lowercase 비교)
        if (entry.path().extension() != ".png")
            continue;
        std::string name = filename.substr(0, filename.find_last_of('.'));

        //여기서 분기가 갈림.  Single인지 / 아님 Multi인지 


        auto splitPos = name.find('_');
        if (splitPos == std::string::npos) //나는 그냥 Single이야 
        {
            std::wstring fullPath = entry.path().wstring();
            ComPtr<ID2D1Bitmap1> newBitmap;
            D2DRenderer::Get().CreateBitmapFromFile(fullPath.c_str(), newBitmap.GetAddressOf());
            UI_SingleBitmaps.emplace(name, std::move(newBitmap));

        }
        else
        {
            std::string objectName = name.substr(0, splitPos); //string 특정 idnex까지 읽는다는 뜻 /substract string 
            std::string bitmapName = name.substr(splitPos + 1);
            std::wstring fullPath = entry.path().wstring(); // 전체 경로 (for loading)

            ComPtr<ID2D1Bitmap1> newBitmap;
            D2DRenderer::Get().CreateBitmapFromFile(fullPath.c_str(), newBitmap.GetAddressOf());
    

            if (newBitmap)
                UI_MultiBitmaps[objectName][bitmapName] = newBitmap;

            else
            {
                std::cout << "UI BANK Bitmap 생성 오류" << objectName << " : " << bitmapName << endl;
            }
        }
    }
}

void UI_Bank::UI_Image_Mapping(const Object& UI_Obj, const string& name) //STATE는 입력 상태에 따른 Bitmap 정보긴 함. 
{
    if (UI_Obj.GetComponent<UI_Renderer>() != nullptr) //Single
    {
        UI_Obj.GetComponent<UI_Renderer>()->SetBitmap(UI_SingleBitmaps[name]);
    }

    else if (UI_Obj.GetComponent<ButtonComponent>() != nullptr)
    {
        for (auto& [ObjNm, bitmapmap] : UI_MultiBitmaps)
        {
            if (ObjNm != name)
                return;
            for (auto& [BitmapNM, bitmap_asset] : bitmapmap)
            {
                UI_Obj.GetComponent<ButtonComponent>()->BitmapPush(BitmapNM, bitmap_asset);
            }
        }
        UI_Obj.GetComponent<ButtonComponent>()->SetCurrentBitmap("default");
    }
}
