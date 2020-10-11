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

	bool roundGoing = false;
	bool whiteMovable = true;
	bool whitePlaced  = true;
	std::vector<std::unique_ptr<BallObject>> ballsPottedCurrRound;
	BallObject* firstHit = nullptr;

	RoundManager();

public:
	~RoundManager();
	static RoundManager& GetInstance();

	TARGET GetTarget() const noexcept;
	

	// handling dropped white ball
	bool IsWhiteDroppedLastRound()		const	noexcept;
	void SetWhiteDropped(bool dropped)			noexcept;
	bool IsWhitePlaced()				const	noexcept;
	void SetWhitePlaced(bool placed)			noexcept;
	bool IsRoundGoing()					const	noexcept;
	void SetRoundGoing(bool state)				noexcept;
	void MemoFirstHit(BallObject* ball)			noexcept;

	void AddNewPottedBall(std::unique_ptr<BallObject>&& ball);
	void ManagePoints();
	std::vector<std::unique_ptr<BallObject>> GetBallsToPutBack(TABLE_STATE ts);
};

