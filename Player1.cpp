#include "Player1.h"
#include <random>

Player1::Player1(int n, int k, int seed, const std::vector<std::string>& S) : Player(n, k)
{
	std::mt19937 rng(seed);
	int num_options = S.size();
	int rand_index = rng() % num_options;
	secret = S[rand_index];
}

std::pair<int, int> Player1::getFeedback(const std::string& guess)
{
	return calculateFeedback(guess, secret);
}
