#pragma once
#include "pch.h"


#include "fmod.h"
#include <vector>
#include <string>
#include <algorithm>

class SoundManager
{
private:
    FMOD_SYSTEM* system = nullptr;
    std::vector<FMOD_SOUND*> bgms;
    std::vector<FMOD_CHANNEL*> bgmChannels;
    std::vector<FMOD_SOUND*> sfx;
    std::vector<FMOD_CHANNEL*> activeSfxChannels;  // 활성 SFX 채널만 관리
    bool initialized = false;

public:
    static SoundManager& Instance();

    FMOD_RESULT Init();
    FMOD_RESULT LoadBGM(const std::string& path);
    FMOD_RESULT LoadSFX(const std::string& path);


    FMOD_RESULT PlayBGM(int index, bool fadeIn = false);
    FMOD_RESULT PlaySFX(int index, float volume = 1.0f);


    FMOD_RESULT StopBGM(int index);
    FMOD_RESULT SetBGMVolume(int index, float volume);
    FMOD_RESULT PauseBGM(int index, bool pause);

    void CleanupFinishedSFX();
    FMOD_RESULT Update();
    void Shutdown();

    FMOD_SYSTEM* GetSystem() const;
    bool IsInitialized() const;

    int GetBGMCount() const;
    int GetSFXCount() const;

    FMOD_RESULT AddBGM(const std::string& key, FMOD_SOUND* sound);
    FMOD_RESULT AddSFX(const std::string& key, FMOD_SOUND* sound);


    FMOD_RESULT PlayBGM(const std::string& key, bool fadeIn = false);
    FMOD_RESULT PlaySFX(const std::string& key, float volume = 1.0f);

    FMOD_RESULT PauseBGM(const std::string& key, bool pause);
    FMOD_RESULT PauseSFX(const std::string& key, bool pause);
    FMOD_RESULT PauseSFX(int index, bool pause);


    // 복사/이동 방지
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
    SoundManager(SoundManager&&) = delete;
    SoundManager& operator=(SoundManager&&) = delete;
    void TakeAllClip();

private:
    SoundManager() = default;
    ~SoundManager();

    std::unordered_map<std::string, int> bgmKeyMap;
    std::unordered_map<std::string, int> sfxKeyMap;
    FMOD_RESULT CrossfadeBGM(const std::string& fromKey, const std::string& toKey, float fadeTime);

   



  
};
