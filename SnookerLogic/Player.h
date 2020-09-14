#pragma once
class Player
{
	std::string name;
	int point;
	int wonFrames;

public:
	Player(const std::string& name);
	~Player();

	int GetPoints() const noexcept { return point; }
	int GetWonFrame() const noexcept { return wonFrames; }
	void AddPoints(int val) { point += val; }
	void IncrementWonFrames() { ++wonFrames; }
};

