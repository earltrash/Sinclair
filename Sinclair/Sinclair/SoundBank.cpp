#include "SoundBank.h"

#include "json.hpp"
#include <fstream>       // ifstream
#include <filesystem>    // std::filesystem

void SoundBank::SoundAssetLoad(string path) //Sound �� ���� 
{
    //namespace fs = std::filesystem;
    //fs::path base = fs::current_path();
    //fs::path resourceFolder = base.parent_path() / path;



    //for (const auto& entry : fs::directory_iterator(resourceFolder))
    //{
    //    if (!entry.is_regular_file())
    //        continue;

    //    std::string filename = entry.path().filename().string();


    //    // Ȯ���� üũ (lowercase ��)
    //    if (entry.path().extension() != ".png")
    //        continue;

    //    std::string name = filename.substr(0, filename.find_last_of('.')); // . ���� 

    //    //���⼭ �бⰡ ����.  Single���� / �ƴ� Multi���� 


    //    auto splitPos = name.find('_');
    //    if (splitPos == std::string::npos) //���� �׳� Single�̾� 
    //    {
    //        std::wstring fullPath = entry.path().wstring(); //�̷��ŷ� ���� �����ؼ� emplace �ϱ� 

    //        m_BG.emplace(splitPos, )//�� �����̰��� //���� 
    //    }
    //    else
    //    {
 
    //        std::string prefix = name.substr(0, splitPos);        // _ �պκ�
    //        std::string suffix = name.substr(splitPos + 1);       // _ �޺κ�
    //        std::wstring fullPath = entry.path().wstring();

    //        if (prefix == "EDM")
    //        {
    //            m_EDM.emplace(suffix, );

    //        }
    //        else if (prefix == "EUIM")
    //        {
    //            m_EUIM.emplace(suffix, );
    //        }
    //        else if (prefix == "HM")
    //        {
    //            m_HM.emplace(suffix, );
    //        }
    //       
    //       //�� ���� �޾ƿͼ� �����ϸ� �� 
    //        




    //    }
    //}



}
