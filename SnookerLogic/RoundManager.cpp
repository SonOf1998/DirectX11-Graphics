#include "pch.h"
#include "RoundManager.h"


void RoundManager::Initialize()
{
	p1 = new Player ("Player 1");
	p2 = new Player ("Player 2");

	currentlyPlayingPlayer = p1;
}

void RoundManager::Deallocate()
{
	delete p1;
	delete p2;
}

void RoundManager::AddNewPottedBall(std::unique_ptr<BallObject>&& ball)
{
	ballsPottedCurrRound.push_back(std::move(ball));
}

void RoundManager::ManagePoints()
{

}

std::vector<std::unique_ptr<BallObject>> RoundManager::GetBallsToPutBack()
{
	return std::vector<std::unique_ptr<BallObject>>();
}
