#include "pch.h"
//#include "fmod.h"


class SoundManager {
private:
//    FMOD_SYSTEM* system = nullptr;
//    std::vector<FMOD_SOUND*> bgms;
//    std::vector<FMOD_CHANNEL*> bgmChannels;
//    std::vector<FMOD_SOUND*> sfx;
//    std::vector<FMOD_CHANNEL*> activeSfxChannels;  // Ȱ�� SFX ä�θ� ����
//    bool initialized = false;
//
//public:
//    static SoundManager& Instance() {
//        static SoundManager instance;
//        return instance;
//    }
//
//    FMOD_RESULT Init() {
//        if (initialized) return FMOD_OK;
//
//        FMOD_RESULT result = FMOD_System_Create(&system, FMOD_VERSION);
//        if (result != FMOD_OK) return result;
//
//        result = FMOD_System_Init(system, 64, FMOD_INIT_NORMAL, nullptr);
//        if (result == FMOD_OK) {
//            initialized = true;
//        }
//        return result;
//    }
//
//    FMOD_RESULT LoadBGM(const std::string& path) {
//        if (!initialized) return FMOD_ERR_UNINITIALIZED;
//
//        FMOD_SOUND* sound;
//        FMOD_RESULT result = FMOD_System_CreateSound(system, path.c_str(),
//            FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
//
//        if (result == FMOD_OK) {
//            bgms.push_back(sound);
//            bgmChannels.push_back(nullptr);
//        }
//        return result;
//    }
//
//    FMOD_RESULT LoadSFX(const std::string& path) {
//        if (!initialized) return FMOD_ERR_UNINITIALIZED;
//
//        FMOD_SOUND* sound;
//        FMOD_RESULT result = FMOD_System_CreateSound(system, path.c_str(),
//            FMOD_DEFAULT | FMOD_2D, 0, &sound);
//
//        if (result == FMOD_OK) {
//            sfx.push_back(sound);
//        }
//        return result;
//    }
//
//    FMOD_RESULT PlayBGM(int index, bool fadeIn = false) {
//        if (!initialized || index < 0 || index >= bgms.size())
//            return FMOD_ERR_INVALID_PARAM;
//
//        // ���� BGM ����
//        if (bgmChannels[index]) {
//            FMOD_BOOL isPlaying;
//            FMOD_Channel_IsPlaying(bgmChannels[index], &isPlaying);
//            if (isPlaying) {
//                FMOD_Channel_Stop(bgmChannels[index]);
//            }
//        }
//
//        FMOD_RESULT result = FMOD_System_PlaySound(system, bgms[index],
//            0, false, &bgmChannels[index]);
//
//        if (result == FMOD_OK && fadeIn) {
//            FMOD_Channel_SetVolume(bgmChannels[index], 0.0f);
//            // ���̵��� ������ ���� �Լ��� �ܺο��� ó��
//        }
//
//        return result;
//    }
//
//    FMOD_RESULT PlaySFX(int index, float volume = 1.0f) {
//        if (!initialized || index < 0 || index >= sfx.size())
//            return FMOD_ERR_INVALID_PARAM;
//
//        FMOD_CHANNEL* channel = nullptr;
//        FMOD_RESULT result = FMOD_System_PlaySound(system, sfx[index],
//            0, false, &channel);
//
//        if (result == FMOD_OK) {
//            FMOD_Channel_SetVolume(channel, volume);
//            activeSfxChannels.push_back(channel);
//        }
//
//        return result;
//    }
//
//    FMOD_RESULT StopBGM(int index) {
//        if (!initialized || index < 0 || index >= bgmChannels.size())
//            return FMOD_ERR_INVALID_PARAM;
//
//        if (bgmChannels[index]) {
//            FMOD_RESULT result = FMOD_Channel_Stop(bgmChannels[index]);
//            bgmChannels[index] = nullptr;
//            return result;
//        }
//        return FMOD_OK;
//    }
//
//    FMOD_RESULT SetBGMVolume(int index, float volume) {
//        if (!initialized || index < 0 || index >= bgmChannels.size())
//            return FMOD_ERR_INVALID_PARAM;
//
//        if (bgmChannels[index]) {
//            return FMOD_Channel_SetVolume(bgmChannels[index], volume);
//        }
//        return FMOD_ERR_INVALID_HANDLE;
//    }
//
//    FMOD_RESULT PauseBGM(int index, bool pause) {
//        if (!initialized || index < 0 || index >= bgmChannels.size())
//            return FMOD_ERR_INVALID_PARAM;
//
//        if (bgmChannels[index]) {
//            return FMOD_Channel_SetPaused(bgmChannels[index], pause);
//        }
//        return FMOD_ERR_INVALID_HANDLE;
//    }
//
//    void CleanupFinishedSFX() {
//        activeSfxChannels.erase(
//            std::remove_if(activeSfxChannels.begin(), activeSfxChannels.end(),
//                [](FMOD_CHANNEL* channel) {
//                    FMOD_BOOL isPlaying;
//                    FMOD_RESULT result = FMOD_Channel_IsPlaying(channel, &isPlaying);
//                    return (result != FMOD_OK || !isPlaying);
//                }),
//            activeSfxChannels.end()
//        );
//    }
//
//    FMOD_RESULT Update() {
//        if (!initialized) return FMOD_ERR_UNINITIALIZED;
//
//        CleanupFinishedSFX();  // �� ������Ʈ���� ����
//        return FMOD_System_Update(system);
//    }
//
//    void Shutdown() {
//        if (!initialized) return;
//
//        // ��� ä�� ����
//        for (auto& channel : bgmChannels) {
//            if (channel) FMOD_Channel_Stop(channel);
//        }
//        for (auto& channel : activeSfxChannels) {
//            if (channel) FMOD_Channel_Stop(channel);
//        }
//
//        // ���� ����
//        for (auto sound : bgms) {
//            if (sound) FMOD_Sound_Release(sound);
//        }
//        for (auto sound : sfx) {
//            if (sound) FMOD_Sound_Release(sound);
//        }
//
//        // �ý��� ����
//        if (system) {
//            FMOD_System_Close(system);
//            FMOD_System_Release(system);
//            system = nullptr;
//        }
//
//        // �����̳� ����
//        bgms.clear();
//        bgmChannels.clear();
//        sfx.clear();
//        activeSfxChannels.clear();
//
//        initialized = false;
//    }
//
//    bool IsInitialized() const { return initialized; }
//
//    int GetBGMCount() const { return static_cast<int>(bgms.size()); }
//    int GetSFXCount() const { return static_cast<int>(sfx.size()); }
//
//    // ����/�̵� ����
//    SoundManager(const SoundManager&) = delete;
//    SoundManager& operator=(const SoundManager&) = delete;
//    SoundManager(SoundManager&&) = delete;
//    SoundManager& operator=(SoundManager&&) = delete;

private:
    SoundManager() = default;

    ~SoundManager() {
       // Shutdown();  // �Ҹ��ڿ��� �ڵ� ����
    }
};