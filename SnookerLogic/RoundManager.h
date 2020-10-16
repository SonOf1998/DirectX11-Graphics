#pragma once

// from DirectX
#include "BallObject.h"

// from SnookerLogic
#include "Player.h"

class BallSet;
class OverlaySet;

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

enum BALL
{
	RED_BALL,
	YELLOW_BALL,
	GREEN_BALL,
	BROWN_BALL,
	BLUE_BALL,
	PINK_BALL,
	BLACK_BALL,
	WHITE_BALL,
	NOMINATE_WARNING
};


class RoundManager
{
	std::unique_ptr<Player> p1;
	std::unique_ptr<Player> p2;
		
	Player* currentlyPlayingPlayer;
	TARGET target;			// target ball
	TARGET newTarget;		// new target calculated in ManagePoints and is assigned to 'target' at the end of GetBallsToBack

	bool roundGoing = false;
	bool whiteMovable = true;
	bool whitePlaced  = true;
	std::vector<std::unique_ptr<BallObject>> ballsPottedCurrRound;
	BallObject* firstHit = nullptr;

	///////////////////////
	// handling overlays //
	///////////////////////
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	OverlaySet* overlaySet;
	///////////////////////

	RoundManager();

public:
	~RoundManager();
	static RoundManager& GetInstance();

	void InitOverlaySystem(ID3D11Device* device, ID3D11DeviceContext* deviceContext, OverlaySet* overlay);

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
	void ManagePoints(BallSet* ballSet);
	std::vector<std::unique_ptr<BallObject>> GetBallsToPutBack(BallSet* ballSet);

	void ClearOverlay();
};

