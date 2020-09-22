#pragma once

/* XAudio2 framework for playing billiard sounds in 3D. */

#include "Resources.h"
#include "SoundPool.h"

class PerspectiveCamera;

class SoundManager
{
public:
	inline static bool muted = false;

public:

	// Would have been static, but static's map MS's implementation is flawed
	// and keeps raising Read Access Violation on destuctor
	std::unordered_map<std::string, std::unordered_map<int, std::wstring>> collisionFileNameMappings = { 
		{"Ball-Ball", 
			{
				{0, LIGHT_TOUCHING_SOUND},
				{1, BALL_HIT_SOUND}
			}
		},
	
		{"Ball-Wall",
			{
				{0, WALL_HIT_SOUND}
			}
		},

		{"Ball-Hole",
			{
				{0, HOLE_SOUND}
			}
		}
	};

public:

	LPCWSTR GetBallBallCollisionSoundFileName(float velocity);
	LPCWSTR GetBallWallCollisionSoundFileName(float velocity);
	LPCWSTR GetBallHoleCollosionSoundFileName(float velocity);

private:

	std::unique_ptr<AudioEngine> audioEngine;
	mutable SoundPool soundPool;
	PerspectiveCamera* camera;

	SoundManager();

public:
	
	void InitializeCamera(PerspectiveCamera* camera);
	~SoundManager() = default;


	void PlaySound(LPCWSTR wavFileName, XMFLOAT3 collisionEffectCenter, void* collidintObj1Ptr, void* collidingObj2Ptr) const;

	static SoundManager& GetInstance();
};

