#include <stdio.h>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <climits>
#include <map>
#include <random>
#include <stdlib.h>

std::string ERROR_MESSAGE = "invalid input\n";

// generateAllValidGuesses(n, k, currentGuess, used, S) generates all valid guesses of length n using digits from 1 to k without repetition and stores them in S
void generateAllValidGuesses(int n, int k, std::string currentGuess, std::vector<bool>& used, std::vector<std::string>& S) {
	// reached the desired length of the guess, add it to the set of valid guesses
	if (currentGuess.length() == n) {
		S.push_back(currentGuess);
		return;
	}

	// add digits from 1 to k to the current guess, ensuring no duplicates
	for (int digit = 1; digit <= k; ++digit) {
		if (!used[digit]) { 
			used[digit] = true;
			
			generateAllValidGuesses(n, k, currentGuess + std::to_string(digit), used, S);
			used[digit] = false; // backtrack to explore other possibilities
		}
	}
}

// calculateFeedback(guess, secret, n) calculates the number of bulls and cows for a given guess against the secret code
std::pair<int, int> calculateFeedback(const std::string& guess, const std::string& secret, int n) {
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

// isConsistentGuess(guess, guessesFeedbacksSeries, n) returns true if guess is consistent with the guessesFeedbacksSeries, false otherwise
bool isConsistentGuess(
	const std::string& guess,
	const std::vector<std::pair<std::string, std::pair<int, int>>>& guessesFeedbacksSeries,
	int n) {

	for (const auto& [g, f] : guessesFeedbacksSeries) {
		auto [bulls, cows] = calculateFeedback(guess, g, n); // calculate feedback for guess against g
		if (bulls != f.first || cows != f.second) { // if the feedback doesn't match, guess is not consistent
			return false;
		}
	}
	return true;
}

// getSmallestConsistentGuess(guessesFeedbacksSeries, S, n) returns the lexicographically smallest guess that is consistent with the guessesFeedbacksSeries
std::string getSmallestConsistentGuess(
	const std::vector<std::pair<std::string, std::pair<int, int>>>& guessesFeedbacksSeries, 
	const std::vector<std::string>& S,
	int n) {

	for (const std::string& guess : S) {
		if (isConsistentGuess(guess, guessesFeedbacksSeries, n)) {
			return guess; // found smallest consistent guess
		}
	}
	return ""; // no consistent guess found
}

// get_sub_group_of_S_with_only_consistent_guesses_by_f_and_g(S, g, f) returns a subgroup of S that contains only guesses consistent with feedback f for guess g
std::vector<std::string> get_sub_group_of_S_with_only_consistent_guesses_by_f_and_g(
	const std::vector<std::string>& S,
	const std::string& g,
	const std::pair<int, int>& f) {

	int n = g.length();
	std::vector<std::string> sub_group;
	for (const std::string& guess : S) {
		if (isConsistentGuess(guess, {{g, f}}, n)) { // check if guess is consistent with feedback f for guess g
			sub_group.push_back(guess); // add to subgroup if consistent
		}
	}
	return sub_group;
}

// get_smallest_lex_guess_with_minimum_maximum_sub_group_size(S, g) returns the smallest lexicographical guess from S that minimizes the maximum size of the sub group of consistent guesses for each possible feedback
std::string get_smallest_lex_guess_with_minimum_maximum_sub_group_size(
	std::vector <std::string>&S)  {

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
			std::pair<int, int> f = calculateFeedback(x, y, n);
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

// runGame(n, k, strategyNum, seed) runs the Bulls and Cows game for given n and k, allowing the user to input feedback for each guess until the correct code is guessed
void runGame(int n, int k, int strategyNum, int seed) {
	std::vector<std::string> S;
	std::vector<bool> used(k + 1, false);
	generateAllValidGuesses(n, k, "", used, S);

	if (S.empty()) {
		std::cout << ERROR_MESSAGE;
		exit(1);
	}

	// generate random secret code from S using the provided seed
	std::mt19937 rng(seed);
	int num_options = S.size();
	int rand_index = rng() % num_options;
	std::string secret = S[rand_index];

	std::vector<std::pair<std::string, std::pair<int, int>>> guessesFeedbacksSeries;
	std::string current_guess = S[0];
	int rounds = 1;

	while (true) {
		// player 1 giving feedback for the current guess against the secret code
		std::pair<int, int> feedback = calculateFeedback(current_guess, secret, n);

		std::cout << "Round " << rounds << ": Guess: " << current_guess
			<< ", Feedback: (" << feedback.first << "," << feedback.second << ")\n";

		// player 2 wins
		if (feedback.first == n) {
			break;
		}

		// save the current guess and its feedback to the series of guesses and feedbacks
		guessesFeedbacksSeries.push_back({ current_guess, feedback });

		// determine the next guess based on the chosen strategy
		if (strategyNum == 1) {
			// strategy 1 (DFS): search again on the entire original group S for the first consistent element
			current_guess = getSmallestConsistentGuess(guessesFeedbacksSeries, S, n);
		}
		else if (strategyNum == 2) {
			// strategy 2 (BFS): filter the group S and choose the guess that minimizes the search space depth
			S = get_sub_group_of_S_with_only_consistent_guesses_by_f_and_g(S, current_guess, feedback);
			current_guess = get_smallest_lex_guess_with_minimum_maximum_sub_group_size(S);
		}

		rounds++;
	}
	
	std::cout << "\nNumber of rounds: " << rounds << "\n";
}

// main function to execute the Bulls and Cows game by input parameters n, k, strategyNum, and seed
int main() {
	int n, k, strategyNum, seed;

	std::cout << "Enter n: ";
	if (!(std::cin >> n)) { 
		std::cout << ERROR_MESSAGE; 
		exit(1); 
		}

	std::cout << "Enter k: ";
	if (!(std::cin >> k)) { 
		std::cout << ERROR_MESSAGE; 
		exit(1); 
		}

	std::cout << "Enter strategy_num: ";
	if (!(std::cin >> strategyNum)) { 
		std::cout << ERROR_MESSAGE; 
		exit(1); 
		}

	std::cout << "Enter seed: ";
	if (!(std::cin >> seed)) { 
		std::cout << ERROR_MESSAGE; 
		exit(1); 
		}
	std::cout << std::endl;

	if (n <= 0 || k <= 0 || n > k || k > 9 || (strategyNum != 1 && strategyNum != 2)) {
		std::cout << ERROR_MESSAGE;
		exit(1);
	}

	runGame(n, k, strategyNum, seed);
	return 0;
}