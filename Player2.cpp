#include "Player2.h"
#include <climits>
#include <iostream>
#include <stdlib.h>

// generateAllValidGuesses(n, k, currentGuess, used, S) generates all valid guesses of length n using digits from 1 to k without repetition and stores them in S
void Player2::generateAllValidGuesses(std::string currentGuess, std::vector<bool>& used) {
	// reached the desired length of the guess, add it to the set of valid guesses
	if (currentGuess.length() == n) {
		S.push_back(currentGuess);
		return;
	}

	// add digits from 1 to k to the current guess, ensuring no duplicates
	for (int digit = 1; digit <= k; ++digit) {
		if (!used[digit]) {
			used[digit] = true;

			generateAllValidGuesses(currentGuess + std::to_string(digit), used);
			used[digit] = false; // backtrack to explore other possibilities
		}
	}
}

// isConsistentGuess(guess, guessesFeedbacksSeries, n) returns true if guess is consistent with the guessesFeedbacksSeries, false otherwise
bool Player2::isConsistentGuess(
	const std::string& guess,
	const std::vector<std::pair<std::string, std::pair<int, int>>>& guessesFeedbacksSeries) const {

	for (const auto& [g, f] : guessesFeedbacksSeries) {
		auto [bulls, cows] = calculateFeedback(guess, g); // calculate feedback for guess against g
		if (bulls != f.first || cows != f.second) { // if the feedback doesn't match, guess is not consistent
			return false;
		}
	}
	return true;
}


// getSmallestConsistentGuess(guessesFeedbacksSeries, S, n) returns the lexicographically smallest guess that is consistent with the guessesFeedbacksSeries
std::string Player2::getSmallestConsistentGuess() const {

	for (const std::string& guess : S) {
		if (isConsistentGuess(guess, guessesFeedbacksSeries)) {
			return guess; // found smallest consistent guess
		}
	}
	return ""; // no consistent guess found
}


// get_sub_group_of_S_with_only_consistent_guesses_by_f_and_g(S, g, f) returns a subgroup of S that contains only guesses consistent with feedback f for guess g
std::vector<std::string> Player2::get_sub_group_of_S_with_only_consistent_guesses_by_f_and_g(
	const std::string& g,
	const std::pair<int, int>& f) const {

	int n = g.length();
	std::vector<std::string> sub_group;
	for (const std::string& guess : S) {
		if (isConsistentGuess(guess, { {g, f} })) { // check if guess is consistent with feedback f for guess g
			sub_group.push_back(guess); // add to subgroup if consistent
		}
	}
	return sub_group;
}


// get_smallest_lex_guess_with_minimum_maximum_sub_group_size(S, g) returns the smallest lexicographical guess from S that minimizes the maximum size of the sub group of consistent guesses for each possible feedback
std::string Player2::get_smallest_lex_guess_with_minimum_maximum_sub_group_size() const {

	int n = S[0].length();
	if (S.empty()) return "";
	if (S.size() == 1) return S[0];

	int min_max_cost = INT_MAX;
	std::string best_guess = "";

	// check all possibilities for the next guess x in S
	for (const std::string& x : S) {

		// key: feedback (bulls, cows), value: count of how many candidates in S would give that feedback if x is guessed
		std::map<std::pair<int, int>, int> feedback_counts;

		// calculate the feedback of x against each possible candidate y in S
		for (const std::string& y : S) {
			std::pair<int, int> f = calculateFeedback(x, y);
			feedback_counts[f]++;
		}

		// find the maximum count of candidates that would give the same feedback for guess x
		int current_max_cost = 0;
		for (const auto& pair : feedback_counts) {
			if (pair.second > current_max_cost) {
				current_max_cost = pair.second;
			}
		}

		// update the best guess if the current guess has a smaller maximum cost
		if (current_max_cost < min_max_cost) {
			min_max_cost = current_max_cost;
			best_guess = x;
		}
	}

	return best_guess;
}

Player2::Player2(int n, int k, int strategyNum) : Player(n, k), strategyNum(strategyNum)
{
	std::vector<bool> used(k + 1, false);
	generateAllValidGuesses("", used);
	if (S.empty()) {
		std::cout << ERROR_MESSAGE;
		exit(1);
	}
	current_guess = S[0];
}

void Player2::receiveFeedback(const std::pair<int, int>& feedback)
{
	guessesFeedbacksSeries.push_back({ current_guess, feedback });

	if (strategyNum == 1) {
		current_guess = getSmallestConsistentGuess();
	}
	else if (strategyNum == 2) {
		S = get_sub_group_of_S_with_only_consistent_guesses_by_f_and_g(current_guess, feedback);
		current_guess = get_smallest_lex_guess_with_minimum_maximum_sub_group_size();
	}
}
