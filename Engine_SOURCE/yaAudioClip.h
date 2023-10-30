#pragma once
#include "yaResource.h"
#include "yaFmod.h"

//#include <Fmod/fmod_studio.hpp>
//#include <Fmod/fmod.hpp>
//#include <Fmod/fmod_common.h>
//#include <Fmod/fmod_codec.h>

//#include "..\External\Include\\DirectXTex\DirectXTex.h"

namespace ya
{
	using namespace ya::math;
	class AudioClip : public Resource
	{
	public:
		AudioClip();
		~AudioClip();

		virtual HRESULT Load(const std::wstring& path, FILE* file = nullptr) override;

		void Play();
		void Stop();
		void Set3DAttributes(const Vector3 pos, const Vector3 vel);
		void SetLoop(bool loop) { mbLoop = loop; }
		void SetDelay(unsigned long long start, unsigned long long end, bool stopchannels = true);
		void SetVolume(float volume);
		bool isPlaying();
	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;
		float mMinDistance;
		float mMaxDistance;
		bool mbLoop;
	};
}

