#pragma once
#include "pch.h"
#include "SoundManager.h"

#include <string>
#include <unordered_map>

class SoundBank
{
public:
    SoundBank() = default;
    ~SoundBank() = default;

    void SoundAssetLoad(const std::string& path);

    FMOD_SOUND* GetEndingBGM(const std::string& id);
    FMOD_SOUND* GetUIBGM(const std::string& id);
    FMOD_SOUND* GetBGM(const std::string& id);
    FMOD_SOUND* GetHistoryBGM(const std::string& id);
    FMOD_SOUND* GetESBGM(const std::string& id);

    std::unordered_map<std::string, FMOD_SOUND*> Get_BGM_MAP();
    std::unordered_map<std::string, FMOD_SOUND*> Get_EDM_MAP();
    std::unordered_map<std::string, FMOD_SOUND*> Get_EUIM_MAP();
    std::unordered_map<std::string, FMOD_SOUND*> Get_HM_MAP();
    std::unordered_map<std::string, FMOD_SOUND*> Get_ES_MAP();


private:
    std::unordered_map<std::string, FMOD_SOUND*> m_BG;
    std::unordered_map<std::string, FMOD_SOUND*> m_EDM;
    std::unordered_map<std::string, FMOD_SOUND*> m_EUIM;
    std::unordered_map<std::string, FMOD_SOUND*> m_HM;
    std::unordered_map<std::string, FMOD_SOUND*> m_ES;
};