#include "Player.h"

std::pair<int, int> Player::calculateFeedback(const std::string& guess, const std::string& secret) const
{
	int bulls = 0, cows = 0;
	for (int i = 0; i < n; ++i) {
		if (guess[i] == secret[i]) {
			++bulls;
		}
		else if (secret.find(guess[i]) != std::string::npos) {
			++cows;
		}
	}
	return { bulls, cows };
}