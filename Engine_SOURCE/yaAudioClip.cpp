#include "yaAudioClip.h"
#include "yaTransform.h"
#include "StrConverter.h"

namespace ya
{
	AudioClip::AudioClip()
		: Resource(eResourceType::AudioClip)
		, mSound(nullptr)
		, mChannel(nullptr)
		, mMinDistance(1.0f)
		, mMaxDistance(1000.0f)
		, mbLoop(false)
	{

	}

	AudioClip::~AudioClip()
	{
		//mSound->release();
		//mSound = nullptr;
	}

	HRESULT AudioClip::Load(const std::wstring& path, FILE* file)
	{
		std::string cPath = StrConverter::ConvertUnicodeToUTF8(path);
		if (!Fmod::CreateSound(cPath, &mSound))
			return S_FALSE;

		mSound->set3DMinMaxDistance(mMinDistance, mMaxDistance);

		return S_OK;
	}

	void AudioClip::Play()
	{
		if (mbLoop)
			mSound->setMode(FMOD_LOOP_NORMAL);
		else
			mSound->setMode(FMOD_LOOP_OFF);

		Fmod::SoundPlay(mSound, &mChannel);
	}

	void AudioClip::Stop()
	{
		mChannel->stop();
	}

	void AudioClip::Set3DAttributes(const Vector3 pos, const Vector3 vel)
	{
		FMOD_VECTOR fmodPos(pos.x, pos.y, pos.z);
		FMOD_VECTOR fmodVel(vel.x, vel.y, vel.z);

		mChannel->set3DAttributes(&fmodPos, &fmodVel);
	}

	void AudioClip::SetDelay(unsigned long long start, unsigned long long end, bool stopchannels)
	{
		mChannel->setDelay(start, end, stopchannels);
		
	}
	void AudioClip::SetVolume(float volume)
	{
		mChannel->setVolume(volume);
	}
	bool AudioClip::isPlaying()
	{
		bool b;
		mChannel->isPlaying(&b);
		return b;
	}
}
