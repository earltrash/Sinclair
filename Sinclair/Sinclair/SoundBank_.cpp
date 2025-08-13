#include "SoundBank_.h"
#include "json.hpp"
#include <fstream>
#include <filesystem>

void SoundBank::SoundAssetLoad(const std::string& path)
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
       // std::cout << entry << std::endl;
        FMOD_SOUND* sound = nullptr;
        
        FMOD_RESULT result = FMOD_System_CreateSound(
            g_fmodSystem,
            fullPath.c_str(),
            FMOD_DEFAULT, // <- 추가 
            nullptr,
            &sound
        );

        if (result != FMOD_OK)
        {
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
            else if (prefix == "ES") {
                m_ES.emplace(suffix, sound);
            }
        }
    }

}

FMOD_SOUND* SoundBank::GetEndingBGM(const std::string& id)
{
    auto it = m_EDM.find(id);
    return it != m_EDM.end() ? it->second : nullptr;
}

FMOD_SOUND* SoundBank::GetUIBGM(const std::string& id)
{
    auto it = m_EUIM.find(id);
    return it != m_EUIM.end() ? it->second : nullptr;
}

FMOD_SOUND* SoundBank::GetBGM(const std::string& id)
{
    auto it = m_BG.find(id);
    return it != m_BG.end() ? it->second : nullptr;
}

FMOD_SOUND* SoundBank::GetHistoryBGM(const std::string& id)
{
    auto it = m_HM.find(id);
    return it != m_HM.end() ? it->second : nullptr;
}

FMOD_SOUND* SoundBank::GetESBGM(const std::string& id)
{
    auto it = m_ES.find(id);
    return it != m_ES.end() ? it->second : nullptr;
}

std::unordered_map<std::string, FMOD_SOUND*> SoundBank::Get_BGM_MAP()
{
    return m_BG;
}

std::unordered_map<std::string, FMOD_SOUND*> SoundBank::Get_EDM_MAP()
{
    return m_EDM;
}

std::unordered_map<std::string, FMOD_SOUND*> SoundBank::Get_EUIM_MAP()
{
    return m_EUIM;
}

std::unordered_map<std::string, FMOD_SOUND*> SoundBank::Get_HM_MAP()
{
    return m_HM;
}

std::unordered_map<std::string, FMOD_SOUND*> SoundBank::Get_ES_MAP()
{
    return m_ES;
}
