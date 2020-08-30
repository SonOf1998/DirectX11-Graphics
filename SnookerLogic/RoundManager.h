#pragma once

// from DirectX
#include "BallObject.h"

// from SnookerLogic
#include "Player.h"

enum class TARGET
{
	RED,
	YELLOW,
	GREEN,
	BROWN,
	BLUE,
	PINK,
	BLACK
};

enum class TABLE_STATE
{
	HAS_REDS,
	ONLY_COLORS
};



class RoundManager
{
	inline static Player* p1;
	inline static Player* p2;

	inline static Player* currentlyPlayingPlayer;

	inline static std::vector<std::unique_ptr<BallObject>> ballsPottedCurrRound;

public:
	RoundManager() = delete;
	~RoundManager() = delete;

	static void Initialize();
	static void Deallocate();

	static void AddNewPottedBall(std::unique_ptr<BallObject>&& ball);
	static void ManagePoints();
	static std::vector<std::unique_ptr<BallObject>> GetBallsToPutBack();
};

