#include "pch.h"
#include "RoundManager.h"


RoundManager::RoundManager()
{
	p1 = std::make_unique<Player>("Player 1");
	p2 = std::make_unique<Player>("Player 2");

	currentlyPlayingPlayer = p1.get();
	target = TARGET::RED;
}

RoundManager::~RoundManager() = default;

RoundManager& RoundManager::GetInstance()
{
	static RoundManager rm;
	return rm;
}

void RoundManager::SetRoundHandled(bool roundHandled)
{
	this->roundHandled = roundHandled;
}

bool RoundManager::IsWhiteDroppedLastRound() const noexcept
{
	return whiteMovable;
}

void RoundManager::SetWhitePlaced(bool placed) noexcept
{
	whitePlaced = placed;
}

/* Collecting the balls potted in the current round in order!
*
*  As soon as the round ends (all the balls stop) this array is
*  getting processed by the ManagerPoints() function.
*/
void RoundManager::AddNewPottedBall(std::unique_ptr<BallObject>&& ball)
{
	ballsPottedCurrRound.push_back(std::move(ball));
}

/* If all the balls are stopped we can calculate the points.
*  No need to synchronize the AddNewPottedBall and this function
*  as static state balls cannot be potted.
*/
void RoundManager::ManagePoints(bool hadHits)
{
	int pointsForCurrentPlayer = 0;
	int pointsForOtherPlayer = 0;

	auto updatePoints = [&] () {
		if (p1.get() == currentlyPlayingPlayer)
		{
			p1->AddPoints(pointsForCurrentPlayer);
			p2->AddPoints(pointsForOtherPlayer);
		}
		else
		{
			p2->AddPoints(pointsForCurrentPlayer);
			p1->AddPoints(pointsForOtherPlayer);
		}
	};

	if (ballsPottedCurrRound.empty())
	{
		if (hadHits)
			return;
		
		// snooker situations, when the target ball wasn't hit
		pointsForOtherPlayer = vmax(4, target + 1);
		updatePoints();
		return;
	}

	// looking for the first hit and check whether it is correct
	// most of the shots will result a single ball potted anyway..
	BallObject* firstHit = ballsPottedCurrRound[0].get();
	if (firstHit->GetPoint() != target + 1)
	{
		// minimum penalty is 4
		pointsForOtherPlayer = vmax(4, firstHit->GetPoint(), target + 1);

		// maximum penalty, we can break out early from the function in this case
		if (pointsForOtherPlayer == 7)
		{
			updatePoints();
			return;
		}
	}
	else {
		pointsForCurrentPlayer = firstHit->GetPoint();
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

std::vector<std::unique_ptr<BallObject>> RoundManager::GetBallsToPutBack(TABLE_STATE ts)
{
	std::vector<std::unique_ptr<BallObject>> ballsToPutBack;

	for (int i = 0; i < ballsPottedCurrRound.size(); ++i)
	{

	}



	return ballsToPutBack;
}
