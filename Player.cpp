#include "Player.h"
#include <iostream>
using namespace std;

bool Player::cowsBulls(const string secret, const string probe, int& bulls, int& cows) {
	int Bull = 0, Cow = 0;
	string probeWord = probe;
	string secretWord = secret;
	for (int j = 0; j < secretWord.length(); j++)
		for (int i = 0; i < probeWord.length(); i++) {
			if (probeWord[i] == secretWord[j] && islower(probeWord[i])) {
				if (i == j)
					Bull++;
				else
					Cow++;
				probeWord[i] = ' ';
				secretWord[j] = ' ';
				j++;
				i = 0;
			}
		}
	bulls = Bull;
	cows = Cow;
	if (cows == 0)
		return false;
	else
		return true;
}

double Player::calcEntropy(const string word) {
	double entropy = 0.0;
	for (int i = 0; i < word.length(); i++) {
		double prob = alphabetProb[word[i] - 'a'];
		if (initialized || !alphaBool[word[i] - 'a'])
			entropy -= prob * log2(prob);
	}
	return entropy;
}

void Player::genAlphaProb() {
	for (int i = 0; i < 26; i++)
		alphabetFreq[i] = 0;
	for (int i = 0; i < wordList.size(); i++) {
		bool haveBeen[26];
		for (int j = 0; j < 26; j++)
			haveBeen[j] = false;
		for (int j = 0; j < wordList[i].length(); j++) 
			if (!haveBeen[wordList[i][j] - 'a']) {
				alphabetFreq[wordList[i][j] - 'a']++;
				haveBeen[wordList[i][j] - 'a'] = true;
			}
	}
	sum = wordList.size();
	for (int i = 0; i < 26; i++)
		alphabetProb[i] = alphabetFreq[i] / sum;
}

void Player::eliminate(int i) {
	bool haveBeen[26];
	for (int j = 0; j < 26; j++)
		haveBeen[j] = false;
	for (int j = 0; j < wordList[i].length(); j++)
		if (!haveBeen[wordList[i][j] - 'a']) {
			alphabetFreq[wordList[i][j] - 'a']--;
			haveBeen[wordList[i][j] - 'a'] = true;
		}
	sum--;
	wordExists[i] = false;
	for (int i = 0; i < 26; i++)
		alphabetProb[i] = alphabetFreq[i] / sum;
}

int Player::findNext() {
	double maxEnt = 0.0;
	string maxStr = "";
	int maxIndex = 0;
	for (int i = 0; i < wordList.size(); i++) {
		if (wordEntropy[i] > maxEnt && wordExists[i]) {
			maxEnt = wordEntropy[i];
			maxStr = wordList[i];
			maxIndex = i;
		}
	}
	cerr << maxStr << "\t" << maxEnt << "\t" << maxIndex << "\t" << sum << endl;
	int bulls, cows;
	if (!initialized) {
		for (int i = 0; i < maxStr.length(); i++)
			alphaBool[maxStr[i] - 'a'] = true;
		for (int i = 0; i < 26; i++)
			if (alphabetFreq[i] == 0)
				alphaBool[i] = true;
	}
	initialized = alphaBool[0];
	for (int i = 1; i < 26; i++)
		initialized &= alphaBool[i];
	/*
	if (!initialized) {
		for (int i = 0; i < 26; i++)
			cerr << alphaBool[i];
		cout << endl;
		for (int i = 0; i < 26; i++)
			cerr << alphabetFreq[i] << "\t";
		cout << endl;
	}

	if (sum < 20) {
		for (int i = 0; i < wordList.size(); i++) {
			if (wordExists[i])
				cerr << wordList[i] << endl;
		}
	}*/
	eliminate(maxIndex);
	return maxIndex;
}

void Player::updateEntropy() {
	for (int i = 0; i < wordList.size(); i++) {
		if (firstRun) {
			double entropy = calcEntropy(wordList[i]);
			wordEntropy.push_back(entropy);
			if (i == wordList.size()-1)
				firstRun = false;
		}
		else {
			if (wordExists[i]) {
				double entropy = calcEntropy(wordList[i]);
				wordEntropy[i] = entropy;
			}
		}
	}
}

Player::Player(const vector<std::string> &words) {
	wordList = words;
	genAlphaProb();
	for (int i = 0; i < wordList.size(); i++) 
		wordExists.push_back(true);
	for (int i = 0; i < 26; i++)
		alphaBool[i] = false;
	initialized = alphaBool[0];
	for (int i = 1; i < 26; i++)
		initialized &= alphaBool[i];
	firstRun = true;
	updateEntropy();
	double maxEnt = 0.0;
	string maxStr = "";
	int maxIndex = 0;
	for (int i = 0; i < wordList.size(); i++) {
		if (wordEntropy[i] > maxEnt && wordExists[i]) {
			maxEnt = wordEntropy[i];
			maxStr = wordList[i];
			maxIndex = i;
		}
	}
	cerr << maxStr << "\t" << maxEnt << "\t" << maxIndex << "\t" << sum << endl;
	start = maxIndex;
	firstTry = true;
	memorized = 0;
}

std::string Player::generateProbeWord() {
	if (firstTry) {
		firstTry = false;
		return wordList[start];
	}
	else if (sum>1)
		return wordList[findNext()];
	else 
		for (int i = 0; i < wordList.size(); i++) 
			if (wordExists[i])
				return wordList[i];
}

void Player::learn(std::string probe, int nInRightPlace, int nInWrongPlace) {
	for (int i = 0; i < memory.size(); i++)
		cerr << memory[i] << "\t" << memoryBulls[i] - memorized << "\t" << memoryCows[i] << endl;

	for (int i = 0; i < wordList.size(); i++) 
		if (wordExists[i]){
			int bull, cow;
			cowsBulls(wordList[i], probe, bull, cow); 
			if (nInRightPlace == 0 && bull !=0) {
				eliminate(i);
				continue;
			}
			else if (bull + cow < nInRightPlace + nInWrongPlace) {
				eliminate(i);
				continue;
			}
			else if (bull < nInRightPlace) {
				eliminate(i);
				continue;
			}
		}
	memory.push_back(probe);
	memoryBulls.push_back(nInRightPlace);
	memoryCows.push_back(nInWrongPlace);
	updateEntropy();
}

Player::~Player() {

}