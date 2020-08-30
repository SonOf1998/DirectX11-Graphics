#pragma once
class Player
{
	std::string name;
	short point;
	short wonFrames;

public:
	Player(const std::string& name);
	~Player();

	short GetPoints() const noexcept { return point; }
	short GetWonFrame() const noexcept { return wonFrames; }
	void AddPoints(short val) { point += val; }
	void IncrementWonFrames() { ++wonFrames; }
};

