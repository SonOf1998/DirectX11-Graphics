#include "pch.h"
#include "RoundManager.h"

#include "BallSet.h"
#include "OverlayObject.h"
#include "OverlaySet.h"
#include "WhiteBallObject.h"

Player* RoundManager::GetOtherPlayer() const
{
	Player* otherPlayer;

	if (currentlyPlayingPlayer == p1.get())
		otherPlayer = p2.get();
	else
		otherPlayer = p1.get();

	return otherPlayer;
}

RoundManager::RoundManager()
{
	p1 = std::make_unique<Player>("Player 1");
	p2 = std::make_unique<Player>("Player 2");

	currentlyPlayingPlayer = p1.get();
	target = TARGET::RED;
	ResetFlags();
}

RoundManager::~RoundManager() = default;

RoundManager& RoundManager::GetInstance()
{
	static RoundManager rm;
	return rm;
}

void RoundManager::InitOverlaySystem(ID3D11Device* device, ID3D11DeviceContext* deviceContext, OverlaySet* overlay)
{
	overlaySet = overlay;
	this->device = device;
	this->deviceContext = deviceContext;
	
	// red ball as the first target
	std::unique_ptr<OverlayObject> overlayItem = std::make_unique<OverlayObject>(device, deviceContext, std::numeric_limits<float>::max(), RED_BALL, OVERLAY_TARGET);
	overlaySet->AddOverlayItem(std::move(overlayItem));
}

TARGET RoundManager::GetTarget() const noexcept
{
	return target;
}

bool RoundManager::IsWhiteDroppedLastRound() const noexcept
{
	return whiteMovable;
}

void RoundManager::SetWhiteDropped(bool dropped) noexcept
{
	whiteMovable = dropped;
}

void RoundManager::SetWhitePlaced(bool placed) noexcept
{
	whitePlaced = placed;
}

bool RoundManager::IsWhitePlaced() const noexcept
{
	return whitePlaced;
}

bool RoundManager::IsRoundGoing() const noexcept
{
	return roundGoing;
}

void RoundManager::SetRoundGoing(bool state) noexcept
{
	roundGoing = state;
}

void RoundManager::MemoFirstHit(BallObject* ball) noexcept
{
	if (firstHit != nullptr)
		return;

	firstHit = ball;
}

bool RoundManager::CanNominate() const noexcept
{
	return canNominate;
}

void RoundManager::DisableNomination() noexcept
{
	canNominate = false;
}

void RoundManager::EnterNominateMode() noexcept
{
	overlaySet->ChangeTarget(NOMINATE_WARNING);
	isNominating = true;
}

void RoundManager::ExitNominateMode(bool successful /* true */) noexcept
{
	isNominating = false;

	// check if nominate mode is done by an actual nomination
	// or only because we hit the white ball
	if (successful)
		madeOneNomination = true;
}

bool RoundManager::IsInNominateMode() const noexcept
{
	return isNominating;
}

bool RoundManager::AlreadyMadeOneNomination() const noexcept
{
	return madeOneNomination;
}

void RoundManager::EnterWalkMode() noexcept
{
	isInWalkMode = true;
}

void RoundManager::ExitWalkMode() noexcept
{
	isInWalkMode = false;
}

bool RoundManager::IsInWalkMode() const noexcept
{
	return isInWalkMode;
}

std::string RoundManager::GetPlayerDisplayName(PLAYER id) const
{
	Player* player;
	std::string additionalData;

	switch (id)
	{
	case PL1:
		player = p1.get();
		break;
	case PL2:
		player = p2.get();
		break;
	default:
		throw std::runtime_error("Switch is not exhaustive in RoundManager::GetPlayerDisplayName()");
	}
	
	if (player == currentlyPlayingPlayer)
		additionalData = "'";

	std::string outString = player->GetName() + additionalData;
	return outString;
}

const std::unique_ptr<Player>& RoundManager::GetPlayer(PLAYER id) const
{
	switch (id)
	{
	case PL1:
		return p1;
	case PL2:
		return p2;
	default:
		throw std::runtime_error("Switch is not exhaustive in RoundManager::GetPlayer()");
	}
}

