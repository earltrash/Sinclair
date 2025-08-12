#include "SoundManager.h"
/*



SoundManager& SoundManager::Instance()
{
    static SoundManager instance;
    return instance;
}

FMOD_RESULT SoundManager::Init()
{
    if (initialized) return FMOD_OK;

    FMOD_RESULT result = FMOD_System_Create(&system, FMOD_VERSION);
    if (result != FMOD_OK) return result;

    result = FMOD_System_Init(system, 64, FMOD_INIT_NORMAL, nullptr);
    if (result == FMOD_OK) {
        initialized = true;
    }
    return result;
}

FMOD_RESULT SoundManager::LoadBGM(const std::string& path)
{
    if (!initialized) return FMOD_ERR_UNINITIALIZED;

    FMOD_SOUND* sound;
    FMOD_RESULT result = FMOD_System_CreateSound(system, path.c_str(),
        FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);

    if (result == FMOD_OK) {
        bgms.push_back(sound);
        bgmChannels.push_back(nullptr);
    }
    return result;
}

FMOD_RESULT SoundManager::LoadSFX(const std::string& path)
{
    if (!initialized) return FMOD_ERR_UNINITIALIZED;

    FMOD_SOUND* sound;
    FMOD_RESULT result = FMOD_System_CreateSound(system, path.c_str(),
        FMOD_DEFAULT | FMOD_2D, 0, &sound);

    if (result == FMOD_OK) {
        sfx.push_back(sound);
    }
    return result;
}

FMOD_RESULT SoundManager::PlayBGM(int index, bool fadeIn)
{
    if (!initialized || index < 0 || index >= bgms.size())
        return FMOD_ERR_INVALID_PARAM;

    if (bgmChannels[index]) {
        FMOD_BOOL isPlaying;
        FMOD_Channel_IsPlaying(bgmChannels[index], &isPlaying);
        if (isPlaying) {
            FMOD_Channel_Stop(bgmChannels[index]);
        }
    }

    FMOD_RESULT result = FMOD_System_PlaySound(system, bgms[index],
        0, false, &bgmChannels[index]);

    if (result == FMOD_OK && fadeIn) {
        FMOD_Channel_SetVolume(bgmChannels[index], 0.0f);
    }

    return result;
}

FMOD_RESULT SoundManager::PlaySFX(int index, float volume)
{
    if (!initialized || index < 0 || index >= sfx.size())
        return FMOD_ERR_INVALID_PARAM;

    FMOD_CHANNEL* channel = nullptr;
    FMOD_RESULT result = FMOD_System_PlaySound(system, sfx[index],
        0, false, &channel);

    if (result == FMOD_OK) {
        FMOD_Channel_SetVolume(channel, volume);
        activeSfxChannels.push_back(channel);
    }

    return result;
}

FMOD_RESULT SoundManager::StopBGM(int index)
{
    if (!initialized || index < 0 || index >= bgmChannels.size())
        return FMOD_ERR_INVALID_PARAM;

    if (bgmChannels[index]) {
        FMOD_RESULT result = FMOD_Channel_Stop(bgmChannels[index]);
        bgmChannels[index] = nullptr;
        return result;
    }
    return FMOD_OK;
}

FMOD_RESULT SoundManager::SetBGMVolume(int index, float volume)
{
    if (!initialized || index < 0 || index >= bgmChannels.size())
        return FMOD_ERR_INVALID_PARAM;

    if (bgmChannels[index]) {
        return FMOD_Channel_SetVolume(bgmChannels[index], volume);
    }
    return FMOD_ERR_INVALID_HANDLE;
}

FMOD_RESULT SoundManager::PauseBGM(int index, bool pause)
{
    if (!initialized || index < 0 || index >= bgmChannels.size())
        return FMOD_ERR_INVALID_PARAM;

    if (bgmChannels[index]) {
        return FMOD_Channel_SetPaused(bgmChannels[index], pause);
    }
    return FMOD_ERR_INVALID_HANDLE;
}

    void SoundManager::CleanupFinishedSFX()
    {
        activeSfxChannels.erase(
            std::remove_if(activeSfxChannels.begin(), activeSfxChannels.end(),
                [](FMOD_CHANNEL* channel) {
                    FMOD_BOOL isPlaying;
                    FMOD_RESULT result = FMOD_Channel_IsPlaying(channel, &isPlaying);
                    return (result != FMOD_OK || !isPlaying);
                }),
            activeSfxChannels.end()
        );
    }

    FMOD_RESULT SoundManager::Update()
    {
        if (!initialized) return FMOD_ERR_UNINITIALIZED;

        CleanupFinishedSFX();  // 매 업데이트마다 정리
        return FMOD_System_Update(system);
    }

    void SoundManager::Shutdown()
    {
        if (!initialized) return;

        // 모든 채널 정지
        for (auto& channel : bgmChannels) {
            if (channel) FMOD_Channel_Stop(channel);
        }
        for (auto& channel : activeSfxChannels) {
            if (channel) FMOD_Channel_Stop(channel);
        }

        // 사운드 해제
        for (auto sound : bgms) {
            if (sound) FMOD_Sound_Release(sound);
        }
        for (auto sound : sfx) {
            if (sound) FMOD_Sound_Release(sound);
        }

        // 시스템 정리
        if (system) {
            FMOD_System_Close(system);
            FMOD_System_Release(system);
            system = nullptr;
        }

        // 컨테이너 정리
        bgms.clear();
        bgmChannels.clear();
        sfx.clear();
        activeSfxChannels.clear();

        initialized = false;
    }

    FMOD_SYSTEM* SoundManager::GetSystem() const
    {
        return system;
    }

    bool SoundManager::IsInitialized() const
    {
        return initialized;
    }

    int SoundManager::GetBGMCount() const
    {
        return static_cast<int>(bgms.size());
    }

    int SoundManager::GetSFXCount() const
    {
        return static_cast<int>(sfx.size());
    }

    SoundManager::~SoundManager()
    {
        Shutdown();
    }
*/