#include "GameRun.h"
#include <iostream>
#include <stdlib.h>

GameRun::GameRun(int n, int k, int strategyNum, int seed) : p2(n, k, strategyNum), p1(n, k, seed, p2.getS()), rounds(1), n(n)
{
	if (p2.isSEmpty()) {
		std::cout << ERROR_MESSAGE;
		exit(1);
	}
}

void GameRun::play()
{
	while (true) {
		std::string current_guess = p2.makeGuess(); // player 2 makes a guess
		std::pair<int, int> feedback = p1.getFeedback(current_guess); // player 1 gives feedback for the current guess
		std::cout << "Round " << rounds << ": Guess: " << current_guess 
			<< ", Feedback: (" << feedback.first << "," << feedback.second << ")\n";
		if (feedback.first == n) { // player 2 wins
			break;
		}
		// save the current guess and its feedback to the series of guesses and feedbacks
		p2.receiveFeedback(feedback);
		rounds++;
	}
	std::cout << "\nNumber of rounds: " << rounds << "\n";
}
