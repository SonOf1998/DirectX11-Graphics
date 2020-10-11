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
void RoundManager::ManagePoints()
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



	// first check the case where no balls potted
	if (ballsPottedCurrRound.empty())
	{
		// if theres a first hit
		if (firstHit != nullptr)
		{
			// which fails to hit the target ball first
			if (firstHit->GetPoint() != target + 1)
			{
				pointsForOtherPlayer = vmax(4, target + 1);
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
		pointsForOtherPlayer = vmax(pointsForOtherPlayer, firstPot->GetPoint(), target + 1);

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
		auto& ball = ballsPottedCurrRound[i];

		switch (ts)
		{
		case TABLE_STATE::HAS_REDS:
			if (ball->GetPoint() != 1)
			{
				ballsToPutBack.push_back(std::move(ball));
			}
			break;
		case TABLE_STATE::ONLY_COLORS:
			if (target + 1 != 7 && ball->GetPoint() == -4)
			{
				ballsToPutBack.push_back(std::move(ball));
			}
			if (ball->GetPoint() > target + 1)
			{				
				ballsToPutBack.push_back(std::move(ball));
			}
			break;
		default:
			throw std::runtime_error("Table state switch is not exhaustive in RoundManager.cpp!");
			break;
		}
	}

	// we flag this round as handled
	roundGoing = false;
	ballsPottedCurrRound.clear();

	std::sort(ballsToPutBack.begin(), ballsToPutBack.end(), [] (std::unique_ptr<BallObject>& b1, std::unique_ptr<BallObject>& b2) {
		return b1->GetPoint() > b2->GetPoint();
	});
	return ballsToPutBack;
}
