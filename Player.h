#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "Game.h"
class Player
{
public:
	Player(const std::vector<std::string> &words);
	std::string generateProbeWord();
	void learn(std::string probe, int nInRightPlace, int nInWrongPlace);
	~Player();
private:	
	Player(const Player&) { }
	Player& operator=(const Player&) { }
	std::vector<std::string> wordList;
	std::vector<double> wordEntropy;
	std::vector<bool> wordExists;
	int alphabetFreq[26];
	double alphabetProb[26];
	double sum;

	// utilities
	double calcEntropy(const std::string word);
	void genAlphaProb();
	void updateEntropy();
	void eliminate(int i);
	int findNext();
	bool cowsBulls(const std::string secret, const std::string probe, int& cows, int& bulls);

	// first few times
	bool alphaBool[26];
	bool initialized;
	bool firstRun;
	bool firstTry;
	int start;

	// use memory
	std::vector<std::string> memory;
	std::vector<int> memoryBulls;
	std::vector<int> memoryCows;
	int memorized;
};

#endif