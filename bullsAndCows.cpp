#include "GameRun.h"
#include <iostream>
#include <stdlib.h>

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

	GameRun game (n, k, strategyNum, seed);
	game.play();
	return 0;
}