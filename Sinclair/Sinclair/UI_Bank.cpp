#include "UI_Bank.h"
#include "Renderer.h"
#include "UI_Renderer.h"
#include "ButtonComponent.h"

void UI_Bank::Load_UI_Image(const string& path) // �ڵ� �����̱� ��. 
{

    fs::path base = fs::current_path();
    //fs::path resourcePath = base.parent_path() / path;

    fs::path resourcePath = base.parent_path() / path; //path == "UI" -> 


    for (const auto& entry : fs::directory_iterator(resourcePath))
    {
        if (!entry.is_regular_file())
            continue;

        std::string filename = entry.path().filename().string();

        // Ȯ���� üũ (lowercase ��)
        if (entry.path().extension() != ".png")
            continue;
        std::string name = filename.substr(0, filename.find_last_of('.'));

        //���⼭ �бⰡ ����.  Single���� / �ƴ� Multi���� 


        auto splitPos = name.find('_');
        if (splitPos == std::string::npos) //���� �׳� Single�̾� 
        {
            std::wstring fullPath = entry.path().wstring();
            ComPtr<ID2D1Bitmap1> newBitmap;
            D2DRenderer::Get().CreateBitmapFromFile(fullPath.c_str(), newBitmap.GetAddressOf());
            UI_SingleBitmaps.emplace(name, std::move(newBitmap));

        }
        else
        {
            std::string objectName = name.substr(0, splitPos); //string Ư�� idnex���� �д´ٴ� �� /substract string 
            std::string bitmapName = name.substr(splitPos + 1);
            std::wstring fullPath = entry.path().wstring(); // ��ü ��� (for loading)

            ComPtr<ID2D1Bitmap1> newBitmap;
            D2DRenderer::Get().CreateBitmapFromFile(fullPath.c_str(), newBitmap.GetAddressOf());
    

            if (newBitmap)
                UI_MultiBitmaps[objectName][bitmapName] = newBitmap;

            else
            {
                std::cout << "UI BANK Bitmap ���� ����" << objectName << " : " << bitmapName << endl;
            }
        }
    }
}


//Object�� �̸����� ��

//���߿� Scene ����� ����, ������Ʈ �̸��̶�, Bitmap �̸� mapping �ϸ� �Ǳ� ��. 



void UI_Bank::UI_Image_Mapping(unique_ptr<Object>& UI_Obj, const string& name) //STATE�� �Է� ���¿� ���� Bitmap ������ ��. 
{
    if (UI_Obj.get()->GetComponent<UI_Renderer>() != nullptr) //Single
    {
        UI_Obj.get()->GetComponent<UI_Renderer>()->SetBitmap(UI_SingleBitmaps[name]);
    }

    else if (UI_Obj.get()->GetComponent<ButtonComponent>() != nullptr)
    {
        for (auto& [ObjNm, bitmapmap] : UI_MultiBitmaps) // 
        {
            if (ObjNm != name)
                return;
            for (auto& [BitmapNM, bitmap_asset] : bitmapmap)
            {
                UI_Obj.get()->GetComponent<ButtonComponent>()->BitmapPush(BitmapNM, bitmap_asset); //ȿ���� �������� �ϱ� ����. 
            }
        }
        UI_Obj.get()->GetComponent<ButtonComponent>()->SetCurrentBitmap("normal"); //ȿ�� �������� �ٲ� 
    }
}

ComPtr<ID2D1Bitmap1> UI_Bank::Get_Image(const string& Nm)
{
    if (UI_SingleBitmaps.find(Nm) != UI_SingleBitmaps.end())
    return UI_SingleBitmaps[Nm];

    else
        return nullptr;
}


ComPtr<ID2D1Bitmap1> UI_Bank::Get_Image(const string& Nm, const string& Nm2)
{
    auto stringIt = UI_MultiBitmaps.find(Nm);
    if (stringIt != UI_MultiBitmaps.end())
    {
        auto bitmapIt = stringIt->second.find(Nm2);
        if (bitmapIt != stringIt->second.end())
        {
            return bitmapIt->second;
        }
    }
    return nullptr;
}