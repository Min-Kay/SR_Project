#pragma once
#include "Engine_Defines.h"
#include "Base.h"

#include "../../fmod/64/inc/fmod.h"
#include "../../fmod/64/inc/fmod.hpp"

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

class CSoundMgr : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)

public:
	enum CHANNELID { BGM, PLAYER,UI, EFFECT1, EFFECT2, EFFECT3, PLAYER_EFFECT1, PLAYER_EFFECT2, PLAYER_EFFECT3, ENEMY_EFFECT1, ENEMY_EFFECT2, ENEMY_EFFECT3, WEAPON_EFFECT1, WEAPON_EFFECT2, WEAPON_EFFECT3, ADDITIONAL_EFFECT1, ADDITIONAL_EFFECT2, MAXCHANNEL };
private:
	CSoundMgr();
	virtual ~CSoundMgr() = default;

public:
	HRESULT NativeConstruct();

public:
	int  VolumeUp(CHANNELID eID, _float _vol);
	int  VolumeDown(CHANNELID eID, _float _vol);
	int  BGMVolumeUp(_float _vol);
	int  BGMVolumeDown(_float _vol);
	int  Pause(CHANNELID eID);
	void Play_Sound(TCHAR* pSoundKey, CHANNELID eID, _float _vol);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();

private:
	float m_volume = SOUND_DEFAULT;
	float m_BGMvolume = SOUND_DEFAULT;
	FMOD_BOOL m_bool;

public:
	virtual void Free() override;

private:
	void LoadSoundFile();

private:
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem = nullptr;
	_bool		m_bPause = false;
};

