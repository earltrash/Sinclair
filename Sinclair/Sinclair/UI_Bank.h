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

	void UI_Image_Mapping(const Object& UI_Name, const string& name); //�׳� ������Ʈ �̸��� �����ؼ� �����ְ�, Default�� �ֵ��� ���� ���� �Ǿ�� �ϴ� ��Ʈ������ ���� 

	ComPtr<ID2D1Bitmap1> Get_Image(const string& Nm);

	ComPtr<ID2D1Bitmap1> Get_Image(const string& Nm, const string& Nm2);

private:
	unordered_map<std::string, std::unordered_map<std::string, ComPtr<ID2D1Bitmap1>>> UI_MultiBitmaps; //Button ���� 

	unordered_map<std::string, ComPtr<ID2D1Bitmap1>> UI_SingleBitmaps; //UI_Renderer ���� 
	//Obj/State/Bitmap 
};