/* Collecting the balls potted in the current round in order!
*
*  As soon as the round ends (all the balls stop) this array is
*  getting processed by the ManagerPoints() function.
*/
void RoundManager::AddNewPottedBall(std::unique_ptr<BallObject>&& ball)
{
	ballsPottedCurrRound.push_back(std::move(ball));
	overlaySet->KillOverlayWithTTL();

	for (uint i = 0; i < ballsPottedCurrRound.size(); ++i)
	{
		int point = ballsPottedCurrRound[i]->GetPoint();
		int ballResId = (point == -4) ? 7 : point - 1;
		uint spotResId = OVERLAY_POT6 - ballsPottedCurrRound.size() + 1 + i;
		std::unique_ptr<OverlayObject> overlayItem = std::make_unique<OverlayObject>(device, deviceContext, 2 + i * 0.10f, (BALL)ballResId, (OVERLAY_SPOT)spotResId);
		overlaySet->AddOverlayItem(std::move(overlayItem));
	}
}
/* If all the balls are stopped we can calculate the points.
*  No need to synchronize the AddNewPottedBall and this function
*  as static state balls cannot be potted.
*/
void RoundManager::ManagePoints(BallSet* ballSet)
{
	// terrible, needs refactor
	// see more @ bs declaration
	bs = ballSet;

	int pointsForCurrentPlayer = 0;
	int pointsForOtherPlayer = 0;

	auto updatePoints = [&] () {

		currentlyPlayingPlayer->AddPoints(pointsForCurrentPlayer);
		GetOtherPlayer()->AddPoints(pointsForOtherPlayer);

		// curr play made a successful pot
		if (pointsForCurrentPlayer > 0)
		{
			currentlyPlayingPlayer->AddBreakPoints(pointsForCurrentPlayer);

			// switch to next target ball
			if (target == RED) {
				ballSet->GetClosestColorToCueBall(ballSet->GetWhiteBallPosition(), &newTarget);
				overlaySet->ChangeTarget((BALL)newTarget);
				canNominate = true;
			}
			else
			{
				if (ballSet->HasReds())
				{
					overlaySet->ChangeTarget(RED_BALL);
					newTarget = RED;
				}
				else
				{
					if (ballSet->HasLower(target))
					{
						overlaySet->ChangeTarget(YELLOW_BALL);
						newTarget = YELLOW;
					}
					else if (target != BLACK_BALL)
					{
						overlaySet->ChangeTarget((BALL)(target + 1));
						newTarget = (TARGET)(target + 1);
					}
					else
					{
						if (currentlyPlayingPlayer->GetPoints() == GetOtherPlayer()->GetPoints())
						{
							target = PINK; // hack GetBallsToPutBack function
							newTarget = BLACK;

							ballSet->MakeWhiteMovable();
						}
						else
						{
							DeclareWinner();
							ballSet->InitTable();
						}
					}
				}
			}
		}
		// foul or current playet didn't pot anything
		else
		{
			currentlyPlayingPlayer->ResetBreak();
			currentlyPlayingPlayer = GetOtherPlayer();

			if (ballSet->HasReds())
			{
				overlaySet->ChangeTarget(RED_BALL);
				newTarget = RED;
			}
			else
			{
				if (ballSet->HasLower(target))
				{
					overlaySet->ChangeTarget(YELLOW_BALL);
					newTarget = YELLOW;
				}
				else
				{
					overlaySet->ChangeTarget((BALL)(target));
					newTarget = (TARGET)(target);
				}
			}
		}

		pointsForCurrentPlayer = 0;
		pointsForOtherPlayer = 0;
	};



	// first check the case where no balls potted
	if (ballsPottedCurrRound.empty())
	{
		// if theres a first hit
		if (firstHit != nullptr)
		{
			// which fails to hit the target ball first
			if (firstHit->GetPoint() != target + 1)
			{
				pointsForOtherPlayer = vmax(4, firstHit->GetPoint(), target + 1);
				updatePoints();
			}
			// correct shot, but no pot
			// update point doesnt get called this case
			else
			{
				currentlyPlayingPlayer->ResetBreak();
				currentlyPlayingPlayer = GetOtherPlayer();

				// havent pot anything and table still has reds
				if (ballSet->HasReds())
				{
					overlaySet->ChangeTarget(RED_BALL);
					newTarget = RED;
				}
				// was playing for red, havent pot anything and the table has no more reds
				// assign yellow as the target ball in this case
				else if (target == RED && !ballSet->HasReds())
				{
					overlaySet->ChangeTarget(YELLOW_BALL);
					newTarget = YELLOW;
				}
				// was playing for color, keep the target in this case
				else
				{
					/*overlaySet->ChangeTarget((BALL)(target + 1));
					newTarget = (TARGET)(target);*/
				}
			}
		}
		else
		{
			// snooker situation foul
			// or player hit the cue ball to soft
			pointsForOtherPlayer = vmax(4, target + 1);
			updatePoints();
		}
				
		return;
	}

	// there was at least one potted ball
	// again, check if the first hit was correct
	if (firstHit != nullptr)
	{
		if (firstHit->GetPoint() != target + 1)
		{
			pointsForOtherPlayer = vmax(4, target + 1);
			if (pointsForOtherPlayer == 7)
			{
				updatePoints();
				return;
			}
		}
	}

	// looking for the first pot and check whether it is correct
	// most of the shots will result a single ball potted anyway..
	BallObject* firstPot = ballsPottedCurrRound[0].get();
	if (firstPot->GetPoint() != target + 1)
	{
		// minimum penalty is 4
		pointsForOtherPlayer = vmax(pointsForOtherPlayer, std::abs(firstPot->GetPoint()), target + 1);

		// maximum penalty, we can break out early from the function in this case
		if (pointsForOtherPlayer == 7)
		{
			updatePoints();
			return;
		}
	}
	else {
		pointsForCurrentPlayer = firstPot->GetPoint();
	}
	
	// handling each consecutive pots
	for (int i = 1; i < ballsPottedCurrRound.size(); ++i)
	{
		BallObject* hit = ballsPottedCurrRound[i].get();

		if (target == TARGET::RED)
		{
			// Is multiple potted reds the case?
			if (pointsForCurrentPlayer == i && hit->GetPoint() == 1)
			{
				++pointsForCurrentPlayer;
				ballSet->GetClosestColorToCueBall(ballSet->GetWhiteBallPosition(), &newTarget);
				overlaySet->ChangeTarget((BALL)newTarget);
				canNominate = true;
			}
			else
			{
				pointsForCurrentPlayer = 0;
				pointsForOtherPlayer = vmax(pointsForOtherPlayer, 4, hit->GetPoint());
			}
		}
		else
		{
			pointsForCurrentPlayer = 0;
			pointsForOtherPlayer = vmax(pointsForOtherPlayer, 4, hit->GetPoint());
		}
	}

	updatePoints();
}

