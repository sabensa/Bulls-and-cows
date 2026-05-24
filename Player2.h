#pragma once
#include "Player.h"
#include <string>
#include <vector>
#include <utility>
#include <map>

const std::string ERROR_MESSAGE = "invalid input\n";

class Player2 : public Player
{
    int strategyNum;
    std::vector<std::string> S;
    std::vector<std::pair<std::string, std::pair<int, int>>> guessesFeedbacksSeries;
    std::string current_guess;

    void generateAllValidGuesses(std::string currentGuess, std::vector<bool>& used);
    bool isConsistentGuess(const std::string& guess, const std::vector<std::pair<std::string, std::pair<int, int>>>& history) const;
    std::string getSmallestConsistentGuess() const;
    std::vector<std::string> get_sub_group_of_S_with_only_consistent_guesses_by_f_and_g(const std::string& g, const std::pair<int, int>& f) const;
    std::string get_smallest_lex_guess_with_minimum_maximum_sub_group_size() const;

public:
    Player2(int n, int k, int strategyNum);

    std::string makeGuess() {return current_guess;}
    void receiveFeedback(const std::pair<int, int>& feedback);

    const std::vector<std::string>& getS() const {return S;}
    bool isSEmpty() const {return S.empty();}
};

