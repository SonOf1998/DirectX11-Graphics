#include "SoundManager.h"

#include "HResultException.h"

LPCWSTR SoundManager::GetBallBallCollisionSoundFileName(float velocity)
{
	if (velocity < 0.2f)
		return collisionFileNameMappings["Ball-Ball"][0].c_str();
	else 
		return collisionFileNameMappings["Ball-Ball"][1].c_str();
}

LPCWSTR SoundManager::GetBallWallCollisionSoundFileName(float velocity)
{
	return collisionFileNameMappings["Ball-Wall"][0].c_str();
}

LPCWSTR SoundManager::GetBallHoleCollisionSoundFileName(float velocity)
{
	return collisionFileNameMappings["Ball-Hole"][0].c_str();
}

LPCWSTR SoundManager::GetCueBallCollisionSoundFileName(float speedFactor)
{
	if (speedFactor < 2.0f)
		return collisionFileNameMappings["Cue-Ball"][0].c_str();
	else
		return collisionFileNameMappings["Cue-Ball"][1].c_str();
}

SoundManager::SoundManager()
{
	HRESULT result;
	THROW_IF_HRESULT_FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
		
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags |= AudioEngine_Debug;
#endif
	audioEngine = std::make_unique<AudioEngine>(eflags);

}

std::unique_ptr<SoundEffect> soundEffect;
std::unique_ptr<SoundEffectInstance> effect;

void SoundManager::InitializeCamera(PerspectiveCamera* camera)
{
	this->camera = camera;
}

void SoundManager::PlaySound(LPCWSTR wavFileName, XMFLOAT3 collisionEffectCenter, void* collidintObj1Ptr, void* collidingObj2Ptr) const
{
	if (muted)
		return;

	audioEngine->Update();

	if (audioEngine->IsCriticalError())
	{
		throw std::runtime_error("Unknown Audio error");
	}

	std::wstring filePath = __FILEW__;
	std::wstring dir = filePath.substr(0, filePath.rfind(L"\\") + 1);

	SoundPool::SFX sfx;
	sfx.collidingObj1Ptr = collidintObj1Ptr;
	sfx.collidingObj2Ptr = collidingObj2Ptr;
	sfx.soundEffect = std::make_unique<SoundEffect>(audioEngine.get(), (dir + wavFileName).c_str());
	sfx.effect = sfx.soundEffect->CreateInstance(SoundEffectInstance_Use3D);
	sfx.effect->SetVolume(1.0f);
		

	soundPool.AddAndTryPlaying(sfx, camera, collisionEffectCenter);	
}

SoundManager& SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}
