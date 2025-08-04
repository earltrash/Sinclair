#pragma once
#include "pch.h"
#include <filesystem>
#include "Object.h"

using namespace std;
using Microsoft::WRL::ComPtr;
namespace fs = std::filesystem;

class UI_Bank
{
public:
	UI_Bank() = default;
	~UI_Bank() = default;

public:
	void Load_UI_Image(const string& path);

	void UI_Image_Mapping(const Object& UI_Name, const string& name); //그냥 오브젝트 이름만 대조해서 던져주고, Default인 애들은 현재 렌더 되어야 하는 비트맵으로 설정 

	ComPtr<ID2D1Bitmap1> Get_Image(const string& Nm);

	ComPtr<ID2D1Bitmap1> Get_Image(const string& Nm, const string& Nm2);

private:
	unordered_map<std::string, std::unordered_map<std::string, ComPtr<ID2D1Bitmap1>>> UI_MultiBitmaps; //Button 전용 

	unordered_map<std::string, ComPtr<ID2D1Bitmap1>> UI_SingleBitmaps; //UI_Renderer 전용 
	//Obj/State/Bitmap 
};