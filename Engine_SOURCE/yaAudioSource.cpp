#include "yaAudioSource.h"
#include "yaAudioClip.h"
#include "yaTransform.h"
#include "yaGameObject.h"

namespace ya
{
	AudioSource::AudioSource()
		: Component(eComponentType::AudioSource)
		, mAudioClip(nullptr)
	{

	}

	AudioSource::~AudioSource()
	{

	}

	void AudioSource::Initialize()
	{
	}

	void AudioSource::Update()
	{
	}

	void AudioSource::FixedUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		Vector3 foward = tr->Forward();

		mAudioClip->Set3DAttributes(pos, foward);
	}

	void AudioSource::Render()
	{
	}

	void AudioSource::Play()
	{
		mAudioClip->Play();
	}
	void AudioSource::Stop()
	{
		mAudioClip->Stop();
	}
	void AudioSource::SetLoop(bool loop)
	{
		mAudioClip->SetLoop(loop);
	}
}