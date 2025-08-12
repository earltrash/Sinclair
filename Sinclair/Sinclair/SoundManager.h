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

    // 복사/이동 방지
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
    SoundManager(SoundManager&&) = delete;
    SoundManager& operator=(SoundManager&&) = delete;

private:
    SoundManager() = default;
    ~SoundManager();
};
