#pragma once

class PerspectiveCamera;

class SoundPool
{
public:
	struct SFX
	{
		std::unique_ptr<SoundEffect> soundEffect;
		std::unique_ptr<SoundEffectInstance> effect;

		void* collidingObj1Ptr;
		void* collidingObj2Ptr;

		void Play();

		SFX& operator=(SFX&& rhs);
	};

private:
	
	SFX channels[SFX_CHANNELS];

public:

	SoundPool();
	~SoundPool();

	void AddAndTryPlaying(SFX& sfx, PerspectiveCamera* camera, const XMFLOAT3& collisionEffectCenter);
};

