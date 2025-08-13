#include "SoundManager.h"
#include "SoundBank_.h"
#include "ResourceManager.h"

#undef max
#undef min

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
    if (result == FMOD_OK) {
        // ������ ������ ����. fadeIn�� ��� ���� 0���� ����.
        float initialVolume = fadeIn ? 0.0f : m_masterBGMVolume;
        FMOD_Channel_SetVolume(bgmChannels[index], initialVolume);
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
        // ���� ������ ������ ������ ���Ͽ� ���� ���� ����
        FMOD_Channel_SetVolume(channel, volume * m_masterSFXVolume);
        activeSfxChannels.push_back(channel);
    }

    return result;
}

FMOD_RESULT SoundManager::StopSFX(int index)
{
    // Ư�� SFX�� �����Ϸ��� activeSfxChannels�� ��ȸ�ϸ� ���带 ���ؾ� �մϴ�.
    // ���� ���������� ��������Ƿ� �ʿ� �� ������ �߰��ؾ� �մϴ�.
    return FMOD_OK;
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
        // �� �Լ��� ������ ������ ������ Ư�� ä���� ������ ���� �����մϴ�.
        // ������ ������ ����Ϸ��� volume * m_masterBGMVolume ���� �����ؾ� �մϴ�.
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
    if (m_crossfade.crossfadeActive)
    {
        m_crossfade.crossElapsed += 0.016f;
        float t = m_crossfade.crossElapsed / m_crossfade.crossFadeTime;
        if (t > 1.0f) t = 1.0f;

        // from(����) ������ ������ ������ ���� ����
        if (m_crossfade.crossFrom >= 0 && m_crossfade.crossFrom < static_cast<int>(bgmChannels.size()) && bgmChannels[m_crossfade.crossFrom]) {
            FMOD_Channel_SetVolume(bgmChannels[m_crossfade.crossFrom], m_crossfade.crossFromStartVol * (1.0f - t) * m_masterBGMVolume);
        }

        // to(���ο�) ������ ������ ������ ���� ����
        if (m_crossfade.crossTo >= 0 && m_crossfade.crossTo < static_cast<int>(bgmChannels.size()) && bgmChannels[m_crossfade.crossTo]) {
            float targetVolume = m_crossfade.crossToStartVol + (1.0f - m_crossfade.crossToStartVol) * t;
            FMOD_Channel_SetVolume(bgmChannels[m_crossfade.crossTo], targetVolume * m_masterBGMVolume);
        }

        // �Ϸ� ó��
        if (t >= 1.0f)
        {
            if (m_crossfade.crossFrom >= 0 && m_crossfade.crossFrom < static_cast<int>(bgmChannels.size()) && bgmChannels[m_crossfade.crossFrom]) {
                FMOD_Channel_Stop(bgmChannels[m_crossfade.crossFrom]);
                bgmChannels[m_crossfade.crossFrom] = nullptr;
            }
            m_crossfade.crossfadeActive = false;
            m_crossfade.crossFrom = -1;
            m_crossfade.crossTo = -1;
            m_crossfade.crossFadeTime = 0.f;
            m_crossfade.crossElapsed = 0.f;
        }
    }

    CleanupFinishedSFX();
    return FMOD_System_Update(system);
}

