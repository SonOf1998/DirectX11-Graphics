#include "SoundPool.h"

#include "PerspectiveCamera.h"

void SoundPool::SFX::Play()
{
	effect->Play();
}

SoundPool::SFX& SoundPool::SFX::operator=(SoundPool::SFX&& rhs)
{
	soundEffect.release();
	effect.release();

	soundEffect = std::move(rhs.soundEffect);
	effect = std::move(rhs.effect);

	collidingObj1Ptr = rhs.collidingObj1Ptr;
	collidingObj2Ptr = rhs.collidingObj2Ptr;

	return *this;
}

SoundPool::SoundPool() = default;

SoundPool::~SoundPool() = default;

void SoundPool::AddAndTryPlaying(SoundPool::SFX& sfx, PerspectiveCamera* camera, const XMFLOAT3& collisionEffectCenter)
{
	std::optional<uint> firstEmptyChannel;

	// we scan through every sound slots
	// we save the first empty one, and keep scanning for currently playing
	// sound effects caused from the same objects as parameter sfx
	// if we find at least one we make the playing operation fail
	for (uint i = 0; i < SFX_CHANNELS; ++i)
	{
		if (channels[i].effect == nullptr || channels[i].effect->GetState() != SoundState::PLAYING)
		{
			if (!firstEmptyChannel.has_value())
			{
				firstEmptyChannel = i;
			}				
		}
		else
		{
			if ((channels[i].collidingObj1Ptr == sfx.collidingObj1Ptr && channels[i].collidingObj2Ptr == sfx.collidingObj2Ptr)
				|| (channels[i].collidingObj1Ptr == sfx.collidingObj2Ptr && channels[i].collidingObj2Ptr == sfx.collidingObj1Ptr))
			{
				return;
			}
		}
	}

	if (firstEmptyChannel.has_value())
	{
		channels[firstEmptyChannel.value()] = std::move(sfx);
		channels[firstEmptyChannel.value()].Play();

		// listener, we set the camera's attributes pretty much 
		AudioListener listener;
		listener.SetPosition(camera->GetPositionF());
		listener.SetOrientation(camera->GetForward(), camera->GetUp());

		// emitter, the center point of a collision
		AudioEmitter emitter;
		emitter.SetPosition(collisionEffectCenter);

		// applying the 3D effect on the sound
		channels[firstEmptyChannel.value()].effect->Apply3D(listener, emitter);
	}
}