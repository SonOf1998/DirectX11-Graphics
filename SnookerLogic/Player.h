#pragma once
class Player
{
	std::string name;
	int breakPoint;
	int point;
	int wonFrames;

public:
	Player(const std::string& name);
	~Player();

	int GetBreak()					const noexcept { return breakPoint; }
	int GetPoints()					const noexcept { return point; }
	int GetWonFrame()				const noexcept { return wonFrames; }
	const std::string& GetName()	const		   { return name;}
	//void AddBreakPoints(int val)		  noexcept { breakPoint += val; }
	void AddPoints(int val)				  noexcept { point += val; breakPoint += val; if (val == 0) { breakPoint = 0; } }
	void IncrementWonFrames()			  noexcept { ++wonFrames; }
	void ResetBreak()					  noexcept { breakPoint = 0; }
	void ResetPoints()					  noexcept { point = 0; }
	
};

