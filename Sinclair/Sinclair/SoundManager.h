#pragma once
#include "pch.h"


#include "fmod.h"
#include <vector>
#include <string>
#include <algorithm>



struct CrossFade
{
    bool   crossfadeActive = false;
    int    crossFrom = -1;
    int    crossTo = -1;
    float  crossFadeTime = 0.f;
    float  crossElapsed = 0.f;
    float  crossFromStartVol = 1.f;
    float  crossToStartVol = 0.f;
};
class SoundManager
{
private:
    FMOD_SYSTEM* system = nullptr;
    std::vector<FMOD_SOUND*> bgms;
    std::vector<FMOD_CHANNEL*> bgmChannels;
    std::vector<FMOD_SOUND*> sfx;
    std::vector<FMOD_CHANNEL*> activeSfxChannels;
    bool initialized = false;

    // 마스터 볼륨
    float m_masterBGMVolume = 1.0f;
    float m_masterSFXVolume = 1.0f;

public:
    static SoundManager& Instance();

    // 초기화 및 리소스 로드
    FMOD_RESULT Init();
    FMOD_RESULT LoadBGM(const std::string& path);
    FMOD_RESULT LoadSFX(const std::string& path);
    void TakeAllClip();

    // 사운드 재생
    FMOD_RESULT PlayBGM(int index, bool fadeIn = false);
    FMOD_RESULT PlaySFX(int index, float volume = 1.0f);
    FMOD_RESULT PlayBGM(const std::string& key, bool fadeIn = false);
    FMOD_RESULT PlaySFX(const std::string& key, float volume = 1.0f);

    // 사운드 제어
    FMOD_RESULT StopBGM(int index);
    FMOD_RESULT StopSFX(int index);
    FMOD_RESULT SetBGMVolume(int index, float volume);
    FMOD_RESULT PauseBGM(int index, bool pause);
    FMOD_RESULT PauseSFX(int index, bool pause);
    FMOD_RESULT PauseBGM(const std::string& key, bool pause);
    FMOD_RESULT PauseSFX(const std::string& key, bool pause);
    FMOD_RESULT CrossfadeBGM(const std::string& fromKey, const std::string& toKey, float fadeTime);

    // 새로운 마스터 볼륨 제어 함수
    void SetMasterBGMVolume(float volume);
    void SetMasterSFXVolume(float volume);
    float GetMasterBGMVolume() const;
    float GetMasterSFXVolume() const;

    // 시스템 업데이트 및 종료
    FMOD_RESULT Update();
    void Shutdown();
    void CleanupFinishedSFX();

    // Getter 함수
    FMOD_SYSTEM* GetSystem() const;
    bool IsInitialized() const;
    int GetBGMCount() const;
    int GetSFXCount() const;
    std::string Get_Playing_Key() { return Playing_key; }

    // 키 기반 관리
    std::string Playing_key;
    void Set_Playing_Key(std::string key) { Playing_key = key; }
    FMOD_RESULT AddBGM(const std::string& key, FMOD_SOUND* sound);
    FMOD_RESULT AddSFX(const std::string& key, FMOD_SOUND* sound);
    std::unordered_map<std::string, int> bgmKeyMap;
    std::unordered_map<std::string, int> sfxKeyMap;

    // 복사/이동 방지
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
    SoundManager(SoundManager&&) = delete;
    SoundManager& operator=(SoundManager&&) = delete;

private:
    SoundManager() = default;
    ~SoundManager();

    CrossFade m_crossfade;
};