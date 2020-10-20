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

enum PLAYER
{
	PL1, PL2
};

class RoundManager
{
	std::unique_ptr<Player> p1;
	std::unique_ptr<Player> p2;
		
	Player* currentlyPlayingPlayer;
	TARGET target;			// target ball
	TARGET newTarget;		// new target calculated in ManagePoints and is assigned to 'target' at the end of GetBallsToBack

	bool roundGoing;
	bool whiteMovable;
	bool whitePlaced;

	// nomination //
	bool canNominate;
	bool isNominating;
	bool madeOneNomination;
	
	// walkmode //
	bool isInWalkMode;
	
	std::vector<std::unique_ptr<BallObject>> ballsPottedCurrRound;
	BallObject* firstHit = nullptr;

	// handling game end //
	bool isRestarted;

	// terrible !! needs refactor //
	// as RoundMan is not responsible for doing anything with the ballset //
	// also it is set through a function that uses ballset firstly during runtime //
	BallSet* bs;

	///////////////////////
	// handling overlays //
	///////////////////////
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	OverlaySet* overlaySet;
	///////////////////////

	void ResetFlags() {
		roundGoing = false;
		whiteMovable = true;
		whitePlaced = true;
		canNominate = false;
		isNominating = false;
		isInWalkMode = false;
		madeOneNomination = false;
		firstHit = nullptr;
	}


	Player* GetOtherPlayer() const;
	RoundManager();

public:
	~RoundManager();
	static RoundManager& GetInstance();

	void InitOverlaySystem(ID3D11Device* device, ID3D11DeviceContext* deviceContext, OverlaySet* overlay);

	TARGET GetTarget() const noexcept;
	

	bool IsWhiteDroppedLastRound()		const	noexcept;
	void SetWhiteDropped(bool dropped)			noexcept;
	bool IsWhitePlaced()				const	noexcept;
	void SetWhitePlaced(bool placed)			noexcept;
	bool IsRoundGoing()					const	noexcept;
	void SetRoundGoing(bool state)				noexcept;
	void MemoFirstHit(BallObject* ball)			noexcept;
	bool CanNominate()					const	noexcept;
	void DisableNomination()					noexcept;
	void EnterNominateMode()					noexcept;
	void ExitNominateMode(bool successful=true)	noexcept;
	bool IsInNominateMode()				const	noexcept;
	bool AlreadyMadeOneNomination()		const	noexcept;
	void EnterWalkMode()						noexcept;
	void ExitWalkMode()							noexcept;
	bool IsInWalkMode()					const	noexcept;

	std::string GetPlayerDisplayName(PLAYER)			const;
	const std::unique_ptr<Player>& GetPlayer(PLAYER)	const;


	void AddNewPottedBall(std::unique_ptr<BallObject>&& ball);
	void ManagePoints(BallSet* ballSet);
	std::vector<std::unique_ptr<BallObject>> GetBallsToPutBack(BallSet* ballSet);
	void DeclareWinner(bool concede = false);
	void UpdateTarget(BallSet* ballSet, BALL t);
	void UpdateTarget(XMVECTOR s, XMVECTOR dir);

	void ClearOverlay();
};

