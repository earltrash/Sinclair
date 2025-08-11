#include "SoundBank.h"

#include "json.hpp"
#include <fstream>       // ifstream
#include <filesystem>    // std::filesystem

void SoundBank::SoundAssetLoad(string path) //Sound 로 고정 
{
    //namespace fs = std::filesystem;
    //fs::path base = fs::current_path();
    //fs::path resourceFolder = base.parent_path() / path;



    //for (const auto& entry : fs::directory_iterator(resourceFolder))
    //{
    //    if (!entry.is_regular_file())
    //        continue;

    //    std::string filename = entry.path().filename().string();


    //    // 확장자 체크 (lowercase 비교)
    //    if (entry.path().extension() != ".png")
    //        continue;

    //    std::string name = filename.substr(0, filename.find_last_of('.')); // . 빼고 

    //    //여기서 분기가 갈림.  Single인지 / 아님 Multi인지 


    //    auto splitPos = name.find('_');
    //    if (splitPos == std::string::npos) //나는 그냥 Single이야 
    //    {
    //        std::wstring fullPath = entry.path().wstring(); //이런거로 파일 접근해서 emplace 하기 

    //        m_BG.emplace(splitPos, )//뭐 파일이겠죠 //파일 
    //    }
    //    else
    //    {
 
    //        std::string prefix = name.substr(0, splitPos);        // _ 앞부분
    //        std::string suffix = name.substr(splitPos + 1);       // _ 뒷부분
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
    //       //뭐 파일 받아와서 저장하면 됨 
    //        




    //    }
    //}



}
