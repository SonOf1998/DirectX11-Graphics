#include "pch.h"

#include "Player.h"


Player::Player(const std::string& name) : name(name), point(0), wonFrames(0)
{
}

Player::~Player() = default;

