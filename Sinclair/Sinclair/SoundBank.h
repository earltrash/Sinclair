#pragma once
#include "pch.h"
#include "SoundManager.h"


using namespace std;
class SoundBank
{
public:
	SoundBank() = default;
	~SoundBank() = default;


	void SoundAssetLoad(string path);

	FMOD_SOUND* GetEndingBGM( string& id);
	FMOD_SOUND* GetUIBGM( string& id);
	FMOD_SOUND* GetBGM( string& id);
	FMOD_SOUND* GetHistoryBGM( string& id);

private:
	std::unordered_map<std::string, FMOD_SOUND*> m_BG;
	std::unordered_map<std::string, FMOD_SOUND*> m_EDM;
	std::unordered_map<std::string, FMOD_SOUND*> m_EUIM;
	std::unordered_map<std::string, FMOD_SOUND*> m_HM;

};

