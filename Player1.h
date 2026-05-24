#pragma once
#include "Player.h"
#include <string>
#include <utility>
#include <vector>

class Player1 : public Player
{
	std::string secret; // the secret code for player 1

public:
	Player1(int n, int k, int seed, const std::vector <std::string>& S);
	std::pair<int, int> getFeedback(const std::string& guess);
};

