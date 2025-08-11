#include "SoundBank.h"

#include "json.hpp"
#include <fstream>       // ifstream
#include <filesystem>    // std::filesystem

void SoundBank::SoundAssetLoad(string path) //Sound 로 고정 
{
    namespace fs = std::filesystem;
    fs::path base = fs::current_path();
    fs::path resourceFolder = base.parent_path() / path;

    FMOD_SYSTEM* g_fmodSystem = SoundManager::Instance().GetSystem();


    for (const auto& entry : fs::directory_iterator(resourceFolder))
    {
        if (!entry.is_regular_file())
            continue;

        std::string filename = entry.path().filename().string();
        std::string name = filename.substr(0, filename.find_last_of('.')); // 확장자 제거
        std::wstring fullPathW = entry.path().wstring();
        std::string fullPath(fullPathW.begin(), fullPathW.end()); // FMOD는 UTF-8 필요

        FMOD_SOUND* sound = nullptr;
        FMOD_RESULT result = FMOD_System_CreateSound(
            g_fmodSystem,
            fullPath.c_str(),
            FMOD_DEFAULT,
            nullptr,
            &sound
        );

        if (result != FMOD_OK)
        {
           // printf("FMOD sound load error: %s\n", FMOD_ErrorString(result));
            continue;
        }

        auto splitPos = name.find('_');
        if (splitPos == std::string::npos) // Single
        {
            m_BG.emplace(name, sound);
        }
        else
        {
            std::string prefix = name.substr(0, splitPos);
            std::string suffix = name.substr(splitPos + 1);

            if (prefix == "EDM") {
                m_EDM.emplace(suffix, sound);
            }
            else if (prefix == "EUIM") {
                m_EUIM.emplace(suffix, sound);
            }
            else if (prefix == "HM") {
                m_HM.emplace(suffix, sound);
            }
        }
    }


}

FMOD_SOUND* SoundBank::GetEndingBGM( string& id)
{
    return m_EDM[id];
}

FMOD_SOUND* SoundBank::GetUIBGM( string& id)
{
    return m_EUIM[id];
}

FMOD_SOUND* SoundBank::GetBGM( string& id)
{
    return m_BG[id];
}

FMOD_SOUND* SoundBank::GetHistoryBGM( string& id)
{
    return m_HM[id];
}
