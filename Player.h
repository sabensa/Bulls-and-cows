#pragma once
#include <string>
#include <utility>
class Player
{
protected:
	int n; // length of the secret code
	int k; // number of possible digits (1 to k)

	std::pair<int, int> calculateFeedback(const std::string& guess, const std::string& secret) const;

public:
	Player(int n, int k) : n(n), k(k) {}
	virtual ~Player() = default;
};