void SoundManager::Shutdown()
{
    if (!initialized) return;

    for (auto& channel : bgmChannels) {
        if (channel) FMOD_Channel_Stop(channel);
    }
    for (auto& channel : activeSfxChannels) {
        if (channel) FMOD_Channel_Stop(channel);
    }

    for (auto sound : bgms) {
        if (sound) FMOD_Sound_Release(sound);
    }
    for (auto sound : sfx) {
        if (sound) FMOD_Sound_Release(sound);
    }

    if (system) {
        FMOD_System_Close(system);
        FMOD_System_Release(system);
        system = nullptr;
    }

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

FMOD_RESULT SoundManager::CrossfadeBGM(const std::string& fromKey, const std::string& tokey, float fadeTime)
{
    auto itTo = bgmKeyMap.find(tokey);
    if (itTo == bgmKeyMap.end()) return FMOD_ERR_INVALID_PARAM;

    int idxTo = itTo->second;
    int idxFrom = -1;
    auto itFrom = bgmKeyMap.find(fromKey);
    if (itFrom != bgmKeyMap.end())
        idxFrom = itFrom->second;

    Set_Playing_Key(tokey);

    FMOD_BOOL isPlaying = 0;
    if (idxTo >= 0 && idxTo < bgmChannels.size() && bgmChannels[idxTo]) {
        FMOD_Channel_IsPlaying(bgmChannels[idxTo], &isPlaying);
    }

    if (!isPlaying) {
        PlayBGM(idxTo, /*fadeIn=*/true); // fadeIn=true�� ���� 0���� ��� ����
    }
    else {
        FMOD_Channel_SetVolume(bgmChannels[idxTo], 0.0f); // �̹� ������̸� ������ 0����
    }

    float fromStartVol = 0.0f;
    if (idxFrom >= 0 && idxFrom < static_cast<int>(bgmChannels.size()) && bgmChannels[idxFrom]) {
        FMOD_Channel_GetVolume(bgmChannels[idxFrom], &fromStartVol);
        // ������ ������ ����� ������ �����̹Ƿ�, 1.0�� �������� �ϴ� ��� �������� ��ȯ
        if (m_masterBGMVolume > 0.001f) {
            fromStartVol /= m_masterBGMVolume;
        }
        else {
            fromStartVol = 0.0f;
        }
    }

    m_crossfade.crossFrom = idxFrom;
    m_crossfade.crossTo = idxTo;
    m_crossfade.crossFadeTime = std::max(0.001f, fadeTime);
    m_crossfade.crossElapsed = 0.f;
    m_crossfade.crossFromStartVol = fromStartVol;
    m_crossfade.crossToStartVol = 0.0f;
    m_crossfade.crossfadeActive = true;

    Playing_key = tokey;
    return FMOD_OK;
}

FMOD_RESULT SoundManager::AddBGM(const std::string& key, FMOD_SOUND* sound)
{
    bgmKeyMap[key] = static_cast<int>(bgms.size());
    bgms.push_back(sound);
    bgmChannels.push_back(nullptr);
    return FMOD_OK;
}

FMOD_RESULT SoundManager::AddSFX(const std::string& key, FMOD_SOUND* sound)
{
    sfxKeyMap[key] = static_cast<int>(sfx.size());
    sfx.push_back(sound);
    return FMOD_OK;
}

FMOD_RESULT SoundManager::PlayBGM(const std::string& key, bool fadeIn)
{
    auto it = bgmKeyMap.find(key);
    if (it == bgmKeyMap.end()) return FMOD_ERR_INVALID_PARAM;

    Set_Playing_Key(key);
    return PlayBGM(it->second, fadeIn);
}

FMOD_RESULT SoundManager::PlaySFX(const std::string& key, float volume)
{
    auto it = sfxKeyMap.find(key);
    if (it == sfxKeyMap.end()) return FMOD_ERR_INVALID_PARAM;
    return PlaySFX(it->second, volume);
}

FMOD_RESULT SoundManager::PauseBGM(const std::string& key, bool pause)
{
    auto it = bgmKeyMap.find(key);
    if (it == bgmKeyMap.end()) {
        return FMOD_ERR_INVALID_PARAM;
    }
    return PauseBGM(it->second, pause);
}

FMOD_RESULT SoundManager::PauseSFX(const std::string& key, bool pause)
{
    auto it = sfxKeyMap.find(key);
    if (it == sfxKeyMap.end()) return FMOD_ERR_INVALID_PARAM;
    return PauseSFX(it->second, pause);
}

FMOD_RESULT SoundManager::PauseSFX(int index, bool pause)
{
    if (!initialized || index < 0 || index >= sfx.size())
        return FMOD_ERR_INVALID_PARAM;

    for (auto& channel : activeSfxChannels)
    {
        if (!channel) continue;
        FMOD_SOUND* currentSound = nullptr;
        if (FMOD_Channel_GetCurrentSound(channel, &currentSound) == FMOD_OK &&
            currentSound == sfx[index])
        {
            FMOD_Channel_SetPaused(channel, pause);
        }
    }

    return FMOD_OK;
}

void SoundManager::SetMasterBGMVolume(float volume)
{
    m_masterBGMVolume = std::clamp(volume, 0.0f, 1.0f);

    // ���� ��� ���� ��� BGM ä�ο� �� ������ ���� ��� ����
    for (auto& channel : bgmChannels)
    {
        if (channel)
        {
            // ũ�ν����̵� ���� ä���� Update���� ������ �����ϹǷ� ������ �� ������,
            // �ϰ����� ���� �켱 ��� ä�ο� �����մϴ�.
            FMOD_Channel_SetVolume(channel, m_masterBGMVolume);
        }
    }
}

void SoundManager::SetMasterSFXVolume(float volume)
{
    m_masterSFXVolume = std::clamp(volume, 0.0f, 1.0f);
    // SFX�� ��� ������ ������ ������ ����ǹǷ�, ���⼭�� ���� �����մϴ�.
    // ���� ��� ���� SFX ������ �ٷ� �ٲٰ� �ʹٸ�, activeSfxChannels�� ��ȸ�ϸ�
    // FMOD_Channel_SetVolume(channel, m_masterSFXVolume)�� ȣ���ؾ� �մϴ�.
}

float SoundManager::GetMasterBGMVolume() const
{
    return m_masterBGMVolume;
}

float SoundManager::GetMasterSFXVolume() const
{
    return m_masterSFXVolume;
}

void SoundManager::TakeAllClip()
{
    auto map = ResourceManager::Get().Get_SoundBank().Get_EDM_MAP();
    for (auto const& [name, source] : map)
    {
        std::string key = name;
        AddBGM(key, ResourceManager::Get().Get_SoundBank().GetEndingBGM(name));
    }

    map = ResourceManager::Get().Get_SoundBank().Get_BGM_MAP();
    for (auto const& [name, source] : map)
    {
        std::string key = name;
        AddBGM(key, ResourceManager::Get().Get_SoundBank().GetBGM(name));
    }

    map = ResourceManager::Get().Get_SoundBank().Get_EUIM_MAP();
    for (auto const& [name, source] : map)
    {
        std::string key = name;
        AddSFX(key, ResourceManager::Get().Get_SoundBank().GetUIBGM(name));
    }

    map = ResourceManager::Get().Get_SoundBank().Get_HM_MAP();
    for (auto const& [name, source] : map)
    {
        std::string key = name;
        AddBGM(key, ResourceManager::Get().Get_SoundBank().GetHistoryBGM(name));
    }

    map = ResourceManager::Get().Get_SoundBank().Get_ES_MAP();
    for (auto const& [name, source] : map)
    {
        std::string key = name;
        AddSFX(key, ResourceManager::Get().Get_SoundBank().GetESBGM(name));
    }
}