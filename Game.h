#pragma once
#ifndef GAME_H
#define GAME_H
#include <vector>
#include <string>
#include <cstdio>
#include <algorithm>

class Game
{
public:
	Game(const std::vector<std::string> &words);
	bool setSecretWord(const std::string &sw);
	int secretWordLength() const;
	void probe(const std::string &probeWord, int &nInRightPlace, int &nInWrongPlace);
	~Game();
private:
	Game(const Game&) { }
	Game& operator=(const Game&) { }
	char *m_secretWord;
	int m_size, m_time;
	std::vector<std::string> wordList;
	void ShowTime();
};
#endif