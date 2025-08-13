#include "SoundManager.h"
#include "SoundBank_.h"
#include "ResourceManager.h"

#include "UIManager.h"
#include "SettingWindow.h"

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

    if (result == FMOD_OK && fadeIn) {
        FMOD_Channel_SetVolume(bgmChannels[index], 0.0f); //
       
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

FMOD_RESULT SoundManager::StopSFX(int index)
{
    return FMOD_RESULT();
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

    std::cout << "[PauseBGM-int] ��û index=" << index
        << " / bgmChannels.size=" << bgmChannels.size()
        << " / channel_ptr=" << bgmChannels[index]
        << std::endl;

        if (bgmChannels[index]) {
            FMOD_BOOL isPlaying = 0;
            FMOD_Channel_IsPlaying(bgmChannels[index], &isPlaying);
            std::cout << "   -> ���� ����: " << (isPlaying ? "��� ��" : "������") << std::endl;
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
            // ���� ������ ����(60fps): �ٸ� �������� 0.016f ��� ���̹Ƿ� ���� ����
            m_crossfade.crossElapsed += 0.016f;

            float t = m_crossfade.crossElapsed / m_crossfade.crossFadeTime;

            SettingWindow* SETWin = dynamic_cast<SettingWindow*>(UIManager::Get().GetWindow(UIWindowType::SettingsWindow));
            float& val = SETWin->getBgmCurrentValue();

            if (t > 1.0f) t = 1.0f;

            // from(����) ���� ��
            if (m_crossfade.crossFrom >= 0 && m_crossfade.crossFrom < static_cast<int>(bgmChannels.size()) && bgmChannels[m_crossfade.crossFrom]) {
                FMOD_Channel_SetVolume(bgmChannels[m_crossfade.crossFrom], m_crossfade.crossFromStartVol * (1.0f - t));
            }

            // to(���ο�) ���� ��
            if (m_crossfade.crossTo >= 0 && m_crossfade.crossTo < static_cast<int>(bgmChannels.size()) && bgmChannels[m_crossfade.crossTo]) {
                FMOD_Channel_SetVolume(bgmChannels[m_crossfade.crossTo], m_crossfade.crossToStartVol + (1.0f - m_crossfade.crossToStartVol) * t);
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



        CleanupFinishedSFX();  // �� ������Ʈ���� ����
        return FMOD_System_Update(system);
    }

    void SoundManager::Shutdown()
    {
        if (!initialized) return;

        // ��� ä�� ����
        for (auto& channel : bgmChannels) {
            if (channel) FMOD_Channel_Stop(channel);
        }
        for (auto& channel : activeSfxChannels) {
            if (channel) FMOD_Channel_Stop(channel);
        }

        // ���� ����
        for (auto sound : bgms) {
            if (sound) FMOD_Sound_Release(sound);
        }
        for (auto sound : sfx) {
            if (sound) FMOD_Sound_Release(sound);
        }

        // �ý��� ����
        if (system) {
            FMOD_System_Close(system);
            FMOD_System_Release(system);
            system = nullptr;
        }

        // �����̳� ����
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

        // fromKey�� ���� ���� �����Ƿ� ���������� ó��
        int idxFrom = -1;
        auto itFrom = bgmKeyMap.find(fromKey);
        if (itFrom != bgmKeyMap.end())
            idxFrom = itFrom->second;

        // ��� BGM�� ��� ���� �ƴϸ� 0 �������� ����
        FMOD_RESULT r = FMOD_OK;
        FMOD_BOOL isPlaying = 0;

        Set_Playing_Key(tokey);

        //Playing_key = toKey;
        std::cout << "cross ���� key" << Playing_key << endl;

        if (idxTo < 0 || idxTo >= static_cast<int>(bgms.size()))
            return FMOD_ERR_INVALID_PARAM;

        if (!bgmChannels[idxTo]) {
            r = PlayBGM(idxTo, /*fadeIn=*/true); // ���ο��� volume 0���� ����
            if (r != FMOD_OK) return r;
        }
        else {
            FMOD_Channel_IsPlaying(bgmChannels[idxTo], &isPlaying);
            if (!isPlaying) {
                r = PlayBGM(idxTo, /*fadeIn=*/true);
                if (r != FMOD_OK) return r;
            }
            else {
                // �̹� ��� ���̸� ������ 0���� ������ ����
                FMOD_Channel_SetVolume(bgmChannels[idxTo], 0.0f);
            }
        }

        // from�� ���� ���� Ȯ�� (������ 0���� ����)
        float fromStartVol = 0.0f;
        if (idxFrom >= 0 && idxFrom < static_cast<int>(bgmChannels.size()) && bgmChannels[idxFrom]) {
            FMOD_Channel_GetVolume(bgmChannels[idxFrom], &fromStartVol);
        }
        
        // ũ�ν����̵� ���� ���
        m_crossfade.crossFrom = idxFrom;
        m_crossfade.crossTo = idxTo;
        m_crossfade.crossFadeTime = std::max(0.001f, fadeTime);
        m_crossfade.crossElapsed = 0.f;
        m_crossfade.crossFromStartVol = fromStartVol;
        m_crossfade.crossToStartVol = 0.0f; // PlayBGM(fadeIn=true)�� �̹� 0���� ����
        m_crossfade. crossfadeActive = true;

        // ���� ��� Ű ����(�ǵ�: toKey�� ���� '����'�� ����)
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

        std::cout<<"���� key" << Playing_key << endl;
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
            std::cout << "[PauseBGM-str] Ű '" << key << "'�� bgmKeyMap���� ã�� �� ����" << std::endl;
            return FMOD_ERR_INVALID_PARAM;
        }

        std::cout << "[PauseBGM-str] key='" << key << "' -> index=" << it->second << std::endl;
        return PauseBGM(it->second, pause);
    }

    FMOD_RESULT SoundManager::PauseSFX(const std::string& key, bool pause)
    {
        auto it = sfxKeyMap.find(key);
        if (it == sfxKeyMap.end()) return FMOD_ERR_INVALID_PARAM;
        return PauseSFX(it->second, pause); // ���� index ��� �Լ� ����
    }

    FMOD_RESULT SoundManager::PauseSFX(int index, bool pause)
    {
        if (!initialized || index < 0 || index >= sfx.size())
            return FMOD_ERR_INVALID_PARAM;

        // activeSfxChannels���� ��ġ�ϴ� ä���� ã�Ƽ� ��� �Ͻ�����
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

    void SoundManager::SetBGMVal(float val)
    {
        string key = Get_Playing_Key();

        int result = bgmKeyMap[key];


        FMOD_Channel_SetVolume(bgmChannels[result], val);

    }

    void SoundManager::SetSFXVal(float val)
    {
        for (auto channel : activeSfxChannels)
        {
            if (channel) {
                FMOD_Channel_SetVolume(channel, val);
            }
        }

    }
 
    void SoundManager::TakeAllClip()
    {
        auto map = ResourceManager::Get().Get_SoundBank().Get_EDM_MAP();

        for (auto [name, source] : map)
        {
            string key = name; //���� �����̱� �� 
            AddBGM(key, ResourceManager::Get().Get_SoundBank().GetEndingBGM(name));
        }

        map = ResourceManager::Get().Get_SoundBank().Get_BGM_MAP();

        for (auto [name, source] : map)
        {
            string key = name; //���� �����̱� �� 
            AddBGM(key, ResourceManager::Get().Get_SoundBank().GetBGM(name));
        }

        map = ResourceManager::Get().Get_SoundBank().Get_EUIM_MAP();

        for (auto [name, source] : map)
        {
            string key = name; //���� �����̱� �� 
            AddSFX(key, ResourceManager::Get().Get_SoundBank().GetUIBGM(name));
        }

        map = ResourceManager::Get().Get_SoundBank().Get_HM_MAP();

        for (auto [name, source] : map)
        {
            string key = name; //���� �����̱� �� 
            AddBGM(key, ResourceManager::Get().Get_SoundBank().GetHistoryBGM(name));
        }

        map = ResourceManager::Get().Get_SoundBank().Get_ES_MAP();

        for (auto [name, source] : map)
        {
            string key = name; //���� �����̱� �� 
            AddSFX(key, ResourceManager::Get().Get_SoundBank().GetESBGM(name));
        }


    }


    