std::vector<std::unique_ptr<BallObject>> RoundManager::GetBallsToPutBack(BallSet* ballSet)
{
	std::vector<std::unique_ptr<BallObject>> ballsToPutBack;

	for (int i = 0; i < ballsPottedCurrRound.size() && !isRestarted; ++i)
	{
		auto& ball = ballsPottedCurrRound[i];

		if (ballSet->HasReds())
		{
			if (ball->GetPoint() != 1)
			{
				ballsToPutBack.push_back(std::move(ball));
			}
		}
		else
		{
			if (target + 1 != 7 && ball->GetPoint() == -4)
			{
				ballsToPutBack.push_back(std::move(ball));
			}
			if (ball->GetPoint() > target + 1)
			{
				ballsToPutBack.push_back(std::move(ball));
			}
		}
	}
	
	target = newTarget;

	// we flag this round as handled
	roundGoing = false;
	firstHit = nullptr;
	ballsPottedCurrRound.clear();
	isRestarted = false;
	madeOneNomination = false;

	std::sort(ballsToPutBack.begin(), ballsToPutBack.end(), [] (std::unique_ptr<BallObject>& b1, std::unique_ptr<BallObject>& b2) {
		return b1->GetPoint() > b2->GetPoint();
	});
	return ballsToPutBack;
}

void RoundManager::DeclareWinner(bool concede) 
{
	Player* otherPlayer = GetOtherPlayer();

	if (concede)
	{
		otherPlayer->IncrementWonFrames();
	}
	else
	{
		// missed black ball game assert
		assert(currentlyPlayingPlayer->GetPoints() != otherPlayer->GetPoints());

		if (currentlyPlayingPlayer->GetPoints() > otherPlayer->GetPoints())
		{
			currentlyPlayingPlayer->IncrementWonFrames();
		}
		else
		{
			otherPlayer->IncrementWonFrames();
		}
	}

	currentlyPlayingPlayer->ResetBreak();
	currentlyPlayingPlayer->ResetPoints();
	otherPlayer->ResetBreak();
	otherPlayer->ResetPoints();
	WhiteBallObject::isInAimMode = true;
	WhiteBallObject::SwitchModes();
	ResetFlags();
	newTarget = RED;
	overlaySet->ChangeTarget((BALL)newTarget);
}

void RoundManager::UpdateTarget(BallSet* ballSet, BALL t)
{
	target = (TARGET)t;
	overlaySet->ChangeTarget(t);
}

void RoundManager::UpdateTarget(XMVECTOR s, XMVECTOR dir)
{
	BallObject* rayHitBall = bs->GetFirstColorHitByRay(s, dir);
	if (rayHitBall != nullptr)
	{
		target = (TARGET)(rayHitBall->GetPoint() - 1);
		overlaySet->ChangeTarget((BALL)(target));
	}
}

void RoundManager::ClearOverlay()
{
	overlaySet->KillOverlayWithTTL();
}
