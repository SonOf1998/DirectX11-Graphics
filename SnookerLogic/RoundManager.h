#pragma once

// from DirectX
#include "BallObject.h"

// from SnookerLogic
#include "Player.h"

enum TARGET
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
	std::unique_ptr<Player> p1;
	std::unique_ptr<Player> p2;
		
	Player* currentlyPlayingPlayer;
	TARGET target;	// target ball

	bool roundHandled = true;
	bool whiteMovable = true;
	bool whitePlaced = true;
	std::vector<std::unique_ptr<BallObject>> ballsPottedCurrRound;

	RoundManager();

public:
	~RoundManager();

	static RoundManager& GetInstance();

	void SetRoundHandled(bool roundHandled);

	// handling dropped white ball
	bool IsWhiteDroppedLastRound()		const	noexcept;
	void SetWhitePlaced(bool placed)			noexcept;

	void AddNewPottedBall(std::unique_ptr<BallObject>&& ball);
	void ManagePoints(bool hadHits);
	std::vector<std::unique_ptr<BallObject>> GetBallsToPutBack(TABLE_STATE ts);
};

